# Mapreduce-0

# MapReduce原理

Mapreduce是一个分布式运算程序的编程框架，是用户开发“基于hadoop的数据分析应用”的核心框架；
Mapreduce核心功能是将用户编写的业务逻辑代码和自带默认组件整合成一个完整的分布式运算程序，并发运行在一个hadoop集群上；

## 为什么要MapReduce
1. 海量数据在单机上处理因为硬件资源限制，无法胜任
2. 而一旦将单机版程序扩展到集群来分布式运行，将极大增加程序的复杂度和开发难度
3. 引入mapreduce框架后，开发人员可以将绝大部分工作集中在业务逻辑的开发上，而将分布式计算中的复杂性交由框架来处理

## MapReduce框架结构及核心运行机制
###  结构
一个完整的mapreduce程序在分布式运行时有三类实例进程：
1. MRAppMaster：负责整个程序的过程调度及状态协调

2. mapTask：负责map阶段的整个数据处理流程

3. ReduceTask：负责reduce阶段的整个数据处理流程

### MR程序运行流程
 流程示意图

![Screen Shot 2020-04-24 at 09.23.55](/Users/dosdrtt/Desktop/Screen Shot 2020-04-24 at 09.23.55.png)
### 流程解析
1. 一个mr程序启动的时候，最先启动的是MRAppMaster，MRAppMaster启动后根据本次job的描述信息，计算出需要的maptask实例数量，然后向集群申请机器启动相应数量的maptask进程
2. maptask进程启动之后，根据给定的数据切片范围进行数据处理，主体流程为：
利用客户指定的inputformat来获取RecordReader读取数据，形成输入KV对
将输入KV对传递给客户定义的map()方法，做逻辑运算，并将map()方法输出的KV对收集到缓存
将缓存中的KV对按照K分区排序后不断溢写到磁盘文件
3. MRAppMaster监控到所有maptask进程任务完成之后，会根据客户指定的参数启动相应数量的reducetask进程，并告知reducetask进程要处理的数据范围（数据分区）
4. Reducetask进程启动之后，根据MRAppMaster告知的待处理数据所在位置，从若干台maptask运行所在机器上获取到若干个maptask输出结果文件，并在本地进行重新归并排序，然后按照相同key的KV为一个组，调用客户定义的reduce()方法进行逻辑运算，并收集运算输出的结果KV，然后调用客户指定的outputformat将结果数据输出到外部存储

### MapTask并行度决定机制
maptask的并行度决定map阶段的任务处理并发度，进而影响到整个job的处理速度
那么，mapTask并行实例是否越多越好呢？其并行度又是如何决定呢？

### mapTask并行度的决定机制
一个job的map阶段并行度由客户端在提交job时决定,而客户端对map阶段并行度的规划的基本逻辑为：
将待处理数据执行逻辑切片（即按照一个特定切片大小，将待处理数据划分成逻辑上的多个split），然后每一个split分配一个mapTask并行实例处理
这段逻辑及形成的切片规划描述文件，由FileInputFormat实现类的getSplits()方法完成，其过程如下图：

### FileInputFormat切片机制
1. 切片定义在InputFormat类中的getSplit()方法
2. ileInputFormat中默认的切片机制：

简单地按照文件的内容长度进行切片,切片大小，默认等于block大小,切片时不考虑数据集整体，而是逐个针对每一个文件单独切片
比如待处理数据有两个文件：
file1.txt    320M
file2.txt    10M
经过FileInputFormat的切片机制运算后，形成的切片信息如下：  
file1.txt.split1--  0~128  
file1.txt.split2--  128~256  
file1.txt.split3--  256~320  
file2.txt.split1--  0~10M

3. FileInputFormat中切片的大小的参数配置

通过分析源码，在FileInputFormat中，计算切片大小的逻辑：Math.max(minSize, Math.min(maxSize, blockSize));  切片主要由这几个值来运算决定

minsize：默认值：1  
配置参数： mapreduce.input.fileinputformat.split.minsize    

maxsize：默认值：Long.MAXValue  
配置参数：`mapreduce.input.fileinputformat.split.maxsize=blocksize`

因此，默认情况下，切片大小=blocksize
maxsize（切片最大值）：
参数如果调得比blocksize小，则会让切片变小，而且就等于配置的这个参数的值

minsize （切片最小值）：

参数调的比blockSize大，则可以让切片变得比blocksize还大

选择并发数的影响因素：

运算节点的硬件配置
运算任务的类型：CPU密集型还是IO密集型
运算任务的数据量
1.4 ReduceTask并行度的决定
reducetask的并行度同样影响整个job的执行并发度和执行效率，但与maptask的并发数由切片数决定不同，Reducetask数量的决定是可以直接手动设置：

//默认值是1，手动设置为4

job.setNumReduceTasks(4);

如果数据分布不均匀，就有可能在reduce阶段产生数据倾斜

注意： reducetask数量并不是任意设置，还要考虑业务逻辑需求，有些情况下，需要计算全局汇总结果，就只能有1个reducetask

尽量不要运行太多的reduce task。对大多数job来说，最好rduce的个数最多和集群中的reduce持平，或者比集群的 reduce slots小。这个对于小集群而言，尤其重要。

