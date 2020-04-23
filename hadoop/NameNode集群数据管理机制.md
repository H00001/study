# NameNode集群数据管理机制

1. 客户端请求向服务器上传数据，发送信息给namenode，namenode将本次操作的内容记录在edits log文件内
2. 然后客户端开始上传数据，数据上传成功后向namenode发送完成信息，然后namenode在内存中写入本次上传的元数据
3. 同步内存的数据到fsimage文件中，同步时机在edits log满之前，刷新方式是将edits log和fsimage合并

edit log满了（64M）会通知second Namenode,seconde Namenode 通知namenode停止向edits
log写数据，但是此时服务器还在写数据，所以namenode 会新建一个edits log.

second namenode从namenode下载edits log和fsimage,然后合并生成fsimage.chkpoint。
second namenode上传fsimage.chkpoint给namenode.
namenode用fsimage.chkpoint替换fsimage.然后删除旧的edits log。然后把新的edits log重命名。

![Screen Shot 2020-04-23 at 10.55.23](/Users/dosdrtt/Library/Application Support/typora-user-images/Screen Shot 2020-04-23 at 10.55.23.png)

## namenode的职责

1.维护元数据
2.维护hdfs的目录树
3.响应客户端的请求

## 客户端职责

* 切分块

，对于客户端来说，写一个副本就可以认为成功，剩下的备份，可以通过dn之间传递

## Q&A

q:如果存在大量小的文件，那么浪费 namenode namenode的元数据空间。同时对于mapreduce损耗性能

q:namenode死机后还能提供服务吗？
a:使用双namenode,高可用机制

q:namenode 如果快速响应查找
a:可能存在内存中，通过mmap机制映射进入

