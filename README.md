### 练习计算机图形学数学部分的项目，其目的是以简单的C++开发图形API，仿制一下OpenGL，本身是Visual Studio Windows窗体程序。

1. 2023-02-13

   初始化项目，添加.gitgnore，将HDC的Canvas绘制区域的RGBA buffer提取出来

2. 2023-02-14

   绘制点的算法(buffer绘点)，Brensenham画线算法(根据上下偏移量决定绘制的像素点)

3. 2023-02-15

   ColorLerp直线的颜色线性插值算法

4. 2023-02-23

   三角形绘制算法(扫描线算法)，Flat三角形绘制，通用三角形绘制算法（将三角形分割成两个平底Flat三角形）

5. 2023-02-24

   三角形绘制算法的边缘裁剪（将Canvas以外的点放弃判断），效率优化，三角形的平面颜色插值算法（先插边的颜色，再插中间一条线的颜色）

6. 2023-02-26

   引入图形读取绘制库

7. 2023-03-01

   图形Alpha(透明度)最小值，图片Alpha Blend混合(图片堆叠)，图片的虚化玻璃效果

8. 2023-03-02

   二次线性插值算法，对缩放后的点计算附近1个单位的四个点，并分别分离出disX1,disX2,disY1,disY2作比，得到插值结果

9. 2023-03-03

   uv纹理坐标的引入，将图片按uv坐标绘制到三角形上，新增了纹路绘制功能

10. 2023-03-03

   不知道Bug是啥原因了，打断点调了俩小时，对着m_buffer这个一维RGBA数组排查数据遇到比较大困难。

11. 2024-02-26

   二维Render部分做完了，三维部分需要建立新的非线性齐次矩阵
