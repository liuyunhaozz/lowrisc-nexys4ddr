# lowrisc-nexys4ddr
基于 `RISC-V` 架构的 `FPGA` 系统设计

## 1. 系统环境与所需工具

### 1.1 系统环境

宿主机：`Windows 10 64-bit`

虚拟机：`Ubuntu 16.04 LTS Desktop` (至少需要100G的硬盘空间)

### 1.2 所需软件

宿主机：

- `VMware Workstation Pro`  (安装虚拟机)

- `NetSarang Xshell ` (串口终端连接工具，用于连接FPGA进行 IO 操作)

虚拟机：

- `Vivado 2018.1 for Linux`
- `Oracle JDK8`

### 1.3 所需硬件

- `Nexys4-DDR` FPGA 
- `Micro SD`卡 (4G内存以上)
- 读卡器
- 路由器，网线 (可选)



## 2.  系统安装 && 项目环境设置

### 2.1 安装`Ubuntu 16.04 LTS`

下载地址：https://releases.ubuntu.com/16.04.7/ubuntu-16.04.7-desktop-amd64.iso

在 `VMware Workstation Pro` , 选取 `typical install`，载入镜像文件进行即可。注意至少分配100G的硬盘空间。因为安装 `Vivado` 和编译内核都需要大量的硬盘空间。

### 2.2 克隆项目文件

我们使用 ` Git` 进行项目管理，将本次项目所用到的所有文件发布在了 `Github release`  上。将下载的压缩包放入虚拟机的主目录 `~` 运行以下命令以进行下载

```sh
$ wget https://github.com/liuyunhaozz/lowrisc-nexys4ddr/releases/download/submit/lowrisc-nexys4ddr.tar.gz
$ tar xzvf lowrisc-nexys4ddr.tar.gz
```

- 注：本项目主要参考了https://github.com/lowRISC/lowrisc-chip，但是在实践的时候发现了原项目代码仓库是采取耦合的方式，在主仓库里包含了几个不同的子仓库，每个仓库都是一个独立的 `Github` 项目。这样在 `clone` 的时候必须采用`--recursive` 的递归方式。这种方法导致 `clone ` 的时候下载非常不稳定，经常断线，在经过很多尝试之后发现几乎不可能将项目完整 `clone` 成功。故我对项目所需的文件采取了单独下载再组合起来并发布的方式，便利了下载过程。由于最后项目文件过大，因此选择将压缩包发布在 `github release`，下载后放在目录解压即可。

### 2.3 项目环境设置

```sh
$ cd ~/lowrisc-nexys4ddr/build_vivado/lorisc-chip
$ source set_env.sh
```



## 3. `Vivado 2018.1` 安装

### 3.1 安装过程

下载地址：https://www.xilinx.com/member/forms/download/xef-vivado.html?filename=Xilinx_Vivado_SDK_2018.1_0405_1.tar.gz

进入下载目录，运行如下命令

```sh
$ tar xvzf Xilinx_Vivado_SDK_2018.1_0405_1.tar.gz
$ cd Xilinx_Vivado_SDK_2018.1_0405_1
$ sudo ./xsetup
```

进入安装向导后，选择 `Web Pack` 免费版本进行安装即可。

### 3.2 安装 `Xilinx Platform Cable USB` 驱动

```sh
$ cd /opt/Vivado/2018.1/data/xicom/cable_drivers/lin64/install_script/install_drivers/
$ sudo ./install_drivers
```

### 3.3 配置环境变量

```sh
$ source /opt/Xilinx/Vivado/2018.1/settings64.sh
```

可将上面的命令加入 `~/.bashrc`, 这样每次打开终端后都会自动写入环境变量



## 4. 构建 `riscv `交叉编译工具 `riscv64-unknown-linux-gnu-gcc`

### 4.1 安装依赖项

```sh
$ sudo apt-get update
$ sudo apt-get install libz-dev texinfo gawk flex bison libexpat1-dev
```



