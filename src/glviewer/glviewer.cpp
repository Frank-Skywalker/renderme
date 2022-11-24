#include "glviewer.hpp"
#include <cstdio>
#include<gl/freeglut.h>
namespace renderme::glviewer
{

    void RenderScenceCB()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glutSwapBuffers();
    }

    auto GL_Viewer::main_loop(int* argc, char** argv)->void
    {
        //Initialization
        //Routines beginning with the glutInit- prefix are used to initialize GLUT state. 
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
        glutInitWindowSize(480, 320);
        glutInitWindowPosition(100, 100);

        glutInitErrorFunc(
            [] (char const* fmt, va_list ap) {
                vprintf(fmt, ap);
                exit(1);
            }
        );
        glutInitWarningFunc(
            [] (char const* fmt, va_list ap) {
                vprintf(fmt, ap);
            }
        );
        glutInit(argc, argv);

        //Window Management
        glutCreateWindow("renderme");   

        //Callback Registration
        glutDisplayFunc(RenderScenceCB);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


        //Beginning Event Processing
        //After a GLUT program has done initial setup such as creating windowsand menus, 
        //GLUT programs enter the GLUT event processing loop by calling glutMainLoop.
        glutMainLoop();
    }


}