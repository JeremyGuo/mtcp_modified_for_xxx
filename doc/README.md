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

## 安装方法

1. 按照mTCP方法运行配置脚本

2. 在设置完成之后添加环境变量

3. >  注意到可能无法通过编译，修改`core.c 1332,1333`行注释掉编译错误的两行

4. >  在`eth_in.h`和`eth_out.h`中加入`myqueue`的部分代码

5. 运行configure后，修改/mtcp/src/Makefile，加入`myqueue.c`

6. 注意要使用正确版本的autoconf工具

> 最大的建议就是不要在虚拟机上玩mTCP



## 在虚拟机上安装mTCP的痛苦记忆

* DPDK对于新版的内核支持不是很好（建议14.04以下）

* 运行的时候必须用管理员权限（建议`sudo su root`）

* UIO驱动和DPDK是绑到一起的，所以在修改完DPDK的代码并重新编译之后，需要重新插入驱动

* 页面大小过小会导致`exiting with code: 1 Cause: Creation of mbuf pool for socket 0`，需要调大页面大小

* `Error reading from file descriptor 13: Input/output error`是因为虚拟机里运行的网卡和真实的网卡存在差异，一些验证无法通过，打开`lib/librte_eal/linuxapp/igb_uio/igb_uio.c`

  修改代码为：

  ```c
  if (pci_intx_mask_supported(dev) || true) { 
  	 dev_dbg(&dev->dev, "using INTX");
  	 udev->info.irq_flags = IRQF_SHARED;
  	 udev->info.irq = dev->irq;  
  	 udev->mode = RTE_INTR_MODE_LEGACY;     
  	 break;             
   }dev_notice(&dev->dev, "PCI INTX mask not supported\n");
  ```

  **注意** 别忘了第一条

* > 推测：版本太老，无法支持虚拟机网卡，明天尝试16版本号的

* 事实上，我们发现代码出现问题在虚拟机无法获取正确的socket号

* > 问题：修改dpdk代码无法在中间插入自己的调试信息

  于是我直接将/mtcp/dpdk-iface-kmod/dpdk_iface_main.c中的Can't determine socket id错误所需要进行的判断直接注释掉....就正常了...

* mtcp自己编译的时候忘了加-lrt，需要修改环境变量LIBS解决。

* 注意使用参数`CFLAGS="-DMAX_CPUS=32"`限制CPU数量

* 注意除了提到的还要安装automake

* 注意上述包括automake在Ubuntu 12.02下不存在可以直接用 apt-get intall 的版本需要自行安装

  ```bash
  http://ftp.gnu.org/gnu/automake/automake-1.15.tar.gz
  ./configure --docdir=/usr/share/doc/automake-1.15
  
  ftp://ftp.gnu.org/gnu/autoconf/autoconf-2.69.tar.gz
  ./configure --prefix=/usr/
  
  make && make install
  ```

* 由于要使用`rte_thread_set_affinity( & cpu )`函数，在DPDK1.4中不提供，我在2.1`lib/librte_eal/common/include/rte_lcore.h`中找到了相关定义

  > 。。。。。我们重新开始吧。。。。

* 由于要使用`rte_mempool_free()`函数，只有DPDK16以上才有

  > 。。。。。我们再来一次吧。。。。

* 由于DPDK19不支持Kernel....所以我们退回到DPDK16.11LTS

* 在`make`之后出现了神奇的错误，即然他自己都不知道有啥用（狗头）我们直接删（注释）了就好。。。

* 每次开机似乎都要重新加载igb_uio的驱动，并且重新设置页面大小