- 注：此处若按照 `github` 上官方给出的包安装会报依赖错误，最后导致重启后无法进入图形界面，可能是某些包起了冲突，最后我只能根据报错来选择上面必需的包进行安装

```sh
# 运行后系统崩溃
$ sudo apt install autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev \
                 gawk build-essential bison flex texinfo gperf libtool patchutils bc \
                 zlib1g-dev libexpat-dev git \
                 libglib2.0-dev libfdt-dev libpixman-1-dev \
                 libncurses5-dev libncursesw5-dev
```



### 4.2 下载源码

```sh
$ mkdir riscv64-linux
$ cd riscv64-linux
$ git clone https://gitee.com/mirrors/riscv-gnu-toolchain
$ git rm qemu
$ git submodule update --init --recursive
```

### 4.3 编译 `Linux`的交叉工具链

```sh
$ ./configure --prefix=/opt/lowriscv 
$ sudo make linux -j $(nproc)
```

### 4.4 导出 toolchain 的安装路径

```sh
$ export PATH="$PATH:/opt/lowriscv/bin"
```

可将上面的命令加入 `~/.bashrc`, 这样每次打开终端后都会自动写入环境变量

---

配置后输入命令出现如下输出则表示安装成功

```sh
yunhao@ubuntu:~/riscv64-linux/riscv-gnu-toolchain$ riscv64-unknown-linux-gnu-gcc -v
Using built-in specs.
COLLECT_GCC=riscv64-unknown-linux-gnu-gcc
COLLECT_LTO_WRAPPER=/opt/riscv64/libexec/gcc/riscv64-unknown-linux-gnu/12.1.0/lto-wrapper
Target: riscv64-unknown-linux-gnu
Configured with: /home/yunhao/riscv64-linux/riscv-gnu-toolchain/gcc/configure --target=riscv64-unknown-linux-gnu --prefix=/opt/riscv64 --with-sysroot=/opt/riscv64/sysroot --with-pkgversion=g --with-system-zlib --enable-shared --enable-tls --enable-languages=c,c++,fortran --disable-libmudflap --disable-libssp --disable-libquadmath --disable-libsanitizer --disable-nls --disable-bootstrap --src=/home/yunhao/riscv64-linux/riscv-gnu-toolchain/gcc --disable-multilib --with-abi=lp64d --with-arch=rv64imafdc --with-tune=rocket --with-isa-spec=2.2 'CFLAGS_FOR_TARGET=-O2   -mcmodel=medlow' 'CXXFLAGS_FOR_TARGET=-O2   -mcmodel=medlow'
Thread model: posix
Supported LTO compression algorithms: zlib
gcc version 12.1.0 (g) 
```

- 注：构建非常耗时，在我的电脑上耗费了 `3h`。并且我发现查找资料上的编译命令为 `sudo make linux -j $(nproc)`，按照该命令运行生成的为 `riscv64-unknown-linux-gnu-gcc`，然而项目中构建` riscv-tools `  实际需要的是 `riscv64-unknown-elf-gcc `。如果按照资料上的命令运行会报错，这是两个完全不同的 `gcc` 版本，需要格外注意。









## 5.  制作 `Vivado project` 文件

### 5.1 安装 `Oracle JDK`

下载 `JDK8` https://www.oracle.com/java/technologies/javase/javase8-archive-downloads.html

```sh
$ tar zxvf jdk-8u111-linux-x64.tar.gz
$ mkdir -p /usr/lib/jdk
$ mv jdk1.8.0_111  /usr/lib/jdk/jdk1.8
```

---

配置环境变量

编辑 `~/.bashrc` 文件，写入

```sh
export JAVA_HOME=/usr/lib/jdk/jdk1.8

export JRE_HOME=${JAVA_HOME}/jre

export CLASSPATH=${JAVA_HOME}/lib:${JRE_HOME}/lib

export PATH=${JAVA_HOME}/bin:$PATH
```

---

测试，出现输出则代表安装完成

```sh
$ java -version
```



