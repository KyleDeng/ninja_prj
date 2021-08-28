# ninja使用记录

[TOC]

---

## 0. 安装
Ubuntu 20.04 TLS 安装：
```shell
sudo apt-get install ninja-build
```

源码编译安装：
```shell
git https://github.com/ninja-build/ninja.git
cd ninja
./configure.py --bootstrap
cp ninja /usr/bin/
```

## 1. 简单例子
### 1.1 单一文件编译
目录结构：
```log
./
├── build.ninja
└── main.c
```

`main.c`
```c
#include <stdio.h>

int main(int argc, char* argv[])
{
    printf("Hello World!\n");

    for(int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    return 0;
}
```

`build.ninja`
```ninja
ninja_required_version = 1.3

cc = gcc
cflags = -Wall

rule Object
    command = $cc $cflags -c $in -o $out
    description = compile .c

rule Program
    command = $cc -o $out $in
    description = out bin

build main.o: Object main.c
build main: Program main.o
```

执行命令：
```log
ninja
[2/2] out bin
```

会生成产物：
```log
.
├── build.ninja
├── main
├── main.c
└── main.o
```

`ninja`默认的解析文件是`build.ninja`，如需另外指定：`ninja -f xxx.ninja`。

也可以指定指定目标：`ninja main.o`

清理产物命令：`ninja -t clean`。

## 1.2 多文件编译
目录结构：
```log
.
├── build.ninja
├── hello.c
├── hello.h
└── main.c
```

`hello.c`
```c
#include <stdio.h>
#include "hello.h"

int main(int argc, char* argv[])
{
    const char* name = "Bob";
    hello(name);

    printf("------main------\n");

    for(int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    return 0;
}
```

`hello.h`
```c
#ifndef __HELLO_H__
#define __HELLO_H__

int hello(const char* name);

#endif
```

`main.c`
```c
#include <stdio.h>
#include "hello.h"

int main(int argc, char* argv[])
{
    const char* name = "Bob";
    hello(name);

    printf("------main------\n");

    for(int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    return 0;
}
```

`build.ninja`
```ninja
ninja_required_version = 1.3

cc = gcc
cflags = -Wall

# 池的概念
# 当一组rule和edge，通过指定其depth，
# 就可控制这组rule和edge并行上限。
pool object
    depth = 2

# 最多允许2个Object并行
rule Object
    command = $cc $cflags -c $in -o $out
    description = $cc $cflags -c $in -o $out
    pool = object

rule Program
    command = $cc -o $out $in
    description = $cc -o $out $in

build main: Program main.o hello.o
build main.o: Object main.c
build hello.o: Object hello.c


# 指定默认目标
default main
```

## 1.3 编译和使用库
没有什么现成的好方法，生硬的写规则。

`build.ninja`
```ninja
ninja_required_version = 1.3

CC = gcc
AR = ar
CFLAGS = -Wall

pool object
    depth = 2

rule Object
    command = $CC $CFLAGS -c $in -o $out
    description = $CC $CFLAGS -c $in -o $out
    pool = object

rule Library
    command = ${AR} rcs ${out} ${in}

rule ProgramMain
    command = ${CC} -o ${out} main.o -L. -lhello

build hello.o: Object hello.c
build libhello.a: Library hello.o
build main.o: Object main.c

build main: ProgramMain main.o libhello.a


default main
```

## 1.4 多级目录处理
目录结构如下：
```log
.
├── build.ninja
├── code
│   ├── goodbye
│   │   ├── goodbye.c
│   │   ├── goodbye.h
│   │   └── goodbye.ninja
│   └── hello
│       ├── hello.c
│       ├── hello.h
│       └── hello.ninja
└── main.c
```

`build.ninja`
```ninja
ninja_required_version = 1.3

CC = gcc
AR = ar
CFLAGS = -Wall

builddir = output

# 一些全局变量
INC_DIRS =
OBJ_LIST =

pool object
    depth = 4

rule Debug
    command = echo debug: ${in}

rule Object
    command = $CC $CFLAGS -c $in ${INC_DIRS} -o $out
    description = $CC $CFLAGS -c $in ${INC_DIRS} -o $out
    pool = object

rule Library
    command = ${AR} rcs ${out} ${OBJ_LIST}

rule ProgramMain
    command = ${CC} -o ${out} ${builddir}/main.o -L${builddir} -lcode

include ./code/hello/hello.ninja
include ./code/goodbye/goodbye.ninja
build ${builddir}/libcode.a: Library
build code: phony ${builddir}/libcode.a
# 不能用以下语法，ninja会把整个${OBJ_LIST}，当成一个依赖
# build ${builddir}/libcode.a: Library ${OBJ_LIST}

build ${builddir}/main.o: Object main.c
build main: phony ${builddir}/main.o

build ${builddir}/out: ProgramMain
build out: phony ${builddir}/out


# 设置默认目标
default hello goodbye main code
```

