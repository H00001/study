## hadoop-command

> 这些命令和linux基本没有区别

创建目录

```hdfs dfs -mkdir /hom```

上传文件或目录到hdfs

```hadoop dfs -put hello /```
```hadoop dfs -put hellodir/ /```

查看目录

```hadoop dfs -ls /```

创建一个空文件

```hadoop dfs -touch /file```

删除一个文件

```hadoop dfs -rm /file```

删除一个目录

```hadoop dfs -rmr /home```

重命名

```hadoop dfs -mv /hello /hello2```

查看文件

```hadoop dfs -cat /hello```

将指定目录下的所有内容merge成一个文件，下载到本地

```hadoop dfs -getmerge /hellodir wa```

使用du文件和目录大小

```hadoop dfs -du /```

将目录拷贝到本地

```hadoop dfs -copyToLocal /home localdir```

查看dfs的情况
```hadoop dfsadmin -report```