### 5.2 生成 `Vivado` 项目文件

```sh
$ sudo apt-get install device-tree-compiler # 否则在生成的时候会报错
$ cd ~/lowrisc-nexys4ddr/build_vivado/lorisc-chip/fpga/board/nexys4_ddr
$ make project
```

- 注：在生成的时候报错：

```sh
[error] (Thread-13) java.io.IOException: Cannot run program "dtc": error=2, No such file or directory
```

查询资料后，运行 `sudo apt-get install device-tree-compiler` 解决

## 6. 将 `riscv` 内核烧录至 `FPGA`

将 FPGA 的 mini USB 电源线连接到电脑上并打开FPGA的开关，在 `VMware` 上选取连接到虚拟机 ( `Ubuntu 16.04 LTS` )，运行以下命令将内核烧入至 FPGA 的 `Flash` 内存中。

```sh
$ cd ~/lowrisc-nexys4ddr/build_vivado/lorisc-chip/fpga/board/nexys4_ddr
$ make program-cfgmem
```

这样做的好处是 `FLash` 为非易失型内存，这样每次上电后 FPGA 都会自动读取 `Flash` 中的内核，避免了重复写入的麻烦。

- 注：选用这个内核项目的初衷就是因为疫情被封在家里无法出门，也没办法网购。而我的电脑只有一个 `USB` 接口，因此无法在烧入硬件的同时使用老师发的 `ARM`仿真器进行编程。因此想到了这个办法。



## 7. 在内核源码中添加 `LED` 驱动文件

### 7.1 关于驱动的基本概念：

驱动程序完全隐藏了设备工作的细节. 用户的活动通过一套标准化的调用来进行,这些调用与特别的驱动是独立的; 设备驱动的角色就是将这些调用映射到作用于实际硬件的和设备相关的操作上.

驱动应当做到使硬件可用, 将所有关于如何使用硬件的事情留给应用程序. 一个驱动,如果它提供了对硬件能力的存取, 没有增加约束,就是灵活的.

`Linux` 下的驱动是使用 `C` 语言进行开发的，但是和我们平常写的 ` C ` 语言也有不同，因为我们平常写的C语言使用的是 ` Libc ` 库，但是驱动是跑在内核中的程序，内核中却不存在 `libc` 库，所以要使用内核中的库函数.

Linux系统分为**内核态**和**用户态**，只有在内核态才能访问到硬件设备，而驱动可以算是内核态中提供出的API，供用户态的代码访问到硬件设备。

(1)每个内核模块只注册自己以便来服务将来的请求, 并且它的初始化函数立刻终止. 换句话说, 模块初始化函数的任务是为以后调用模块的函数做准备; 好像是模块说, ” 我在这里, 这是我能做的.”模块的退出函数(例子里是 hello_exit)就在模块被卸载时调用. 它好像告诉内核, “我不再在那里了, 不要要求我做任何事了.” 每个内核模块都是这种类似于事件驱动的编程方法, 但不是所有的应用程序都是事件驱动的.

(2)事件驱动的应用程序和内核代码的退出函数不同: 一个终止的应用程序可以在释放资源方面”懒惰”, 或者完全不做清理工作, 但是模块的退出函数必须小心恢复每个由初始化函数建立的东西, 否则会保留一些东西直到系统重启.

`insmod` 命令用于将给定的模块加载到内核中。`Linux` 有许多功能是通过模块的方式，在需要时才载入 `kernel`。如此可使 `kernel` 较为精简，进而提高效率，以及保有较大的弹性。这类可载入的模块，通常是设备驱动程序。例如：`insmod xxx.ko`

`rmmod` 命令用于从当前运行的内核中移除指定的内核模块。执行 `rmmod` 指令，可删除不需要的模块。`Linux` 操作系统的核心具有模块化的特性，应此在编译核心时，务须把全部的功能都放入核心。你可以将这些功能编译成一个个单独的模块，待有需要时再分别载入它们。例如：`rmmod xxx.ko`

