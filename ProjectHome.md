Linux系统下的H3C 802.1X PAP认证客户端，基于libpcap，适用于H3C 802.1X PAP认证环境，如中山大学东校区。

# 特性 #
  * 版本号上传(iNode v5.0) 更新日期：2011-11-27
  * 跨平台软件，可以运行在x86、ARM、MIPS等体系结构计算机上。
  * 掉线重拨
  * 支持外部DHCP客户端（或者拨号成功后调用任何程序）
  * 广播/多播触发
  * 后台运行

# 特别说明 #
校园网已经提供2个iNode for Windows版本：
  1. Inode 802.1X Client V3.60 for Windows7/Vista/XP/Server2000/Server2003客户端
  1. Inode 802.1X Client V5.0 for Windows7/Vista/XP/Server2000/Server2003客户端
此外，也提供闭源的非windows客户端：
  1. Inode 802.1X Client for Mac os认证客户端
  1. Inode 802.1X Client for Linux 认证客户端

我们提供的开源版本主要面向**特殊人士**，谢谢您的关注！

# 使用范例 #
```
wget http://esysu-auth-client.googlecode.com/files/EsysuAuthClient-x.x.tar.gz
tar zxf EsysuAuthClient-x.x.tar.gz
cd EsysuAuthClient-x.x
make
sudo ./EsysuAuthClient -n eth0 -u netid -p yourpasswd
--eth0: Send EAPOL_START
...'
...
^Ceth0: Send EAPOL_Logoff
```

# 使用参数 #
```
EsysuAuthClient 1.0  --- An 802.1X PAP Client
mzguanglin@gmail.com (2010)
-n <Interface> Specify network interface
-d run as deamon
-D <DHCP Client> Need DHCP after being authenticated
-u <username> Specify username
-p <password> Specify password
-t <triggle type> broadcast / multicast
-P <pid file> Store process ID of daemon in file
-h Help information
```