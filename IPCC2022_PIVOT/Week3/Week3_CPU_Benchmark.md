# Week3 CPU Benchmark

***注：服务器网络限制无法使用apt等，安装过程应大致相仿。以本地作训练机，配置如下：***

```bash
Linux version 5.15.90.1-microsoft-standard-WSL2 (oe-user@oe-host) (x86_64-msft-linux-gcc (GCC) 9.3.0, GNU ld (GNU Binutils) 2.34.0.20200220)
CPU: 13th Gen Intel(R) Core(TM) i5-13600KF core 14 6-8 20 threads
cpu cores : 10
```



## 1. HPCG Benchmark



**HPCG** 前置所需环境：

`gcc g++ gfortran mpich`						**（server: g++ install& apt net**



### 1.1 GCC&&G++

```bash
# install
$ sudo apt update
$ sudo apt install build-essential

# cat install location && version
$ which gcc
$ which g++
$ gcc -v
$ g++ -v
```



### 1.2 GFORTRAN

```bash
# install
$ sudo yum install gcc-gfortran

# cat install location
$ which gfortran
$ gfortran -v
```

***p.s. 可能还有环境配置 .bashrc source 啥的 有问题记得加***



### 1.3 MPICH

```bash
# 1.下载依赖包并解压 4.1.1为版本号，具体而定
$ wget http://www.mpich.org/static/downloads/3.2.1/mpich-4.1.1.tar.gz
$ tar xzf mpich-4.1.1.tar.gz && cd mpich-4.1.1
# 2.修改配置环境 -prefix确定安装目录
$ ./configure -prefix=/usr/local/mpich-4.1.1
# 3.安装
$ make && make install
# 4.cat install location
$ which mpicc && which mpicxx && which mpiexec
# 5.example test in one host
```

![image-20230319232101652](png\image-20230319232101652.png)



### 1.4 HPCG

```bash
git clone https://github.com/hpcg-benchmark/hpcg.git
```

```bash
# DIR:	hpcg-master/setup		Make.Linux_MPI
# 修改配置文件,所需修改内容如下：
TOPdir       = /home/root/myProjects/HPCGBenchmarkTest/hpcg-master
SRCdir       = $(TOPdir)/src
INCdir       = $(TOPdir)/src
BINdir       = $(TOPdir)/bin
CXX          = /usr/local/mpich-4.1.1/bin/mpicxx
```

```bash
# top-dir 设置安装配置环境
/home/root/myProjects/HPCGBenchmarkTest/hpcg-master/configure Linux_MPI && make
```

```bash
root@NAGARA:/home/root/myProjects/HPCGBenchmarkTest/hpcg-master/setup/build/bin# ls
HPCG-Benchmark_3.1_2023-03-19_16-26-28.txt  hpcg.dat  hpcg20230319T155602.txt  xhpcg
root@NAGARA:/home/root/myProjects/HPCGBenchmarkTest/hpcg-master/setup/build/bin# pwd
/home/root/myProjects/HPCGBenchmarkTest/hpcg-master/setup/build/bin
root@NAGARA:/home/root/myProjects/HPCGBenchmarkTest/hpcg-master/setup/build/bin# mpirun -np 8 ./xhpcg
cat HPCG-Benchmark_3.1_2023-03-19_16-26-28.txt

Final Summary=
Final Summary::HPCG result is VALID with a GFLOP/s rating of=8.8171
Final Summary::HPCG 2.4 rating for historical reasons is=8.93651
Final Summary::Reference version of ComputeDotProduct used=Performance results are most likely suboptimal
Final Summary::Reference version of ComputeSPMV used=Performance results are most likely suboptimal
Final Summary::Reference version of ComputeMG used=Performance results are most likely suboptimal
Final Summary::Reference version of ComputeWAXPBY used=Performance results are most likely suboptimal
Final Summary::Results are valid but execution time (sec) is=1776.48
Final Summary::Official results execution time (sec) must be at least=1800
```

服务器按照这个步骤做sbatch任务就行。



## 2. HPL Benchmark



**HPL** 主要前置所需环境：

` GCC/GFORTRAN(1.1&1.2)	MPICH(1.3)	GOTOBLAS	HPL`



### 2.1 GOTOBLAS