(3) 一个模块在内核空间运行, 而应用程序在用户空间运行. 内核空间和用户空间特权级别不同,而且每个模式有它自己的内存映射–它自己的地址空间.

(4) 内核编程与传统应用程序编程方式很大不同的是并发问题. 大部分应用程序, 多线程的应用程序是一个明显的例外, 典型地是顺序运行的, 从头至尾, 不必要担心其他事情会发生而改变它们的环境. 内核代码没有运行在这样的简单世界中, 即便最简单的内核模块必须在这样的概念下编写, “很多事情可能马上发生”.

### 7.2 在 `Linux` 中添加自己编写的驱动的几种方法

1. **在文件系统中通过 `io` 命令点亮led**

```sh
io -4 0xff780004  0x1000000 (设置gpio1_d0方向为输出)
io -4 0xff780000  0x1000000 （设置gpio1_d0输出高电平）
```

2.  **编写内核驱动，通过操作寄存器，点亮LED。**

```sh
#include <linux/module.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/ioport.h>

//GPIO1 base address according to rk3288 datasheet
#define GPIO1_BASE 0xff780000

// GPIO register map define
struct gpio {
	u32 data; //gpio data register
	u32 direction;//gpio direction register
} *gpio1;

//led module init。
int __init led_init(void)
{
	//remap the gpio1 physic address to virtual address
	gpio1 = (struct gpio *)ioremap(GPIO1_BASE, 8);

	//write 1 at bit24 to set gpio1_b0 direction as output
	writel(1 << 24, &gpio1->direction);
	//write 1 at bit24 to set gpio1_b0 as high
	writel(1 << 24, &gpio1->data);

	return 0;
}
//led module exit. 
void __exit led_exit(void)
{
	//Cancel gpio1 remap
	iounmap(gpio1);
}

module_init(led_init);
module_exit(led_exit);

MODULE_AUTHOR("Eric Gao");
MODULE_LICENSE("GPL");
```

3. **在文件系统中，通过内核提供的接口操作GPIO**

```sh
$ cd /sys/class/gpio/
$ echo 56 > export              # (将标号为56的GPIO导出到文件系统)
$ echo out > gpio56/direction   # (设置gpio方向为输出)
$ echo 1 > gpio56/value         # (使该GPIO输出高电平)
```

4. **通过用户态程序结合内核GPIO接口实现LED闪烁**

```c
// led.c:

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
  system("echo 56 > /sys/class/gpio/unexport");
  system("echo 56 > /sys/class/gpio/export");
  system("echo 'out' > /sys/class/gpio/gpio56/direction");
  while (1) {
    system("echo 0 > /sys/class/gpio/gpio56/value");
    usleep(500000);
    system("echo 1 > /sys/class/gpio/gpio56/value");
    usleep(500000);
  }
  system("echo 56 > /sys/class/gpio/unexport");

  return 0;
}
```

经过很多尝试后，我发现由于方法1, 3, 4需要在进入FPGA后进行操作，然而 `lowrsic Linux` 操作系统默认没有包含`io`, `insmod`, `rmmod` 等很多驱动常见的命令，甚至缺少`/sys/class/gpio` 目录，导致上述方法无法实现。而当我执行命令 `sudo apt-get install kmod` 以安装上述命令后又出现了系统崩溃的现象，导致重启无法进入系统，这个 `bug` 目前还没有解决，因此我采用了方法2：把驱动源码加到内核里然后直接编译内核源代码



### 7.3 编写 `Linux` 驱动

一般我们会把需要的功能编译到内核中，但是这样会带来一个问题，那就是内核会越来越庞大，对功能经行修改时就需要重新编译内核。这对于一些只在特定场合才会使用的功能来说就很不方便，不过 `Linux` 提供了一种机制--模块（module）。模块本身不会被编译到内核中，从而控制了内核的大小，当需要时就可以被动态的加载到内核中去。一旦模块被加载到内核中，那么它就和内核中的其他部分完全一样了。下面是一个简单的内核模块源码：

