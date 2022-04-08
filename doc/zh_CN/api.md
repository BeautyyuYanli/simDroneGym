本项目的所有类都处于命名空间 `simDroneGym` 中. 以下展示用户可直接调用的类及其成员

### 1. triple (三元组)

`triple` 是一个三维向量, 通常用于存储物体在三维空间中的运动学属性, 例如坐标, 速度, 加速度等.

---

`double val[3];`

存储数值

`triple();`

默认构造函数, 使初始值为 (0,0,0)

`triple(double val_1, double val_2, double val_3);`

构造函数, 指定向量的值

`triple(const triple &);`

拷贝构造函数

`double& operator[](size_t index);`

运算符`[]`, 取向量第 index 维度的值

`const double& operator[](size_t index)const;`

同上, 常量版本

`triple& operator=(const triple &);`

赋值函数, 同拷贝构造函数

`bool operator==(const triple &);`

运算符`==`, 判断相等

`triple operator+(const triple &) const;`

运算符`+`, 向量加法

`triple operator-(const triple &) const;`

运算符`-`, 向量减法

`triple operator*(double) const;`

运算符`*`, 向量数乘

`triple& operator+=(const triple &);`

运算符`+=`

`triple& operator-=(const triple &);`

运算符`-=`

---

**与该类相关的非成员函数:**

`std::istream& operator>>(std::istream &, simDroneGym::triple &);`

流运算符`>>`, 输入 triple. 实质为输入三个 double, 为 triple 三个维度的数值.

`std::ostream& operator<<(std::ostream &, const simDroneGym::triple &);`

流运算符`<<`, 输出 triple.

### 2. shape (形状) 类族

`shape` 描述一个几何形状的存在. 包括基类 `baseShape` 及其派生类 `cuboidShape` (长方体). 本项目使用的形状均为长方体.

#### class baseShape

`triple origin;`

相对原点坐标, 即该形状的原点与"拥有该形状的对象"(通常为下述 `spirit` )的坐标的相对值.

#### class cuboidShape : public baseShape

`triple length;`

长方体边长, 即从原点开始向三个维度衍生的长度.

`cuboidShape();`

默认构造函数, `origin` 和 `length` 均为 0 向量, 即无形状.

`cuboidShape(std::string presetMode, std::initializer_list<triple> argv);`

带有预设 `presetMode` 的构造函数, 预设如下:

- `"cuboid"`: 长方体, `origin` 为 0, `length` 为 `argv[0]`

- `"radar"` 和 `"cuboid_centered"`: 居中的长方体, `length` 为 `argv[0]`, `origin` 为 `length * (-0.5)`

- `"radar_oracle"` 和 `"manual"`: 手动设定, `length` 为 `argv[0]`, `origin` 为 `argv[1]`

### 3. spirit (精灵) 类族

环境中的一切物体均为 `spirit`, 包括基类 `baseSpirit`, 其派生类 `realSpirit` (实体精灵) , 以及 `realSpirit` 的派生类 `obstacleSpirit` (障碍物) 和 `droneSpirit` (无人机).

#### class baseSpirit

基础的 `spirit` 类, 能感知自身的运动状态, 以及时间的流逝.

`triple coord, velocity, acceleration;`

精灵的坐标, 速度, 加速度.

`virtual void display(std::ostream &os)const;`

打印该精灵的信息到输出流 os 上.

#### class realSpirit : public baseSpirit

`realSpirit` (实体精灵) 是具有形状的 `spirit`.

`cuboidShape sp;`

自身形状, 默认为无形状.

`bool overlapCheck(const realSpirit &)const;`

检查是否与另一 `realSpirit` 在空间中重叠

`bool collisionCheck(const realSpirit &)const;`

检查是否与另一 `realSpirit` 在接下来的一秒内发生碰撞

`virtual void display(std::ostream &os)const;`

打印该实体精灵的信息到输出流 os 上.

#### class obstacleSpirit : public realSpirit

`obstacleSpirit` (障碍物) 基本与 `realSpirit` 相同

`void display(std::ostream &os)const;`

打印该障碍物的信息到输出流 os 上.

#### class droneSpirit : public realSpirit

`droneSpirit` (无人机) 有工作状态, 具有 radar (雷达) 功能

`bool workingState = true;`

工作状态, 无法正常工作时为 `false`

`cuboidShape radarScope;`

雷达的探测范围

`std::vector<realSpirit> radarResults;`

雷达探测到的 `realSpirit`

`void radarDetect();`

启动雷达, 更新 `radarResults`

`std::string radarResultsToString();`

返回 `radarResults` 字符串化的结果

`void display(std::ostream &os)const;`

打印该无人机的信息到输出流 os 上.

`std::string toString()const;`

返回自身字符串化的结果

### 4. environment (环境)

environment (环境) 是一切 `spirit` 存在的空间, 也是面向用户的主要接口.

`triple minCoord = triple(-1024,-1024,-1024), maxCoord = triple(1024,1024,1024);`

该空间的边界, 默认为从 (-1024,-1024,-1024) 到 (1024,1024,1024) 的正方体.

`std::vector<obstacleSpirit> obstaclesList;`

环境中的所有障碍物列表.

`std::vector<droneSpirit> dronesList;`

环境中的所有无人机列表.

`std::vector<realSpirit> destinationsList;`

环境中的所有目的地列表.

`size_t droneCreate();`

创造一个无人机, 并返回其在列表中的编号.

无人机默认坐标为 (0,0,0), 形状是 (0,0,0) 到 (1,1,1) 的正方体. 速度和加速度均为 0, 雷达可以探视空间边界之内的所有无人机和障碍物.

`size_t obstacleCreate();`

创造一个障碍物, 并返回其在列表中的编号.

默认坐标, 速度和加速度均为 0, 无形状.

`size_t destinationCreate();`

创造一个目的地, 并返回其在列表中的编号.

默认坐标, 速度和加速度均为 0, 无形状.

`bool actionsDefine(std::vector<std::vector<std::string>>);`

定义一个列表的动作, 动作是作用于某个无人机上.

每个动作都由三个字符串, "行为", "属性", "值" 组成.

第一个字符串为"行为", 可取值为:

- `"set"`: 设置"属性"为"值"

- `"add"`: 使"属性"增加"值"

第二个字符串为"属性", 可取值为:

- `"coord"`: 坐标

- `"velocity"`: 速度

- `"acceleration"`: 加速度

- `"workingState"`: 工作状态

第三个字符串为"值", 其应符合相应类型 (`triple` 或 `bool`) 使用流运算符 `>>` 输入的格式.

返回值为动作列表定义是否成功. 若不成功将会在 `cerr` 流上输出信息.

`environment& act(size_t actionIndex, size_t droneIndex);`

对于编号为 `droneIndex` 的无人机, 执行编号为 `actionIndex` 的动作.

返回值为环境自身的引用, 用户可以使用链式写法执行多个动作.

`environment& actDone();`

完成所有动作, 使时间流过 1s.

1s 时间内可能发生碰撞等事件.

返回值为环境自身的引用.

`void display(std::ostream &os)const;`

打印该环境的信息到输出流 os 上.

`std::string toString()const;`

返回自身字符串化的结果

`bool stopCheck(size_t droneIndex)const;`

检查编号为 `droneIndex` 的无人机的运动状态是否为静止 (无速度和加速度).

`bool onDestination(size_t droneIndex, size_t destinationIndex)const;`

检查编号为 `droneIndex` 的无人机是否与编号为 `destinationIndex` 的目的地区域重叠.
