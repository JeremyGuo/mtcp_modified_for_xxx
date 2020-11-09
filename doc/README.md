目前已经使用了最新版的mTCP和DPDK

首先测试工具在/apps/pingpong/目录下，配置文件在/apps/pingpong/config.conf，配置按照mTCP的正常配置方法即可，目前仅使用单CPU

直接在/apps/pingpong/下运行make即可编译

运行时请使用sudo

如要修改监听的端口，请直接修改/apps/pingpong/pingpong.c中mtcp_bind中参数

pingpong服务运行流程：



1. 循环
   1. 和client连接
   2. 接受client发过来一个integer(n)
   3. 循环n次
      1. 发送上一次接受到的字符串（第一次为发送n的字符串）
      2. 接收一个新的字符串
   4. 断开和client的连接



该目录下提供了测使用程序（使用的Linux Socket）

使用方法为`./pingpong_cli 127.0.0.1 8000`