`code/hello/hello.ninja`
```ninja
LOCAL_PATH = ./code/hello

build ${builddir}/hello.o: Object ${LOCAL_PATH}/hello.c

build hello: phony ${builddir}/hello.o

INC_DIRS = ${INC_DIRS} -I${LOCAL_PATH}
OBJ_LIST = ${OBJ_LIST} ${builddir}/hello.o
```

`code/goodbye/goodbye.ninja`
```ninja
LOCAL_PATH = ./code/goodbye

build ${builddir}/goodbye.o: Object ${LOCAL_PATH}/goodbye.c

build goodbye: phony ${builddir}/goodbye.o

INC_DIRS = ${INC_DIRS} -I./code/goodbye
OBJ_LIST = ${OBJ_LIST} ${builddir}/goodbye.o
```

## 2. `ninja`语法
### 2.1 概念
| 概念 | 翻译 | 解释 |
| ---- | -----| ---- |
| edge | 边 | 即build语句，指定目标、规则与输入，时编译过程中拓扑图中的一条边 |
| target | 目标 | 编译过程需要产生的目标，有build语句指定 |
| output | 输出 | build语句的前半段，也是target的另一种称呼 |
| input | 输入 | build语句的后半段，用来产生output文件，另一种称呼是依赖 |
| rule | 规则 | 通过指定command与一些内置变量，决定如何从输入产生输出 |
| pool | 池 | 一组rule或edge，通过指定其depth，可以控制并行的上限 |
| scope | 作用域 | 变量的作用范围，有rule与build语句的块级，也有文件级别 |

### 2.2 关键字
| 关键字 | 作用 |
| ------ | ---- |
| build | 定义一个edge |
| rule | 定义一个rule |
| pool | 定义pool |
| default | 指定默认的一个或多个target |
| include | 添加一个ninja文件到当前scope |
| subninja | 添加一个ninja文件，拥有独立的scope，但是继承主ninja |
| phony | 一个内置的特殊规则，指定非文件的target，其实就是重命名一个target |

### 2.3 变量
变量有两种，**内置变量**和**自定义变量**，二者都可以通过`var = str`的方式来定义，
通过`$var`或`${var}`的方式引用，
变量只用一种类型，就是字符串。

顶层的内置变量有两个：
| 内置变量 | 作用 |
| -------- | ---- |
| builder | 指定一些文件的输出目录，如`.ninja_log`、`.ninja_deps`等 |
| `ninja_required_version` | 指定ninja命令的最低版本 |

### 2.4 rule
```ninja
rule name
    command = echo ${in} > ${out}
    var = str
```

通常一个rule就是通过`${in}`生成`${out}`的过程。

除了`command`以外，`rule`还支持以下变量：
| 内置变量 | 作用 |
| -------- | ---- |
| command | 定义一个规则的执行命令，也说明了必要的变量 |
| description | command的说明，会代替command在无`-v`时打印 |
| generator | 指定后，这条rule生成的文件不会被默认清理 |
| in | 空格分割的input列表 |
| `in_newline` | 换行符分割的input列表 |
| out | 空格分割的output列表 |
| depfile | 指定一个Makefile文件作为额外的显示依赖 |
| deps | 指定gcc或msvc方式的依赖处理 |
| restat | 在command执行结束后，如果output时间戳不变，则当做未执行 |
| rspfile，`rspfile_content` | 同时指定，则执行command前，把`rspfile_content`写入rspfile文件，执行后删除 |

### 2.5 build edge
```ninja
build foo: phony bar
    var = str
```
build代码块，也是编译过程中的一个edge。
其中，foo就是output，bar就是input，`:`后面第一个位置的phony就是rule，var就是自定义变量。
在build块中，也可以对rule块的变量进行扩展（复写）。

最复杂的情况如下：
```ninja
build output0 output1 | output2 output3: rule_name $
        input0 input1 $
        | input2 input3 $
        || input4 input5
    var0 = str0
    var1 = str1
```
其中，行末的`$`是转义字符，并未真正换行；
output0和output1是显示（explicit）输出，会出现在`${out}`列表中；
出现在`|`后面的output2和output3是隐式（implicit）输出，不会出现在`${out}`列表中；
`rule_name`是规则名称； input0和input1是显示依赖（输入），会出现在`${in}`列表中；
出现在`|`后面的input2和input3是隐式依赖，不会出现在`${in}`列表中；
出现在`||`后面的input4和input5是隐式`order-only`依赖，不会出现在`${in}`列表中。

### 2.6 pool
pool的意义，在于限制一些非常消耗硬件资源的edge同时执行。
```ninja
pool example
    depth = 2

rule echo_var
    command = echo ${var} >> ${out}
    pool = example

build a: echo_var
    var = a

build b: echo_var
    var = b

build c: echo_var
    var = c
```
以上代码，通过`pool = example`，在rule或build代码块中指定对应的edge所属的pool为example。
由于example的`depth = 2`，所以a、b、c三个target最多只有2个可以同时生成。

目前，Ninja只有一个内置的pool，名为console。 这个pool的depth等于1，只能同时执行1个edge。
它的特点是，可以直接访问stdin、stdout、stderr三个特殊stream。