```c
// hello.c

#include <linux/init.h>
#include <linux/module.h>

/*
模块加载函数
模块加载函数在模块加载的时候运行，一般以__init声明，以module_init(函数名)的形式被指定。如果初始化成功就会返回0，如果初始化失败就会返回错误编码。需要注意的是在内核模块中用于输出的函数是内核空间的printk而不是用户空间的printf，KERN_INFO为printk的打印级别。
*/

static int __init hello_init(void)
{
    printk(KERN_INFO "Hello World enter\n");
    return 0;
}
module_init(hello_init);


/*
模块卸载函数
模块卸载函数在模块卸载的时候运行，一般以__exit声明，以module_exit(函数名)的形式被指定。模块卸载函数不返回任何值，一般是完成与模块加载函数相反的功能。
*/

static void __exit hello_exit(void)
{
    printk(KERN_INFO "Hello World exit\n");
}
module_exit(hello_exit);

// 模块许可声明
MODULE_AUTHOR("YouZi");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("A simple module named Hello World");
MODULE_ALIAS("A simple module");
```

---

lowRISC 默认打开了 `GPIO0`，控制 `LED` 的地址在偏移量为 `15` 的地址出。针对此编写了LED驱动代码:

```c
#include <linux/module.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/ioport.h>

// 宏定义
#define GPIO0_BASE 0xffffffd004040000
#define LED 15
uint64_t *led_base;

// 初始化驱动模块
int __init led_init(void)
{
	// 将物理地址映射到可以驱动代码访问的虚拟地址
	led_base = (uint64_t *)ioremap(GPIO0_BASE + LED * sizeof(uint64_t), 8);
	
    // set *led_base = 0x200202
	// write 1 at bit 10 to open LED
    writel(1 << 9, led_base);
    printk("LED driver enabled")
	return 0;
}

// 退出驱动模块
void __exit led_exit(void)
{
	// 取消地址映射
	iounmap(led_base);
}

module_init(led_init);
module_exit(led_exit);

// 模块注册信息
MODULE_AUTHOR("Yunhao Liu");
MODULE_LICENSE("GPL");
```

### 7.4 将驱动源码编入内核

1. 首先在驱动源码的同级目录创建 `led.c`, `Makefile`, `Kconfig` 三个文件，`led.c` 为驱动程序源码，`Kconfig` 为源码编译时的配置文件

```sh
$ cd ~/lowrisc-nexys4ddr/build_vivado/lowrisc-chip/riscv-linux/drivers/mmc'
$ mkdir led
$ cd led
$ touch led.c Makefile Kconfig
```

```sh
# Kconfig
# SPDX-License-Identifier: GPL-2.0
# Yunhao's LED driver configuration  

# menuconfig对应的是一个目录是否被使能，这里即我们添加的led
menuconfig LED_DEVICE  
     # 这个字符串是在make menuconfig中的说明，见上图所见
	bool "LED device support"                  
	default n  # 默认不使能

if LED_DEVICE # 如果该文件所处的目录被使能，则进入
 config LED # 该宏决定led.c是否被编译
 bool "LED driver" # 此字符串为make menuconfig中的说明，见下图
 default n # 默认不使能
endif

```

```makefile
# Makefile
# SPDX-License-Identifier: GPL-2.0
# LED drivers configuration
obj-$(CONFIG_LED) 		+= led.o # 这个宏便是上诉操作的目录
```

---

2. 在新添加的 `led` 目录的同级目录中，对 `Kconfig` 和 `Makefile` 做修改. 分别添加 `source "drivers/mmc/led/Kconfig"`  和 `obj-$(CONFIG_LED_DEVICE) += led/`

---

3. 编辑 `arch/$(SRCARCH)/Kconfig` 文件，它是整个Linux系统编译时用到的 `Kconfig` 文件。添加 `CONFIG_LED_DEVICE=y` 和 `CONFIG_LED=y`

