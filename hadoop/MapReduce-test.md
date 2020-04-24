
# MapReduce-test
## mapreduce示例编写及编程规范
### 编程规范
用户编写的程序分成三个部分：Mapper，Reducer，Driver(提交运行mr程序的客户端)
Mapper的输入数据是KV对的形式（KV的类型可自定义）
Mapper的输出数据是KV对的形式（KV的类型可自定义）
Mapper中的业务逻辑写在map()方法中
map()方法（maptask进程）对每一个<K,V>调用一次
Reducer的输入数据类型对应Mapper的输出数据类型，也是KV
Reducer的业务逻辑写在reduce()方法中
Reducetask进程对每一组相同k的<k,v>组调用一次reduce()方法
用户自定义的Mapper和Reducer都要继承各自的父类
整个程序需要一个Drvier来进行提交，提交的是一个描述了各种必要信息的job对象
# wordcount示例编写

需求：在一堆给定的文本文件中统计输出每一个单词出现的总次数
定义一个mapper类
//首先要定义四个泛型的类型
//keyin:  LongWritable    valuein: Text
//keyout: Text            valueout:IntWritable

```
public class WordCountMapper extends Mapper<LongWritable, Text, Text, IntWritable>{
	//map方法的生命周期：  框架每传一行数据就被调用一次
	//key :  这一行的起始点在文件中的偏移量
	//value: 这一行的内容
	@Override
	protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
		//拿到一行数据转换为string
		String line = value.toString();
		//将这一行切分出各个单词
		String[] words = line.split(" ");
		//遍历数组，输出<单词，1>
		for(String word:words){
			context.write(new Text(word), new IntWritable(1));
		}
	}
}
```
将单词作为key，次数1为value。以便于后续的数据分发，根据单词分发/相同的单词分到相同的reduce task

定义一个reducer类

//生命周期：框架每传递进来一个kv 组，reduce方法被调用一次
```
	@Override
	protected void reduce(Text key, Iterable<IntWritable> values, Context context) throws IOException, InterruptedException {
		//定义一个计数器
		int count = 0;
		//遍历这一组kv的所有v，累加到count中
		for(IntWritable value:values){
			count += value.get();
		}
		context.write(key, new IntWritable(count));
	}
}
(3)定义一个主类，用来描述job并提交job

package top.gunplan.bigdata

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.CombineTextInputFormat;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class WordcountDriver {


	public static void main(String[] args) throws Exception {

		Configuration conf = new Configuration();
		conf.set("mapreduce.framework.name", "local");
		conf.set("fs.defaultFS", "file:///");
		conf.set("fs.defaultFS", "hdfs://localhost:9000/");
    Job job = Job.getInstance(conf);
		
		/*job.setJar("/home/hadoop/wc.jar");*/
		//指定本程序的jar包所在的本地路径
		job.setJarByClass(WordcountDriver.class);
		
		//指定本业务job要使用的mapper/Reducer业务类
		job.setMapperClass(WordcountMapper.class);
		job.setReducerClass(WordcountReduce.class);
		
		/*job.setCombinerClass(WordcountCombiner.class);*/
		job.setCombinerClass(WordcountReduce.class);
		
		/*job.setInputFormatClass(CombineTextInputFormat.class);
		CombineTextInputFormat.setMaxInputSplitSize(job, 4194304);
		CombineTextInputFormat.setMinInputSplitSize(job, 2097152);*/
		
		job.setMapOutputKeyClass(Text.class);
		job.setMapOutputValueClass(IntWritable.class);
		

		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(IntWritable.class);
		

		FileInputFormat.setInputPaths(job, new Path("/"));
		//指定job的输出结果所在目录
		FileOutputFormat.setOutputPath(job, new Path("/"));
		
		/*job.submit();*/
		boolean res = job.waitForCompletion(true);
		System.exit(res?0:1);
		
	}
}
```
2.2 mapreduce程序运行模式
2.2.1 本地运行模式
mapreduce程序是被提交给LocalJobRunner在本地以单进程的形式运行
而处理的数据及输出结果可以在本地文件系统，也可以在hdfs上
怎样实现本地运行？写一个程序，不要带集群的配置文件（本质是你的mr程序的conf中是否有mapreduce.framework.name=local以及yarn.resourcemanager.hostname参数）
本地模式非常便于进行业务逻辑的debug，只要在eclipse中打断点即可
如果在windows下想运行本地模式来测试程序逻辑，需要在windows中配置环境变量：

