# yarn执行mr流程

### ![img](https://img2018.cnblogs.com/blog/1460324/201809/1460324-20180914145344291-1044324612.png)

 

### 服务功能

ResouceManager： 
  1. 处理客户端的请求 
    2、启动和监控ApplicationMaster 
    3、监控nodemanager 
    4、资源的分配和调度

Nodemanager 
  1、处理单个节点的资源管理 
  2、处理来自ResouceManager的命令 
  3、处理来自ApplicationMaster的命令

ApplicationMaser 
  1、为应用程序申请资源，并分配给内部任务 
  2、任务的监控和容错

Container 
对多任务运行环境的抽象，包括CPU、内存等多维度资源以及环境变量、启动命令等任务运行的相关环境

### 运行流程

1.client向yarn提交job，首先找ResourceManager分配资源，
2.ResourceManager开启一个Container,在Container中运行一个Application manager
3.Application manager找一台nodemanager启动Application master，计算任务所需的计算
4.Application master向Application manager（Yarn）申请运行任务所需的资源
5.Resource scheduler将资源封装发给Application master
6.Application master将获取到的资源分配给各个nodemanager
7.各个nodemanager得到任务和资源开始执行map task
8.map task执行结束后，开始执行reduce task
9.map task和 reduce task将执行结果反馈给Application master  
10.Application master将任务执行的结果反馈application manager。



## mrapplication

 MRAppMaster工作流程

3.1用户向YARN中（RM）提交应用程序，其中包括ApplicationMaster程序、启动ApplicationMaster的命令、用户程序等。

3.2ResourceManager为该应用程序分配第一个Container，ResouceManag与某个NodeManager通信，启动应用程序ApplicationMaster，NodeManager接到命令后，首先从HDFS上下载文件(缓存），然后启动ApplicationMaser。

3.3 当ApplicationMaster启动后，它与ResouceManager通信，以请求和获取资源。ApplicationMaster获取到资源后，与对应NodeManager通信以启动任务。

注：

1.如果该应用程序第一次在给节点上启动任务，则NodeManager首先从HDFS上下载文件缓存到本地，这个是由分布式缓存实现的，然后启动该任务。

 2.分布式缓存并不是将文件缓存到集群中各个结点的内存中，而是将文件换到各个结点的磁盘上，以便执行任务时候直接从本地磁盘上读取文件。

4ApplicationMaster首先向ResourceManager注册，这样用户可以直接通过ResourceManage查看应用程序的运行状态，然后它将为各个任务申请资源，并监控它们的运行状态，直到运行结束，即重复步骤5~8

5ApplicationMaster采用轮询的方式通过RPC协议向ResourceManager申请和领取资源

6一旦ApplicationMaster申请到资源后，ApplicationMaster就会将启动命令交给NodeManager,要求它启动任务。启动命令里包含了一些信息使得Container可以与ApplicationMaster进行通信。

7NodeManager为任务设置好运行环境（包括环境变量、JAR包、二进制程序等）后，将任务启动命令写到一个脚本中，并通过运行该脚本启动任务（Container）。

8在应用程序运行过程中，用户可随时通过RPC向ApplicationMaster查询应用程序的当前运行状态

9应用程序运行完成后，ApplicationMaster向ResourceManager注销并关闭自己

# MR 运行原理

![20200212141218611](/Users/dosdrtt/Desktop/20200212141218611.jpg)

2.MapTask的详细流程:
1.提交相应的信息到mr appmaster
(1)都回提交哪些信息？

split.xml 配置信息
jar包
切片信息.mrappmaster根据切片信息开启对应数量的maptask
(2) 切片信息怎么得到？
默认TextInputFormat调用父类FileInputPutFormat 中getSplits方法得到切片信息。
再调用createRecordReader 返回RecordReader对象读取切片记录。默认使用LineRecordreader 读取切片信息。行偏移量作为key，内容作为value。RecordReader会在输入块上被反复调用，直到整个输入块被处理完毕，每一次调用RecordReader都会调用Mapper类的map()函数。