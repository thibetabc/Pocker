Mac下Eclipse编译cocos2dx的c++
环境：
Mac OS X El Catpion 10.11.4 (15E65)
Xcode 7.3
Eclipse 4.5.0
Cocos2dx 3.6

过程：
Eclipse 导入proj.android工程后,按照链接1操作，运行报错：

sh: cocos: command not found
Traceback (most recent call last):
  File "/Users/qqronaldz/Desktop/project/code/pokerclient/proj.android/build_native.py", line 45, in <module>
    build(opts.build_mode)
  File "/Users/qqronaldz/Desktop/project/code/pokerclient/proj.android/build_native.py", line 30, in build
    raise Exception("Build dynamic library for project [ " + app_android_root + " ] fails!")
Exception: Build dynamic library for project [ /Users/qqronaldz/Desktop/project/code/pokerclient/proj.android/../ ] fails!
Please use cocos console instead.

根据链接2提示，在链接1的操作步骤【切换到Environment标签】填入（对应Eclpise中Environment标签 的  Variable 和 Value）:

1、ANDROID_SDK_ROOT ：/Users/qqronaldz/Documents/eclipse/sdk/
2、ANDROID_NDK_ROOT ：/Users/qqronaldz/Documents/eclipse/sdk/
3、NDK_ROOT ：/Users/qqronaldz/Documents/eclipse/android-ndk-r10e/
4、COCOS_CONSOLE_ROOT ： /Applications/Cocos/Cocos2d-x/cocos2d-x-3.10/tools/cocos2d-console/bin
5、COCOS_FRAMEWORKS ： /Applications/Cocos/frameworks
6、ANT_ROOT ： /Applications/Cocos/tools/ant/bin

如何查看自己Value路径：
在mac终端中键入：vi ~/.bash_profile 就可以看到一堆东西，

# ————————guo ——————————
export ANDROID_SDK_ROOT=/Users/qqronaldz/Documents/eclipse/sdk/
export ANDROID_NDK_ROOT=/Users/qqronaldz/Documents/eclipse/android-ndk-r9d/
#export COCOS2DX_ROOT=/
export NDK_ROOT=/Users/qqronaldz/Documents/eclipse/android-ndk-r10e/
export PB_ROOT=/
export PATH=$ANDROID_SDK_ROOT:$PATH
export PATH=$ANDROID_SDK_ROOT/tools:$ANDROID_SDK_ROOT/platform-tools:$PATH
export PATH=$ANDROID_NDK_ROOT:$PATH
#---------------------------

# Add environment variable COCOS_CONSOLE_ROOT for cocos2d-x
export COCOS_CONSOLE_ROOT=/Applications/Cocos/Cocos2d-x/cocos2d-x-3.10/tools/cocos2d-console/bin
export PATH=$COCOS_CONSOLE_ROOT:$PATH

# Add environment variable COCOS_FRAMEWORKS for cocos2d-x
export COCOS_FRAMEWORKS=/Applications/Cocos/frameworks
export PATH=$COCOS_FRAMEWORKS:$PATH

# Add environment variable ANT_ROOT for cocos2d-x
export ANT_ROOT=/Applications/Cocos/tools/ant/bin

export PATH=$ANT_ROOT:$PATH

export PATH=/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin:/usr/X11/bin:$PATH

#gradle
export GRADLE_HOME=/Users/qqronaldz/Downloads/gradle-2.13
export PATH=$PATH:$GRADLE_HOME/bin

# Add environment variable COCOS_X_ROOT for cocos2d-x
export COCOS_X_ROOT=/Applications/Cocos/Cocos2d-x
export PATH=$COCOS_X_ROOT:$PATH

# Add environment variable COCOS_TEMPLATES_ROOT for cocos2d-x
export COCOS_TEMPLATES_ROOT=/Applications/Cocos/Cocos2d-x/cocos2d-x-3.10/templates
export PATH=$COCOS_TEMPLATES_ROOT:$PATH

然后看清楚，复制粘贴就好

最后要修改 项目里的build_native.py文件

将command = 'cocos compile -p android -s %s -m %s' % (app_android_root, build_mode)改成

