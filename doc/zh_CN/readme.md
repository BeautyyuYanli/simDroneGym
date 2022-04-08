# simDroneGym

[simDroneGym](https://github.com/BeautyYuYanli/simDroneGym) 是一个为无人机提供模拟环境的训练场. 你可以设置一个环境, 并编写你的算法来控制其中的无人机. 

## 一. 编译和引用

本项目使用 `c++17` 标准编译, 请先确认你的编译器版本足够高.

你可以使用以下两种方式的任意一种使用本项目. 二进制文件将存放在 `build/` 文件夹中, 请先新建该文件夹.

### 1. 编译为静态库

```shell
g++ -std=c++17 -c simDroneGym/src/simdronegym.cpp -o build/libsimDroneGym.o
ar rvs build/libsimDroneGym.a build/libsimDroneGym.o
```

之后, 将 `simDroneGym/include/` 文件夹复制到你的项目中, 并引用其中的头文件  `simdronegym.hpp`. 形如:

```cpp
// a.cpp
#include "simDroneGym/include/simdronegym.hpp"
```

并在编译时加入静态库 `build/libsimDroneGym.a`. 形如:

```shell
g++ a.cpp build/libsimDroneGym.a
```

或者, 你可以直接使用 `cmake` 编译.

### 2. 直接引用代码

将 `simDroneGym/` 文件夹复制到你的项目中, 并引用其中的源代码 `src/simdronegym.cpp`. 形如:

```cpp
// b.cpp
#include "simDroneGym/include/simdronegym.cpp"
```

直接编译即可:

```shell
g++ b.cpp
```

## 二. 使用

`demo/` 目录下有两份示例, 其中 `demo1.cpp` 展现了本库的大部分功能; `demo2.cpp` 则设置了一个较为简单的环境, 并使用 `Q-Learning` 算法控制无人机飞向目的地.

阅读更多[示例说明](demo.md)

### 三. API 文档

阅读[API 文档](api.md)

### 四. 源代码文档

阅读[源代码文档](source.md)

## 许可证

[GNU GPLv3](https://www.gnu.org/licenses/)