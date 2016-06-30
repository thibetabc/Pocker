
转载：http://blog.csdn.net/wanzhihui0000/article/details/22946073

最近在mac上编译Android 版本，各种崩溃让人蛋疼，网上学习了下ndk-stack使用方法。

自己备忘下：

1.运行终端。 跳转到你android sdk 目录 因为你的adb 在里面。

如 cd /Users/name/Android/adt-bundle-mac-x86_64-20131030/sdk/platform-tools 

2、找了路径正确继续下一步，./adb logcat | 你android ndk-stack所在的路径 -sym /你安卓工程.so文件所在的目录

如./adb logcat | /Users/name/Android/android-ndk-r8e/ndk-stack -sym /Users/name/test/proj.android/obj/local/armeabi

3、正确配置后会在终端出现

- waiting for device -
4、AndroidManifest.xml 设置 Debuggable 为true  在Application 切页中
5、另外启动一个终端 编译android工程
6、ADT连接设备运行你的工程就行了，出现崩溃的话会在你前面配置的终端里面打印出来。
如
********** Crash dump: **********
Build fingerprint: 'Xiaomi/aries/aries:4.1.1/JRO03L/JLB29.0:user/release-keys'
pid: 6546, tid: 6575, name: Thread-9654  >>> com.cocos2dx.SanGo <<<
signal 11 (SIGSEGV), code 1 (SEGV_MAPERR), fault addr deadbaad
Stack frame #00  pc 00013a78  /system/lib/libc.so
Stack frame #01  pc 00015e55  /system/lib/libc.so (dlfree+1628)
Stack frame #02  pc 00017003  /system/lib/libc.so (free+10)
Stack frame #03  pc 004965e8  /data/data/com.cocos2dx.test/lib/libgame.so (cocos2d::CCImage::~CCImage()+88): Routine __static_initialization_and_destruction_0 in /Users/name/Sango_Android/test/libs/cocos2dx/platform/android/CCApplication.cpp:127
Stack frame #04  pc 0049664c  /data/data/com.cocos2dx.teset/lib/libgame.so (cocos2d::CCImage::~CCImage()+20): Routine CCLog in /Users/name/test/libs/cocos2dx/platform/android/CCCommon.cpp:36
重要的就是 cpp 行号。为我们定位崩溃地点很有作用，再结合程序的打印输出位置，就能很快定位问题了。





--------------------------------

//chmod改读写 777 ./make.sh    sudo权限 ./make.sh man查询





针对AndroidStudio进行编译
终端 cd 到项目的 proj.android 打开如下命令： 
cocos compile -p android --android-studio

xcode打包apk
终端 cd 到项目的 proj.android 打开如下命令： 
cocos run -p android -m release
编译android项目
cocos compile -p android




第三方渠道：
去掉微信支付的sdk，，分两个android包，，编译时区分代码注释




美团Android自动化之旅—生成渠道包
http://tech.meituan.com/mt-apk-packaging.html
美团Android自动化之旅—适配渠道包
http://tech.meituan.com/mt-apk-adaptation.html



##BUG堆栈 ndk-stack：
1）终端cd到adb路径：
    /Users/qqronaldz/Documents/eclipse/sdk/platform-tools

2）./adb logcat | 你android ndk-stack所在的路径/ndk-stack -sym /你安卓工程.so文件所在的目录：
   ./adb logcat | /Users/qqronaldz/Documents/eclipse/android-ndk-r9d/ndk-stack -sym /Users/qqronaldz/Desktop/project/code/pokerclient/proj.android/obj/local/armeabi-v7a
./adb logcat | /Users/qqronaldz/Documents/eclipse/android-ndk-r9d/ndk-stack -sym /Users/qqronaldz/Desktop/project/code/ReleaseClient/proj.android/obj/local/armeabi-v7a