注：上诉的关键操作就是配置了`LED_DEVICE`和 `LED` 这两个宏为 `y`，`LED_DEVICE` 对应的就是 `led/` 目录是否使能，`LED` 对应的就是 `led/` 目录下的驱动文件 `led.c` 是否使能





## 8. 编译 `Linux` 内核 && 制作系统镜像



### 8.1 生成 verilog 

```sh
$ cd ~/lowrisc-nexys4ddr/build_vivado/lorisc-chip/fpga/board/nexys4_ddr
$ make verilog
```

可能会遇到的问题：

```sh
/bin/bash: line 2: /home/yunhao/lowrisc-nexys4ddr/build_vivado/lowrisc-chip/rocket-chip/scripts/vlsi_mem_gen: Permission denied
/home/yunhao/lowrisc-nexys4ddr/build_vivado/lowrisc-chip/rocket-chip/vsim/Makefrag-verilog:20: recipe for target '/home/yunhao/lowrisc-nexys4ddr/build_vivado/lowrisc-chip/rocket-chip/vsim/generated-src/freechips.rocketchip.system.DefaultConfig.behav_srams.v' failed
```

解决办法：

```sh
# 赋予文件执行权限
$ chmod +x /home/yunhao/lowrisc-nexys4ddr/build_vivado/lowrisc-chip/rocket-chip/scripts/vlsi_mem_gen
```



### 8.2 编译内核并制作镜像

```sh
$ cd ~/lowrisc-nexys4ddr/build_vivado/lowrisc-chip/riscv-linux
$ if [ -f .config ]; then echo configured; else make ARCH=riscv defconfig; fi
# 这一步生成vmlinux
$ make ARCH=riscv -j 4 CROSS_COMPILE=riscv64-unknown-linux-gnu- CONFIG_INITRAMFS_SOURCE="initramfs.cpio"
$ mkdir -p ../rocket-chip/riscv-tools/riscv-pk/build
$ cd ../rocket-chip/riscv-tools/riscv-pk/build
$ ../configure --prefix=$RISCV --host=riscv64-unknown-elf --with-payload=../../../../riscv-linux/vmlinux --enable-logo
$ cd ~/lowrisc-nexys4ddr/build_vivado/lowrisc-chip/riscv-linux
$ scripts/dtc/dtc ../rocket-chip/riscv-tools/riscv-pk/machine/lowrisc.dts -O dtb -o ../rocket-chip/riscv-tools/riscv-pk/build/lowrisc.dtb
$ rm -f ../rocket-chip/riscv-tools/riscv-pk/build/payload.o
$ make -C ../rocket-chip/riscv-tools/riscv-pk/build
$ cp -p ../rocket-chip/riscv-tools/riscv-pk/build/bbl ../fpga/board/nexys4_ddr/boot.bin
$ riscv64-unknown-elf-strip ../fpga/board/nexys4_ddr/boot.bin
$ cp ../fpga/board/nexys4_ddr/boot.bin ~/lowrisc-nexys4ddr/
```



可能会遇到的问题：

```sh
scripts/extract-cert.c:21:25: fatal error: openssl/bio.h: No such file or directory
compilation terminated.
scripts/Makefile.host:90: recipe for target 'scripts/extract-cert' failed
make[1]: *** [scripts/extract-cert] Error 1
Makefile:1045: recipe for target 'scripts' failed
make: *** [scripts] Error 2
```

解决办法：

```sh
sudo apt-get install libssl-dev
```











## 9. 制作 `SD` 卡

使用读卡器将 `SD`卡连接至电脑，在 `VMware` 上选取连接到虚拟机 ( `Ubuntu 16.04 LTS` )。

在设备上识别出 `SD` 卡的编号 (eg: `sdb`)

```sh
$ lsblk
```

---

进入项目目录，格式化 `SD` 卡并分区

```sh
$ cd ~/lowrisc-nexys4ddr
$ make USB=sdb cleandisk partition
```

---

将构建的基于 `Debian Linux` 的 `Lowrisc Linux` 系统写入SD卡

