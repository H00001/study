# hadoop shuffle 

## 1.什么是Shuffle机制

在Hadoop中数据从Map阶段传递给Reduce阶段的过程就叫Shuffle，Shuffle机制是整个MapReduce框架中最核心的部分。

Shuffle翻译成中文的意思为：洗牌、发牌（核心机制：数据分区、排序、缓存)

## Shuffle

一般把数据从Map阶段输出到Reduce阶段的过程叫Shuffle，所以Shuffle的作用范围是Map阶段数据输出到Reduce阶段数据输入这一整个中间过程！

**![img](https://img-blog.csdn.net/2018061422060152)**





map任务数是和我们的逻辑片数有关，有几个逻辑片就有几个map任务，而我们的reduce任务是和我们的分区数相关，有几个分区就对应几个reduce任务。



## Shuffle的执行阶段流程

1).Collect阶段：将MapTask的结果输出到默认大小为100M的环形缓冲区，保存的是key/value序列化数据，Partition分区信息等。

2).Spill 阶段：当内存中的数据量达到一定的阀值的时候，就会将数据写入本地磁盘，在将数据写入磁盘之前需要对数据进行一次排序的操作，如果配置了combiner，还会将有相同分区号和key的数据进行排序。 

3).Merge 阶段：把所有溢出的临时文件进行一次合并操作，以确保一个MapTask最终只产生一个中间数据文件。

4).Copy阶段： ReduceTask启动Fetcher线程到已经完成MapTask的节点上复制一份属于自己的数据，这些数据默认会保存在内存的缓冲区中，当内存的缓冲区达到一定的阀值的时候，就会将数据写到磁盘之上。

5).Merge阶段：在ReduceTask远程复制数据的同时，会在后台开启两个线程(一个是内存到磁盘的合并，一个是磁盘到磁盘的合并)对内存到本地的数据文件进行合并操作。

6).Sort阶段：在对数据进行合并的同时，会进行排序操作，由于MapTask 阶段已经对数据进行了局部的排序，ReduceTask只需保证Copy的数据的最终整体有效性即可