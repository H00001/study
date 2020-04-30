# Hadoop序列化机制的特征

对于处理大规模数据的 Hadoop平台,其序列化机制需要具有如下特征:

1. 紧凑:由于带宽是 Hadoop集群中最稀缺的资源,一个紧凑的序列化机制可以充分利用数据中心的带宽。

2. 快速:在进程间通信(包括 MapReduce过程中涉及的数据交互)时会大量使用序列化机制,因此,必须尽量减少序列化和反序列化的开销

3. 可扩展:随着系统的发展,系统间通信的协议会升级,类的定义会发生变化,序列化机制需要支持这些升级和变化。

4. 互操作:可以支持不同开发语言间的通信,如C++和Java间的通信。这样的通信可以通过文件(需要精心设计文件的格式)或者后面介绍的IPC机制实现。

Java的序列化机制虽然强大，却不符合上面这些要求。 Java Serialization将每个对象的类名写入输出流中,这导致了Java序列化对象需要占用比原对象更多的存储空间。同时，为了减少数据量，同一个类的对象的序列化结果只输出一份元数据，并通过某种形式的引用,来共享元数据。引用导致对序列化后的流进行处理的时候，需要保持一些状态。想象如下一种场景，在一个上百G的文件中，反序列化某个对象，需要访问文件中前面的某一个元数据,这将导致这个文件不能切割，并通过 MapReduce来处理。同时，Java序列化会不断地创建新的对象，对于 MapReduce应用来说，不能重用对象，在已有对象上进行反序列化操作，而是不断创建反序列化的各种类型记录,这会带来大量的系统开销。


## Hadoop Writable框架

```java
@InterfaceAudience.Public
@InterfaceStability.Stable
public interface Writable {
  /** 
   * 序列化一个对象，将一个对象按照某个数据传输格式写入到out流中
   * Serialize the fields of this object to <code>out</code>.
   * 
   * @param out <code>DataOuput</code> to serialize this object into.
   * @throws IOException
   */
  void write(DataOutput out) throws IOException;

  /** 
   * 反序列化，从in流中读入字节，按照某个数据传输格式读出到一个对象中
   * Deserialize the fields of this object from <code>in</code>.  
   * 
   * <p>For efficiency, implementations should attempt to re-use storage in the 
   * existing object where possible.</p>
   * 
   * @param in <code>DataInput</code> to deseriablize this object from.
   * @throws IOException
   */
  void readFields(DataInput in) throws IOException;
}
```

## 5 代码示例

### 5.1 序列化反序列化工具函数



```java
/** 
 * 将一个实现了Writable接口的对象序列化成字节流 
 * @param writable 
 * @return 
 * @throws IOException 
 */ 
public static byte[] serialize2Bytes(Writable writable) throws IOException{
    ByteArrayOutputStream baos = new ByteArrayOutputStream() ;
    DataOutputStream dos = new DataOutputStream(baos);
    writable.write(dos);
    dos.close();
    baos.close();
    return baos.toByteArray();
}

 /** 
 * 将字节流转化为实现了Writable接口的对象 
 * @param writable 
 * @param bytes 
 * @return 
 * @throws IOException 
 */  
public static void deserializeFromBytes(Writable writable, byte[] bytes) throws IOException{
    ByteArrayInputStream bais = new ByteArrayInputStream(bytes) ;
    DataInputStream dataIn = new DataInputStream(bais) ;
    writable.readFields(dataIn) ;
    dataIn.close();
    bais.close();
}

/** 
 * 将一个实现了Writable接口的对象序列化到文件中
 * @param writable
 * @param file
 * @return 
 * @throws IOException 
 */ 
public static void serialize2File(Writable writable, File file) throws IOException{
    FileOutputStream fos = new FileOutputStream(file) ;
    DataOutputStream dos = new DataOutputStream(fos) ;
    writable.write(dos);
    dos.close();
    fos.close();
}

 /** 
 * 将文件输入流流转化为实现了Writable接口的对象 
 * @param writable 
 * @param file 
 * @return 
 * @throws IOException 
 */  
public static void deserializeFromBytes(Writable writable, File file) throws IOException{
    FileInputStream fis = new FileInputStream(file);
    DataInputStream dataIn = new DataInputStream(fis) ;
    writable.readFields(dataIn) ;
    dataIn.close();
    fis.close();
}
```

### 5.2 自定义类型示例（实现WritableComparable接口）

```java
package com.seriable;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.WritableComparable;

public class PeopleWritable implements WritableComparable<PeopleWritable> {
    private IntWritable age;
    private Text name;
    public PeopleWritable(){
    }
    public PeopleWritable(IntWritable age, Text name) {
        super();
        this.age = age;
        this.name = name;
    }
    public IntWritable getAge() {
        return age;
    }
    public void setAge(IntWritable age) {
        this.age = age;
    }
    public Text getName() {
        return name;
    }
    public void setName(Text name) {
        this.name = name;
    }
    //序列化方法
    public void write(DataOutput out) throws IOException {
        age.write(out);
        name.write(out);
    }
    //反序列化方法
    public void readFields(DataInput in) throws IOException {
        age.readFields(in);
        name.readFields(in);
    }
    //比较函数，使得对象可比较大小
    public int compareTo(PeopleWritable o) {
        int cmp = age.compareTo(o.getAge());
        if(0 !=cmp)return cmp;
        return name.compareTo(o.getName());
    }
}
```