```sh
$ make USB=sdb mkfs fatdisk extdisk
```



## 10. 在 `FPGA` 上启动系统

### 10.1 设置硬件

1. 将 `Micro SD` 插入 FPGA。

2. 将 FPGA 的 mini USB 电源线连接到电脑上。

3. (可选) 将网线插入 FPGA 的网口，这样FPGA上的系统启动后会自动连接以太网。

4. 在宿主机 (`Windows 10`) 上打开 `Xshell`，新建一个项目，选取 `Serial` 连接，输入FPGA连接的端口，这个端口可以在设备管理器上看到 (eg: `COM4`)。运行项目建立宿主机与 FPGA 的串口通信。

5. 打开FPGA的开关，`Xshell` 终端上出现一系列的输出，进入`Linux` 系统，说明项目构建成功。



### 10.2 体验 `lowRISC Linux`

进入系统后，会提示设置 `root` 密码和普通用户密码。`lowRISC Linux` 基于 `Debian Linux` 的一个发行板，运行在架构为 `RISCV` 的处理器上。其内置了 `gcc`，`python2` 等一些实用的工具，可以在 FPGA 上完成一些任务。

例：在 `FPGA` 上编写并编译运行 `hello world` 程序

```c
// hello.c
#include<stdio.h>
int main()
{
    printf("Hello World\n");
}
```

```sh
$ gcc hello.c -o hello
$ ./hello
Hello World
```

---

例：在 `FPGA` 上测试 `python` 程序

```sh
$ python
>>> print('Hello World')
>>> Hello World
>>> import this
The Zen of Python
...
```





### 10.3 通过 `apt` 命令获取包 

`lowRISC Linux` 编写了网络驱动，因此在连接网线后可以访问互联网。但在运行 `sudo apt-get install` 命令安装包后会报错。这是因为系统构建的时间较早，有很多包的地址已经更新，因此需要先更新源。在更新源的时候我又遇到了密钥缺失的问题，在查找很多资料之后，找到了解决方案。

```sh
$ wget --no-check-certificate https:www.ports.debian.org/archive_2022.key
$ cat archive_2022.key | sudo apt-key add
$ sudo apt-get update
```

进行如上设置后，便可以通过 `apt` 命令获取包 

例：安装 `vim`

```sh
$ sudo apt-get install vim
```

- 注：进行网络设置的初衷是因为 `Linux` Live 驱动要用 `insmod` 命令加载，但是这个系统初始并没有内置 `insmod` 命令。需要 `sudo apt-get install kmod` 安装。但是在设置网络并安装`kmod`后 FPGA 上的系统发生了崩溃，重启后无法以 `root`或普通用户身份进入系统，这个`bug` 目前还没有找到解决方法。正因为如此想到了直接把驱动源码加到内核里然后直接编译内核源代码的办法。

 



## 附录

### `lowRISC CPU `架构

