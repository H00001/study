# Hadoop-cluster-install

## write core-site.xml

```bash
<configuration>
    <property>
   <name>hadoop.tmp.dir</name>
   <value>file:/usr/hdfs/tmp</value>
   <description>A base for other temporary directories.</description>
 </property>
 <property>
  <name>io.file.buffer.size</name>
   <value>131072</value>
 </property>
 <property>
   <name>fs.defaultFS</name>
   <value>hdfs://h0.gunplan.top:9000</value>
 </property>
</configuration>
```

## Write hdfs-siit 

```xml
<configuration>         
<property>                 
<name>dfs.namenode.secondary.http-address</name>                 <value>h0.gunplan.top:9001</value>         
</property>         
<property>                 <name>dfs.namenode.name.dir</name>                 <value>file:/usr/dfs/name</value>         </property>         <property>                 <name>dfs.datanode.data.dir</name>                 <value>file:/usr/dfs/data</value>         </property>         
  <property>                 <name>dfs.replication</name>                 <value>2</value>         
</property>         <property>                 <name>dfs.webhdfs.enabled</name>                 
<value>true</value>         </property>         
<property>                 
<name>dfs.permissions</name>                 
<value>false</value>        
</property>         
<property>                 
<name>dfs.web.ugi</name>                 
<value>supergroup</value>         </property> </configuration>
```

## write maprted-site.xml

```xml
<configuration>         
  <property>                 
    <name>mapreduce.framework.name</name>                 
    <value>yarn</value>         
  </property>         
  <property>                 
    <name>mapreduce.jobhistory.address</name>               
    <value>h0.gunplan.top:10020</value>         
  </property>         
  <property>                 
    <name>mapreduce.jobhistory.webapp.address</name>   
    <value>h0.gunplan.top:19888</value>         
  </property> 
</configuration>
```

## Write yarn-site,xml

```
<configuration>
        <property>
                <name>yarn.nodemanager.aux-services</name>
                <value>mapreduce_shuffle</value>
        </property>
        <property>
                <name>yarn.nodemanager.aux-services.mapreduce.shuffle.class</name>
                <value>org.apache.hadoop.mapred.ShuffleHandler</value>
        </property>
        <property>
                <name>yarn.resourcemanager.address</name>
                <value>h0.gunplan.top:8032</value>
        </property>
        <property>
                <name>yarn.resourcemanager.scheduler.address</name>
                <value>h0.gunplan.top:8030</value>
        </property>
        <property>
                <name>yarn.resourcemanager.resource-tracker.address</name>
                <value>h0.gunplan.top:8031</value>
        </property>
        <property>
                <name>yarn.resourcemanager.admin.address</name>
                <value>h0.gunplan.top:8033</value>
        </property>
        <property>
                <name>yarn.resourcemanager.webapp.address</name>
                <value>h0.gunplan.top:8088</value>
        </property>
</configuration>
```

## copy 

```
cp -r /mnt/nfs/hadoop-2.10.0/ ./
```

## export java home

```export JAVA_HOME="/mnt/nfs/jdk-14.0.1"```

