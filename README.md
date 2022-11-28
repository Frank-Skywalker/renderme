# README



## CMakeLists



##### assimp/CMakeLists.txt

未作修改，写的本来就很好

##### glfw/CMakeLists.txt

禁掉CMake对dll/lib输出位置的修改

##### glew/CMakeLists.txt

将build/cmake/拷贝到glew根目录

将GLEW_DIR变量修改为正确目录

禁掉CMake对dll/lib输出位置的修改

```cmake
#set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
#set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
#set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
```





## 问题

##### util.hpp

静态成员变量

函数内静态变量

模板类

class和struct的区别

五大构造函数