```sh
Interrupt map (2 harts 2 interrupts):
  [1, 2] => ExampleRocketSystem

/dts-v1/;

/ {
	#address-cells = <1>;
	#size-cells = <1>;
	compatible = "freechips,rocketchip-unknown-dev";
	model = "freechips,rocketchip-unknown";
	L13: cpus {
		#address-cells = <1>;
		#size-cells = <0>;
		L5: cpu@0 {
			clock-frequency = <0>;
			compatible = "sifive,rocket0", "riscv";
			d-cache-block-size = <64>;
			d-cache-sets = <64>;
			d-cache-size = <16384>;
			d-tlb-sets = <1>;
			d-tlb-size = <32>;
			device_type = "cpu";
			i-cache-block-size = <64>;
			i-cache-sets = <64>;
			i-cache-size = <16384>;
			i-tlb-sets = <1>;
			i-tlb-size = <32>;
			mmu-type = "riscv,sv39";
			next-level-cache = <&L10 &L7>;
			reg = <0>;
			riscv,isa = "rv64imafdc";
			status = "okay";
			timebase-frequency = <1000000>;
			tlb-split;
			L3: interrupt-controller {
				#interrupt-cells = <1>;
				compatible = "riscv,cpu-intc";
				interrupt-controller;
			};
		};
	};
	L7: memory@80000000 {
		device_type = "memory";
		reg = <0x80000000 0x10000000>;
	};
	L12: soc {
		#address-cells = <1>;
		#size-cells = <1>;
		compatible = "freechips,rocketchip-unknown-soc", "simple-bus";
		ranges;
		L1: clint@2000000 {
			compatible = "riscv,clint0";
			interrupts-extended = <&L3 3 &L3 7>;
			reg = <0x2000000 0x10000>;
			reg-names = "control";
		};
		L2: debug-controller@0 {
			compatible = "sifive,debug-013", "riscv,debug-013";
			interrupts-extended = <&L3 65535>;
			reg = <0x0 0x1000>;
			reg-names = "control";
		};
		L10: error-device@3000 {
			compatible = "sifive,error0";
			reg = <0x3000 0x1000>;
			reg-names = "mem";
		};
		L6: external-interrupts {
			interrupt-parent = <&L0>;
			interrupts = <1 2>;
		};
		L0: interrupt-controller@c000000 {
			#interrupt-cells = <1>;
			compatible = "riscv,plic0";
			interrupt-controller;
			interrupts-extended = <&L3 11 &L3 9>;
			reg = <0xc000000 0x4000000>;
			reg-names = "control";
			riscv,max-priority = <7>;
			riscv,ndev = <2>;
		};
		L8: mmio@60000000 {
			#address-cells = <1>;
			#size-cells = <1>;
			compatible = "simple-bus";
			ranges = <0x60000000 0x60000000 0x20000000>;
		};
		L9: rom@10000 {
			compatible = "sifive,rom0";
			reg = <0x10000 0x10000>;
			reg-names = "mem";
		};
	};
};

Generated Address Map
	       0 -     1000 ARWX  debug-controller@0
	    3000 -     4000 ARWX  error-device@3000
	   10000 -    20000  R XC rom@10000
	 2000000 -  2010000 ARW   clint@2000000
	 c000000 - 10000000 ARW   interrupt-controller@c000000
	60000000 - 80000000  RWX  mmio@60000000
	80000000 - 90000000  RWXC memory@80000000
```



## 参考文献

1. [Build your own RISC-V architecture on FPGA](http://modernhackers.com/build-your-own-risc-v-architecture-on-fpga/)

2. [lowRISC Documentation](https://lowrisc.org/docs/)

3. [lowrisc-chip](https://github.com/lowRISC/lowrisc-chip)
4. [lowrisc-quickstart-modernhackers.com](https://github.com/mazsola2k/lowrisc-quickstart-modernhackers.com)
5. [Getting started with binaries](https://www.cl.cam.ac.uk/~jrrk2/docs/getting-started-binaries/)
6. [[Build your own bitstream and images](https://www.cl.cam.ac.uk/~jrrk2/docs/ethernet-v0.5/development/)
7. [riscv-gnu-toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain)
8. [Linux驱动篇(一)——驱动相关概念](https://zhuanlan.zhihu.com/p/126348891)
9. [Linux驱动篇(二)——动态加载模块](https://zhuanlan.zhihu.com/p/129021420)
10. [Linux下的Vivado安装——以Ubuntu为例](https://www.bilibili.com/read/cv5622114/)
11. [在 QEMU 上运行 RISC-V 64 位版本的 Linux](https://zhuanlan.zhihu.com/p/258394849)
12. [交叉编译linux内核实例（最详细）总结](https://blog.csdn.net/Luckiers/article/details/124531266)
13. [【linux】编译安装内核、编译busybox文件系统](https://cactusii.github.io/post/linux-geng-huan-nei-he-ban-ben/)
14. [图解嵌入式系统开发之驱动篇：LED驱动](https://zhuanlan.zhihu.com/p/27242098)



