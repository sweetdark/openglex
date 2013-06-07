openglex
========
opengl 入门练习

第一步，选择一个编译环境

现在Windows系统的主流编译环境有Visual Studio，Broland C++ Builder，Dev-C++等，它们都是支持OpenGL的。但这里我们选择Visual Studio 2005作为学习OpenGL的环境。

第二步，安装GLUT工具包

GLUT不是OpenGL所必须的，但它会给我们的学习带来一定的方便，推荐安装。
Windows环境下的GLUT下载地址：（大小约为150k）
http://www.opengl.org/resources/libraries/glut/glutdlls37beta.zip
无法从以上地址下载的话请使用下面的连接:
http://upload.programfan.com/upfile/200607311626279.zip
Windows环境下安装GLUT的步骤：
1、将下载的压缩包解开，将得到5个文件
2、在“我的电脑”中搜索“gl.h”，并找到其所在文件夹（如果是VisualStudio2005，则应该是其安装目录下面的“VC\PlatformSDK\include\gl文件夹”）。把解压得到的glut.h放到这个文件夹。
3、把解压得到的glut.lib和glut32.lib放到静态函数库所在文件夹（如果是VisualStudio2005，则应该是其安装目录下面的“VC\lib”文件夹）。
4、把解压得到的glut.dll和glut32.dll放到操作系统目录下面的system32文件夹内。（典型的位置为：C:\Windows\System32）

第三步，建立一个OpenGL工程

这里以VisualStudio2005为例。
选择File->New->Project，然后选择Win32 Console Application，选择一个名字，然后按OK。
在谈出的对话框左边点Application Settings，找到Empty project并勾上，选择Finish。
然后向该工程添加一个代码文件，取名为“OpenGL.c”，注意用.c来作为文件结尾。
搞定了，就跟平时的工程没什么两样的。