％HADOOP_HOME％  =  d:/hadoop-2.6.1

%PATH% =  ％HADOOP_HOME％\bin

并且要将d:/hadoop-2.6.1的lib和bin目录替换成windows平台编译的版本

### MapReduce中的Combiner
combiner是MR程序中Mapper和Reducer之外的一种组件
combiner组件的父类就是Reducer
combiner和reducer的区别在于运行的位置：
Combiner是在每一个maptask所在的节点运行
Reducer是接收全局所有Mapper的输出结果；

> combiner的意义就是对每一个maptask的输出进行局部汇总，以减小网络传输量

具体实现步骤：

自定义一个combiner继承Reducer，重写reduce方法
在job中设置：  job.setCombinerClass(CustomCombiner.class)
(5) combiner能够应用的前提是不能影响最终的业务逻辑

而且，combiner的输出kv应该跟reducer的输入kv类型要对应起来

 

## MapReduce原理篇

### mapreduce的shuffle机制
### 概述：

mapreduce中，map阶段处理的数据如何传递给reduce阶段，是mapreduce框架中最关键的一个流程，这个流程就叫shuffle；
shuffle: 洗牌、发牌——（核心机制：数据分区，排序，缓存）；
具体来说：就是将maptask输出的处理结果数据，分发给reducetask，并在分发的过程中，对数据按key进行了分区和排序；
主要流程


shuffle是MR处理流程中的一个过程，它的每一个处理步骤是分散在各个map task和reduce task节点上完成的，整体来看，分为3个操作：

1. 分区partition
2. Sort根据key排序
3. Combiner进行局部value的合并
   详细流程
   maptask收集我们的map()方法输出的kv对，放到内存缓冲区中
   从内存缓冲区不断溢出本地磁盘文件，可能会溢出多个文件 （默认100M）
   多个溢出文件会被合并成大的溢出文件
   在溢出过程中，及合并的过程中，都要调用partitoner进行分组和针对key进行排序
   reducetask根据自己的分区号，去各个maptask机器上取相应的结果分区数据
   reducetask会取到同一个分区的来自不同maptask的结果文件，reducetask会将这些文件再进行合并（归并排序）
   合并成大文件后，shuffle的过程也就结束了，后面进入reducetask的逻辑运算过程（从文件中取出一个一个的键值对group，调用用户自定义的reduce()方法）
   Shuffle中的缓冲区大小会影响到mapreduce程序的执行效率，原则上说，缓冲区越大，磁盘io的次数越少，执行速度就越快

缓冲区的大小可以通过参数调整,  参数：io.sort.mb  默认100M

 

## yarn的重要概念
yarn并不清楚用户提交的程序的运行机制
yarn只提供运算资源的调度（用户程序向yarn申请资源，yarn就负责分配资源）
yarn中的主管角色叫ResourceManager
yarn中具体提供运算资源的角色叫NodeManager
这样一来，yarn其实就与运行的用户程序完全解耦，就意味着yarn上可以运行各种类型的分布式运算程序（mapreduce只是其中的一种），比如mapreduce、storm程序，spark程序，tez ……
所以，spark、storm等运算框架都可以整合在yarn上运行，只要他们各自的框架中有符合yarn规范的资源请求机制即可
Yarn就成为一个通用的资源调度平台，从此，企业中以前存在的各种运算集群都可以整合在一个物理集群上，提高资源利用率，方便数据共享


🌰

对日志数据中的上下行流量信息汇总，并输出按照总流量倒序排序的结果

数据如下：