```bash
# 1.下载依赖包并解压 1.13为版本号，具体而定
$ wget https://www.tacc.utexas.edu/documents/1084364/1087496/GotoBLAS21.13.tar.gz
$ tar xzf GotoBLAS21.13.tar.gz && cd GotoBLAS2
# 2.make安装
$ make CC=gcc BINARY=64 TARGET=NEHALEM
# 若出现：
# ../libgoto2_nehalem-r1.13.a(ssymv_U.o): error adding symbols: 错误的值
# collect2: error: ld returned 1 exit status
# 则执行：
# $ gedit f_check
# 298 行改为如下：
# print MAKEFILE "FEXTRALIB=$linker_L -lgfortran -lm -lquadmath -lm $linker_a\n";
# make clean
# make CC=gcc BINARY=64 TARGET=NEHALEM USE_OPENMP=1 INTERFACE64=1
```

若出现：

```
GotoBLAS build complete.

    OS               ... Linux             
    Architecture     ... x86_64               
    BINARY           ... 64bit                 
    C compiler       ... GCC  (command line : gcc)
    Fortran compiler ... G77  (command line : g77)
    Library Name     ... libgoto2_nehalem-r1.13.a (Single threaded)  
```

则安装成功



### 2.2 HPL

```bash
# 1.下载依赖包并解压 1.13为版本号，具体而定
$ wget https://netlib.org/benchmark/hpl/hpl-2.3.tar.gz
$ tar xzf hpl-2.3.tar.gz && cd hpl-2.3
# 2.把/hpl-2.3/setup/Make.Linux_PII_FBLAS cp 至上一层目录并且命名为 Make.my
# 3. 修改Make.my 配置文件， 在此之前先确认所需的相关环境路径
#
# hpl dir: 
# root@NAGARA:/home/root/myProjects/HPLEnv/hpl-2.3# pwd
# /home/root/myProjects/HPLEnv/hpl-2.3
#
# mpich *install* dir:
# /usr/local/mpich-4.1.1
#
# gotoblas2 dir:
# root@NAGARA:/home/root/myProjects/LASEnv/GotoBLAS2# pwd
# /home/root/myProjects/LASEnv/GotoBLAS2
# Make.my相关的修改项如下：

Make.my
TOPdir		 = /home/root/myProjects/HPLEnv/hpl-2.3	# hpl解压包所在目录
MPdir		 = /usr/local/mpich-4.1.1	# mpich***安装包***所在目录
MPlib		 = $(MPdir)/lib/libmpi.so
LAdir        = /home/root/myProjects/LASEnv/GotoBLAS2	# gotoblas2解压包所在目录
LAinc        =
LAlib        = $(LAdir)/libgoto2_nehalemp-r1.13.a
HPL_OPTS     = -DHPL_CALL_CBLAS
CC           = /usr/local/mpich-4.1.1/bin/mpicc
CCFLAGS      = $(HPL_DEFS) -fomit-frame-pointer -fopenmp -O3 -funroll-loops
LINKER       = /usr/local/mpich-4.1.1/bin/mpif77
LINKFLAGS 	 = $(CCFLAGS)
# 4.编译安装
$ make arch=my
#若出现报错，检查相关路径是否正确
# 5.benchmark
$ mpirun -np 2 ./xhpl
$ cd bin/my/
$ cat HPL-Benchmark.txt
```

![image-20230319234605563](png\image-20230319234605563.png)





1. AIStation创建开发环境 记得设置端口

2. ssh 登录环境 下载数据模型
3. 进行benchmark



鲲鹏 920 处理器为 ARM CPU 芯片，

其主要特点： 

⚫ 最大可支持 64cores，2.6GHz，可支持多种核数量和频率的型号搭配。 

⚫ 兼容适配 ARMv8-A 架构特性，支持 ARMv8.1 和 ARMv8.2 扩展。 

⚫ Core 为 64bits-TaiShan core 核。 

⚫ 每个 core 集成 64KB L1 ICache，64KB L1 Dcache 和 512KB L2 Dcache。 

⚫ 支持最大 64MB 的 L3 Cache 容量。 

⚫ 支持超标量，可变长度，乱序流水线。 

⚫ 支持 ECC 1bit 纠错，ECC 2bit 报错。 

⚫ 支持片间 Hydra 高速接口，通道速率高达 30Gbps。 

⚫ 支持 8 个 DDR 控制器。 

⚫ 最大支持 8 个物理以太网口。 

⚫ 支持 3 个 PCIe 控制器，支持 GEN4(16Gbps)，并可向下兼容。









```

```

