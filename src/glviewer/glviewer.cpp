#include "glviewer.hpp"
#include<gl/freeglut.h>
namespace renderme::glviewer
{



    void RenderScenceCB()
    {
        // 清空颜色缓存
        glClear(GL_COLOR_BUFFER_BIT);
        // 交换前后缓存
        glutSwapBuffers();
    }

    auto GL_Viewer::main_loop()->void
    {
        // 通知开始GLUT的内部循环
        glutMainLoop();
    }

    auto GL_Viewer::init(int* argc, char** argv)->void
    {
        // 初始化GLUT
        glutInit(argc,argv);

        // 显示模式：双缓冲、RGBA
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

        // 窗口设置
        glutInitWindowSize(480, 320);      // 窗口尺寸
        glutInitWindowPosition(100, 100);  // 窗口位置
        glutCreateWindow("Tutorial 01");   // 窗口标题

        // 开始渲染
        glutDisplayFunc(RenderScenceCB);

        // 缓存清空后的颜色值
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    }


}