1363157985066 13726230503 00-FD-07-A4-72-B8:CMCC 120.196.100.82 24 27 2481 24681 200

1363157995052 13826544101 5C-0E-8B-C7-F1-E0:CMCC 120.197.40.4 4 0 264 0 200

1363157991076 13926435656 20-10-7A-28-CC-0A:CMCC 120.196.100.99 2 4 132 1512 200

1363154400022 13926251106 5C-0E-8B-8B-B1-50:CMCC 120.197.40.4 4 0 240 0 200

## 分析
基本思路：实现自定义的bean来封装流量信息，并将bean作为map输出的key来传输

MR程序在处理数据的过程中会对数据排序(map输出的kv对传输到reduce之前，会排序)，排序的依据是map输出的key

所以，我们如果要实现自己需要的排序规则，则可以考虑将排序因素放到key中，让key实现接口：WritableComparable

然后重写key的compareTo方法

## 实现

1. 自定义bean
```
public class FlowBean implements WritableComparable<FlowBean>{
	
	long upflow;
	long downflow;
	long sumflow;
	
	//如果空参构造函数被覆盖，一定要显示定义一下，否则在反序列时会抛异常
	public FlowBean(){}
	
	public FlowBean(long upflow, long downflow) {
		super();
		this.upflow = upflow;
		this.downflow = downflow;
		this.sumflow = upflow + downflow;
	}
	
	public long getSumflow() {
		return sumflow;
	}
	 
	public void setSumflow(long sumflow) {
		this.sumflow = sumflow;
	}
	 
	public long getUpflow() {
		return upflow;
	}
	public void setUpflow(long upflow) {
		this.upflow = upflow;
	}
	public long getDownflow() {
		return downflow;
	}
	public void setDownflow(long downflow) {
		this.downflow = downflow;
	}
	 
	//序列化，将对象的字段信息写入输出流
	@Override
	public void write(DataOutput out) throws IOException {
		
		out.writeLong(upflow);
		out.writeLong(downflow);
		out.writeLong(sumflow);
		
	}
	 
	//反序列化，从输入流中读取各个字段信息
	@Override
	public void readFields(DataInput in) throws IOException {
		upflow = in.readLong();
		downflow = in.readLong();
		sumflow = in.readLong();
		
	}
```
2 mapper和reducer

```java
public class FlowCount {

	static class FlowCountMapper extends Mapper<LongWritable, Text, FlowBean,Text > {
	 
		@Override
		protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
	 
			String line = value.toString();
			String[] fields = line.split("\t");
			try {
				String phonenbr = fields[0];
	 
				long upflow = Long.parseLong(fields[1]);
				long dflow = Long.parseLong(fields[2]);
	 
				FlowBean flowBean = new FlowBean(upflow, dflow);
	 
				context.write(flowBean,new Text(phonenbr));
			} catch (Exception e) {
	 
				e.printStackTrace();
			}
	 
		}
	 
	}
	 
	static class FlowCountReducer extends Reducer<FlowBean,Text,Text, FlowBean> {
	 
		@Override
		protected void reduce(FlowBean bean, Iterable<Text> phonenbr, Context context) throws IOException, InterruptedException {
	 
			Text phoneNbr = phonenbr.iterator().next();
	 
			context.write(phoneNbr, bean);
	 
		}
	 
	}
	 
	public static void main(String[] args) throws Exception {
	 
		Configuration conf = new Configuration();
	 
		Job job = Job.getInstance(conf);
	 
		job.setJarByClass(FlowCount.class);
	 
		job.setMapperClass(FlowCountMapper.class);
		job.setReducerClass(FlowCountReducer.class);
	 
		 job.setMapOutputKeyClass(FlowBean.class);
		 job.setMapOutputValueClass(Text.class);
	 
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(FlowBean.class);
	 
		// job.setInputFormatClass(TextInputFormat.class);
	 
		FileInputFormat.setInputPaths(job, new Path(args[0]));
		FileOutputFormat.setOutputPath(job, new Path(args[1]));
	 
		job.waitForCompletion(true);
	 
	}


```