COCOS_CONSOLE_ROOT = os.environ['COCOS_CONSOLE_ROOT']
command = '%s/cocos compile -p android -s %s -m %s' % (COCOS_CONSOLE_ROOT,app_android_root, build_mode)


就能运行编译！

#在Eclipse打开Classes里的c++文件(参考链接4)

选中工程Pocker，右键-New-Folder，Enter or select the parent folder选择Poker, FolderName自己起一个名字（这里我填了Classes）。
点 advanced，选择最后一个选项：Link to alternate location(Linked Folder)，在下面输入要导进来的cpp所在的目录（这里我填了 /Users/qqronaldz/Desktop/project/code/Poker/Classes），点 finish即可










=================以下为参考资料==================================================================

参考链接：
链接1: http://www.cocos.com/doc/article/index?type=cocos2d-x&url=/doc/cocos-docs-master/manual/framework/html5/jsb/compilation-in-eclipse/zh.md
链接2: http://my.oschina.net/u/2274890/blog/493574
链接3: http://www.cocoachina.com/bbs/read.php?tid-296022-page-3.html
链接4: http://www.cnblogs.com/zouzf/p/4202537.html
在Eclipse中编译安卓项目

这篇文档会展示如何在eclipse中编译一个cocos2d-js的android项目。

需要的环境:

ADT bundle
NDK r9d
python 2.7
创建项目

像我们通常做的那样，在终端运行cocos new MyGame -l js -d ~/workspace。

在eclipse中导入项目

打开ADT bundle中的eclipse，点击File->new->other，选择Android Project from Existing Code。



在Root Directory中选择~/workspace/MyGame。我们可能会看到很多eclipse找到了很多项目，这里我们暂时只需要选择MyGame和libcocosd2dx。

然后我们就可以在Package Explorer看到这两个项目被导入了。



在eclipse中设置一个NDK builder

我们需要设置一个NDK builder来编译C++代码。在MyGame点击右键，选择Properties。



在Builders设置中，点击New新建一个builder。在弹出对话框选择program，点击OK会看到下面的窗口。



现在我们开始设置这个Builder。在Location下面点击Browse Worksapce，选择MyGame中的build_native.py。



然后在Working Directory下面点击Brwose Worksapce，选择MyGame文件夹。



在Arguments中填入-b release或者-b debug。如果不填，默认debug编译。



切换到Refresh标签， 选中Refresh resources upon completion。



切换到Environment标签，点击New按钮， 添加NDK_ROOT。



现在我们就完成了NDK builder的设置，点击OK按钮关闭对话框。

运行

像普通的android项目那样clean & run就可以了。所有的资源和脚本会自动拷贝到assets文件夹，C++代码会自动编译，然后生成apk。






--------------------------------------------------------------------------------
笔者的环境：mac, eclipse, ndk-r10c,cocos2.3,cocos2dx3.7.1

目前cocos提供了多种方式供开发者打包android版本的游戏。目前最稳定的方式是通过cocostudio中的打包方式，也是最方便的，只需要在偏好设置里设置好ndk，sdk的目录即可。

但是当开发后期我们需要为自己的游戏移植sdk时，可能这个方法不能满足我们的需要，我们就需要用传统的打包方式。

即把项目中的proj.android导入eclipse。再在eclipse里开发相应的java代码完成sdk的移植。

从3.5开始android项目的编译C++的选项默认是未选取的。 



这个时候会有很多问题，首先我们需要配置环境变量



一共是这些变量。最好都不要少，mac环境下的eclipse貌似不会去.bash_profile下找定义的环境变量


注意一个地方

COCOSFRAMEWORK要定义成/Applications/Cocos/frameworks/

这里是我自己试对的。。－－

然后要修改 项目里的build_native.py文件

将command = 'cocos compile -p android -s %s -m %s' % (app_android_root, build_mode)改成

COCOS_CONSOLE_ROOT = os.environ['COCOS_CONSOLE_ROOT']
    command = '%s/cocos compile -p android -s %s -m %s' % (COCOS_CONSOLE_ROOT,app_android_root, build_mode)
    

大功告成～
