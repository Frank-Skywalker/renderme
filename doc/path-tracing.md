# Path Tracing Report

##### 22221332 陈九润



## 编程环境

- OS：Windows 11 Pro 22H2
- CPU: Intel(R) Core(TM) i7-8750H CPU @ 2.20GHz   2.21 GHz
- GPU: Geforce GTX 1060 6G
- RAM: 16GB
- IDE: Visual Studio 2022





## 使用说明

详见README.md

打开bin/renderme.exe

- 控制相机：WASD、CTRL、SPACE、按住右键拖动鼠标
- 选择场景：拖动Scene滑块
  - ![image-20230111215054186](./zbuffer.assets/image-20230111215054186.png)
- 开启ZBuffer：选中ZBuffer
  - ![image-20230111215045752](./zbuffer.assets/image-20230111215045752.png)
- 场景信息：
  - ![image-20230111215108636](./zbuffer.assets/image-20230111215108636.png)





## 数据结构说明



源代码位于src/integrators/zbuffer.hpp和src/integrators/zbuffer.cpp

每帧执行的ZBuffer函数为ZBuffer_Integrator::render

ZBuffer算法，未进行加速



##### ZBuffer算法流程

```c++
build_polygon_table();	//构建多边形表
perform_zbuffer();		//进行ZBuffer算法
clean_polygon_table();	//清理数据结构
```



##### 边

```c++
	struct Edge final
	{
		// Not changed during zbuffer
		int ymax;		//边的最大y值
		Float dxdl;		//沿着边向下移动一条扫描线，对应x的变化值

		// Changed during zbuffer
		int dy;			//边跨越的剩余扫描线数量
		Float x;		//边在当前扫描线处的x值
		Float z;		//边在当前扫描线处的z值
	}; 
```



##### 活化边表

```c++
	using Active_Edge_List = std::list<Edge*>;
```



##### 多边形

```c++
	struct Polygon final
	{
		// Not changed during zbuffer
		glm::vec4 equation;					//多边形所在平面的方程
		Float dzdx;							//多边形所在平面的dzdx
		Float dzdy;							//多边形所在平面的dzdy
		glm::vec3 color;					//多边形颜色
		
		// Changed during zbuffer
		int dy;								//多边形跨越的剩余扫描线数量
		std::vector<Edge> edges;			//属于该多边形的所有边
		Active_Edge_List active_edge_list;	//属于该多边形的活化多边形表

	};
```



##### 分类多边形表

```c++
// 二维数组
// 第一维为总的扫描线数目
using Polygon_Table = std::vector<std::vector<Polygon>>;
```



##### 活化多边形表

```c++
using Active_Polygon_List = std::list<Polygon*>;
```





## 实验结果



##### Cube

- 顶点数：24
- 面数：12
- 生成每帧时间：42ms

![image-20230111220458370](./zbuffer.assets/image-20230111220458370.png)

![image-20230111220504659](./zbuffer.assets/image-20230111220504659.png)



##### Nanosuit

- 顶点数：21461
- 面数：19058
- 生成每帧时间：260ms

![image-20230111220643141](./zbuffer.assets/image-20230111220643141.png)

![image-20230111220633957](./zbuffer.assets/image-20230111220633957.png)



##### Backpack

- 顶点数：53464
- 面数：67907
- 生成每帧时间：1300ms

![image-20230111221128821](./zbuffer.assets/image-20230111221128821.png)

![image-20230111221342759](./zbuffer.assets/image-20230111221342759.png)
