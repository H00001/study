# HADOOP-install-prepare

## download Hadoop

```wget https://mirror.bit.edu.cn/apache/hadoop/common/hadoop-2.10.0/hadoop-2.10.0.tar.gz```

## set nfs share

```bash 
echo "/directory *(ro,no_subtree_check)" >> /etc/exports
exportfs -r
```
## probe mount

```bash
showmount -e h0.gunplan.top
```

## mount directory

```bash
echo "h0.gunplan.top:/share /mnt/nfs nfs defaults 0 0" >> /etc/fstab
apt install nfs-common
mount -a
```

## copy hadoop
```bash
mkdir ~/hadoop && cp /mnt/nfs/hadoop-2.10.0.tar.gz ~/hadoop/
```
## set jdk

```
export JAVA_HOME=/mnt/nfs/java
export PATH=$PATH:$JAVA_HOME/bin
```

## add hosts

```
echo -e "123.56.223.150  h0.gunplan.top\n47.95.210.168   h1.gunplan.top\n47.101.52.8    h2.gunplan.top\n39.106.46.179   h3.gunplan.top" >> /etc/hosts
```

## set ssh-keygen

```
ssh-keygen -t ed25519
+--[ED25519 256]--+
|   .=.=o=.+.     |
|     Bo+.#..E    |
|     .o B.Xoo    |
|     .   OoB     |
|    .   S +.o .  |
|     . o   + o . |
|      o     * o  |
|       .   o = . |
|            +    |
+----[SHA256]-----+
ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAICRjV3DBfItUhxdNIlL43sNJmr5Q92mpSPkxwJ+kp4cH parallels@server-leader
```

