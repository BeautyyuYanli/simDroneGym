本项目的源代码位于目录 `simDroneGym/` 中. 本项目采用声明与定义分离的结构, 所有声明均在 `simDroneGym/include/` 的 `.hpp` 头文件中, 所有定义均在 `simDroneGym/src/` 的 `.cpp` 文件中. 所有 `.hpp` 文件与 `.cpp` 文件一一对应.

以下分文件展示类及其成员, 其中忽略在 "API 文档" 中已提及的内容.

### 1. numcpp

头文件: `simDroneGym/include/numcpp.hpp`

#### triple

该类在 "API 文档" 中均已解释.

#### intervals (区间集)

`intervals` (区间集) 是一系列不重叠左闭右开区间的集合. 主要在碰撞检测中用于计算时间区间.

`std::vector<std::pair<double, bool>> v;`

存储区间的所有边界节点. 其中的 `double` 值为边界点, `bool` 值取 `0` 表示为左界, 取 `1` 表示为右界.

`intervals baseArithmetic(const intervals &, bool mode) const;`

集合交运算和并运算的共同逻辑函数, `mode` 值取 `0` 表示并运算, `1` 表示交运算

`intervals() = default;`

默认构造函数, 空集.

`intervals(double left, double right);`

构造函数, 指定一个区间的集合.

`intervals operator+(const intervals &) const;`

并运算.

`intervals operator*(const intervals &) const;`

交运算.

`bool empty() const;`

返回该集合是否为空.

`void print(std::ostream &os) const;`

打印该区间信息.

`std::ostream& operator<<(std::ostream &, const simDroneGym::intervals &);`

非成员函数, 流运算符 `<<`, 输出区间信息.

### 2. shape

头文件: `simDroneGym/include/shape.hpp`

该类族在 "API 文档" 中均已解释.

### 3. spirit

头文件: `simDroneGym/include/spirit.hpp`

#### baseSpirit

`const environment *env = nullptr;`

指向所处 `environment` 的指针, 此时 `environment` 声明为虚类. 默认该指针为 `NULL`

`baseSpirit() = default;`

默认构造函数

`baseSpirit(const environment *);`

构造函数, 指定 `env` 的值.

`virtual void dispatcher();`

派遣器函数, 执行时间流动 1s 时的变化, 主要为运动学属性的变化.

#### realSpirit

`realSpirit(const environment *);`

构造函数, 指定 `env` 的值.

`realSpirit(const realSpirit *);`

构造函数, 类似于拷贝构造函数, 复制指针所指的对象.

`virtual void dispatcher();`

派遣器函数, 执行时间流动 1s 时的变化, 主要为运动学属性的变化. 保证自身不超出空间边界.

#### obstacleSpirit

`obstacleSpirit(const environment *);`

构造函数, 指定 `env` 的值.

#### droneSpirit

`droneSpirit(const environment *);`

构造函数, 指定 `env` 的值.

`void dispatcher();`

派遣器函数, 执行时间流动 1s 时的变化, 主要为运动学属性的变化. 保证自身不超出空间边界. 在 `workingState` 为 `false` 时将自身属性全部设为 0.

### 4. environment

头文件: `simDroneGym/include/environment.hpp`

`std::vector<std::vector<std::string>> actionsList;`

动作列表, 存储用户定义的动作.

`std::vector<realSpirit> radarRespond(const droneSpirit &detector)const;`

雷达响应, 回复 `detector` 请求的雷达信息.
