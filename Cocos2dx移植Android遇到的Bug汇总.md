#程序包org.cocos2dx.lib不存在

解决办法：
在src下的project.properties文件中添加如下依赖项：
android.library.reference.1=../cocos2d/cocos/platform/android/java


#android拍照后，使用图片黑块。
解决办法：
//android照相返回-要在主线程操作
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&]()
    {
        //更新UI
       // update_chat_layer();
    });
    



**遇到问题
> Pocker的ios第一个版本迭代后，进行Android版本的移植，遇到如下Bug：

<pre><code>
05-10 17:50:51.583: D/umeng(8306): loadLibrary
05-10 17:50:51.583: D/dalvikvm(8306): Trying to load lib /data/app-lib/com.insul.poker-1/libmobclickcpp.so 0x428810f0
05-10 17:50:51.600: D/dalvikvm(8306): Added shared lib /data/app-lib/com.insul.poker-1/libmobclickcpp.so 0x428810f0
05-10 17:50:51.600: D/JniHelper(8306): JniHelper::setJavaVM(0x41cf99d8), pthread_self() = 1075520332
05-10 17:50:51.603: D/umeng(8306): loadLibrary did nothing
05-10 17:50:51.607: D/dalvikvm(8306): Trying to load lib /data/app-lib/com.insul.poker-1/libcocos2dcpp.so 0x428810f0
05-10 17:50:51.730: D/dalvikvm(8306): Added shared lib /data/app-lib/com.insul.poker-1/libcocos2dcpp.so 0x428810f0
05-10 17:50:51.730: D/JniHelper(8306): JniHelper::setJavaVM(0x41cf99d8), pthread_self() = 1075520332
05-10 17:50:51.773: D/Cocos2dxActivity(8306): model=HUAWEI P6-T00
05-10 17:50:51.773: D/Cocos2dxActivity(8306): product=P6-T00
05-10 17:50:51.773: D/Cocos2dxActivity(8306): isEmulator=false
05-10 17:50:51.780: D/com.umeng.message.MessageSharedPrefs(8306): Constructor()
05-10 17:50:51.780: E/com.umeng.message.UmengRegistrar(8306): isRegisteredToUmeng: empty registration id
05-10 17:50:51.863: W/System.err(8306): Invalid int: ""
05-10 17:50:51.970: E/com.umeng.message.UTrack(8306): RegistrationId is empty
05-10 17:50:52.160: D/com.umeng.message.PushAgent(8306): processName=com.insul.poker
05-10 17:50:52.160: D/com.umeng.message.PushAgent(8306): The AndroidManifest config is right
05-10 17:50:52.277: E/WXREGEST(8306): WXREGEST
05-10 17:50:52.277: E/mark(8306): onCreate
05-10 17:50:52.277: D/dalvikvm(8306): Trying to load lib /data/app-lib/com.insul.poker-1/libmobclickcpp.so 0x428810f0
05-10 17:50:52.280: D/dalvikvm(8306): Shared lib '/data/app-lib/com.insul.poker-1/libmobclickcpp.so' already loaded in same CL 0x428810f0
05-10 17:50:52.307: D/Cocos2dxActivity(8306): onResume()
05-10 17:50:52.307: V/MicroMsg.SDK.WXApiImplV10.ActivityLifecycleCb(8306): com.insul.poker.AppActivity  onActivityResumed
05-10 17:50:52.317: D/com.umeng.message.RegistrationReceiver(8306): RegistrationReceiver
05-10 17:50:52.320: D/BaseIntentService(8306): runIntent to receive one messageIntent { act=com.insul.poker.intent.action.COMMAND flg=0x10 pkg=com.insul.poker cmp=com.insul.poker/com.umeng.message.RegistrationReceiver (has extras) }|com.umeng.message.UmengIntentService
05-10 17:50:52.430: D/libEGL(8306): loaded /system/lib/egl/libEGL_VIVANTE.so
05-10 17:50:52.443: D/plat(8306): new sso handler
05-10 17:50:52.450: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.UMLWHandler
05-10 17:50:52.450: D/plat(8306): new sso handler
05-10 17:50:52.450: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.SinaSsoHandler
05-10 17:50:52.453: D/plat(8306): new sso handler
05-10 17:50:52.453: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.UMPinterestHandler
05-10 17:50:52.453: D/plat(8306): new sso handler
05-10 17:50:52.453: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.QZoneSsoHandler
05-10 17:50:52.453: D/plat(8306): new sso handler
05-10 17:50:52.453: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.UMQQSsoHandler
05-10 17:50:52.453: D/plat(8306): new sso handler
05-10 17:50:52.457: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.RenrenSsoHandler
05-10 17:50:52.457: D/plat(8306): new sso handler
05-10 17:50:52.460: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.QQwbHandler
05-10 17:50:52.460: D/plat(8306): new sso handler
05-10 17:50:52.463: D/plat(8306): new sso handler
05-10 17:50:52.463: D/libEGL(8306): loaded /system/lib/egl/libGLESv1_CM_VIVANTE.so
05-10 17:50:52.467: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.UMYXHandler
05-10 17:50:52.467: D/plat(8306): init gets called
05-10 17:50:52.467: D/plat(8306): new sso handler
05-10 17:50:52.467: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.EmailHandler
05-10 17:50:52.470: D/plat(8306): new sso handler
05-10 17:50:52.470: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.UMEvernoteHandler
05-10 17:50:52.470: D/plat(8306): new sso handler
05-10 17:50:52.473: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.UMFacebookHandler
05-10 17:50:52.473: D/plat(8306): new sso handler
05-10 17:50:52.477: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.UMFlickrHandler
05-10 17:50:52.477: D/plat(8306): new sso handler
05-10 17:50:52.477: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.UMFourSquareHandler
05-10 17:50:52.477: D/plat(8306): new sso handler
05-10 17:50:52.480: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.UMGooglePlusHandler
05-10 17:50:52.480: D/libEGL(8306): loaded /system/lib/egl/libGLESv2_VIVANTE.so
05-10 17:50:52.480: D/plat(8306): new sso handler
05-10 17:50:52.480: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.UMInstagramHandler
05-10 17:50:52.480: D/plat(8306): new sso handler
05-10 17:50:52.483: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.UMKakaoHandler
05-10 17:50:52.483: D/plat(8306): new sso handler
05-10 17:50:52.483: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.UMLineHandler
05-10 17:50:52.483: D/plat(8306): new sso handler
05-10 17:50:52.493: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.UMLinkedInHandler
05-10 17:50:52.493: D/plat(8306): new sso handler
05-10 17:50:52.503: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.UMPocketHandler
05-10 17:50:52.507: D/plat(8306): new sso handler
05-10 17:50:52.507: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.UMWhatsAppHandler
05-10 17:50:52.507: D/plat(8306): new sso handler
05-10 17:50:52.507: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.UMYNoteHandler
05-10 17:50:52.507: D/plat(8306): new sso handler
05-10 17:50:52.507: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.SmsHandler
05-10 17:50:52.510: D/plat(8306): new sso handler
05-10 17:50:52.513: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.DoubanHandler
05-10 17:50:52.513: D/plat(8306): new sso handler
05-10 17:50:52.513: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.UMTumblrHandler
05-10 17:50:52.513: D/plat(8306): new sso handler
05-10 17:50:52.513: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.TwitterHandler
05-10 17:50:52.513: D/plat(8306): new sso handler
05-10 17:50:52.513: V/xxxx(8306): ignore=java.lang.ClassNotFoundException: com.umeng.socialize.handler.AlipayHandler
05-10 17:50:52.540: V/ProxyFactroy(8468): getInstance[base] successfully
05-10 17:50:52.580: D/CocosPlayClient.cpp(8306): isEnabled = 0
05-10 17:50:52.580: D/CocosPlayClient.cpp(8306): isDemo = 0
05-10 17:50:52.580: D/CocosPlayClient.cpp(8306): isNotifyFileLoadedEnabled = 0
05-10 17:50:52.600: D/Cocos2dxActivity(8306): onWindowFocusChanged() hasFocus=true
05-10 17:50:52.613: D/cocos2d-x debug info(8306): {
05-10 17:50:52.613: D/cocos2d-x debug info(8306):   gl.supports_vertex_array_object: false
05-10 17:50:52.613: D/cocos2d-x debug info(8306):   gl.supports_BGRA8888: false
05-10 17:50:52.613: D/cocos2d-x debug info(8306):   gl.supports_ATITC: false
05-10 17:50:52.613: D/cocos2d-x debug info(8306):   gl.supports_S3TC: false
05-10 17:50:52.613: D/cocos2d-x debug info(8306):   cocos2d.x.version: cocos2d-x 3.5
05-10 17:50:52.613: D/cocos2d-x debug info(8306):   gl.supports_discard_framebuffer: true
05-10 17:50:52.613: D/cocos2d-x debug info(8306):   cocos2d.x.compiled_with_profiler: false
05-10 17:50:52.613: D/cocos2d-x debug info(8306):   gl.supports_PVRTC: false
05-10 17:50:52.613: D/cocos2d-x debug info(8306):   cocos2d.x.build_type: DEBUG
05-10 17:50:52.613: D/cocos2d-x debug info(8306):   gl.renderer: Immersion.16
05-10 17:50:52.613: D/cocos2d-x debug info(8306):   gl.supports_ETC1: true
05-10 17:50:52.613: D/cocos2d-x debug info(8306):   cocos2d.x.compiled_with_gl_state_cache: true
05-10 17:50:52.613: D/cocos2d-x debug info(8306):   gl.version: OpenGL ES 2.0
05-10 17:50:52.613: D/cocos2d-x debug info(8306):   gl.supports_NPOT: true
05-10 17:50:52.613: D/cocos2d-x debug info(8306):   gl.max_texture_units: 32
05-10 17:50:52.613: D/cocos2d-x debug info(8306):   gl.vendor: Hisilicon Technologies
05-10 17:50:52.613: D/cocos2d-x debug info(8306):   gl.max_texture_size: 8192
05-10 17:50:52.613: D/cocos2d-x debug info(8306): }
05-10 17:50:52.613: V/Agoo(8468): agoo_logger_path[/storage/emulated/0/_agoo_log_bin_]
05-10 17:50:52.613: D/ControlService(8468): onHandleIntent [com.insul.poker][com.insul.poker.intent.action.COMMAND]
05-10 17:50:52.617: D/ControlService(8468): command --->[register]
05-10 17:50:52.777: D/dalvikvm(8306): GC_CONCURRENT freed 922K, 13% free 11959K/13672K, paused 9ms+16ms, total 52ms
05-10 17:50:52.797: D/SocializeNetUtils(8306): ##### 未加密参数 : dc=com.umeng.share&dt=1462873852724&os=Android&md5imei=1DC09D6957CE30726F78AC5D68B2C93E&tp=1&imei=860463020974191&opid=1&mac=88%3Ae3%3Aab%3A7d%3A45%3Ac2&ak=55d7cc9c67e58eed9100790a&de=HUAWEI+P6-T00&sdkv=5.0.2&pcv=2.0&en=Wi-Fi&ni=0
05-10 17:50:52.817: D/SocializeNetUtils(8306): #### 完整请求链接 : http://log.umsns.com/bar/get/55d7cc9c67e58eed9100790a/?ud_get=7hDBcj4Pw0T/w2Ec9yZbG0hd1DknOXrfLL0zyJ456hmKSyOFwLnnGAW7JFswGleiMfBaoiLuJki6ONrpTQfmYyeF6uaebGtVc1yA+Ov8jdvC5sSZdd37MgEujnQYkj8OPWL71Rp7VDPIBocUPd85lxUNofpTlxNhbskh8KACHfHx2FxNO7fesvYsGCtDJlijZSfp0slIKOgx4v8k1JLhej/2tUZwD3mSwEbOGemktstXcpB6uGqJzXjhwrjruKYJVFzDeI3Y0H6f5mttaJY3Vyi+I7y5LuM1VXLsjfmoj17vyyGn2OpxtvlT82JeEXMT
05-10 17:50:52.820: E/xxxxx(8306):  stringcom.umeng.socialize.net.ActionBarRequest@42a3e488
05-10 17:50:52.820: E/xxxxx(8306):  stringorg.apache.http.client.methods.HttpGet@42a0ba38
05-10 17:50:52.830: I/MID(8306): read mid from Settings.System
05-10 17:50:52.833: V/ControlService(8468): handleRegister[com.insul.poker]--->[deviceToken==null][retty:false]
05-10 17:50:52.940: D/ControlService(8468): doRegister app_version=1.0
05-10 17:50:53.060: E/MtaSDK(8306): [pool-5-thread-1(342): null:-1] - Server response error code:404, error:{"ret":-1, "msg":"invalid appkey"}
05-10 17:50:53.110: V/MicroMsg.SDK.WXApiImplV10.ActivityLifecycleCb(8306): WXStat trigger onForeground
05-10 17:50:53.180: I/UClient(8306): 596  Use GZIPInputStream get data....
05-10 17:50:53.207: E/xxxxx(8306):  json{"msg": "no error", "data": {"fr": "0", "pv": 1, "uid": "1dc09d6957ce30726f78ac5d68b2c93e", "cm": 0, "lk": 0, "ek": "e240ef7072a1a2516c3ed74e283ec73d", "st": "1", "sn": 174, "sid": "053e5c211d64cd631ea9deb84c15d06a", "ft": "0"}, "st": 200}
05-10 17:50:53.213: D/cocos2d-x debug info(8306): create rendererRecreatedListener for GLProgramState
05-10 17:50:53.233: D/umeng_share_response(8306): is http 200:true
05-10 17:50:53.240: D/httpClient(8468): isRedirectRequested[Server==nginxDate==Tue, 10 May 2016 09:50:53 GMTContent-Type==application/json;charset=UTF-8Content-Length==182Connection==keep-aliveMres-length==182m-retcode==SUCCESSx-paramkey==mtop.push.device.createAndRegisters-rt==9]
05-10 17:50:53.243: D/SyncHttp.client(8468): request url:[http://api.m.taobao.com/rest/api3.do?ttid=android%40umeng&t=1462873852&imei=umeng&appKey=umeng%3A55d7cc9c67e58eed9100790a&v=4.0&sign=2d04786abaff9505583f7ae739bf614e&data=%7B%22new_device%22%3A%22true%22%2C%22c1%22%3A%22HUAWEI+P6-T00%22%2C%22c2%22%3A%22umeng%22%2C%22device_global_id%22%3A%22VutzrtGJR3kDAA6sLYLZAStr%22%2C%22c0%22%3A%22Huawei%22%2C%22c6%22%3A%221d4648fdf136d281%22%2C%22package_name%22%3A%22com.insul.poker%22%2C%22c5%22%3A%22022BTF7N41059031%22%2C%22app_version%22%3A%221.0%22%2C%22sdk_version%22%3A20151208%2C%22c4%22%3A%2288%3Ae3%3Aab%3A7d%3A45%3Ac2%22%2C%22c3%22%3A%22umeng%22%7D&api=mtop.push.device.createAndRegister&imsi=umeng] : result code [200]:[{"api":"mtop.push.device.createAndRegister","v":"4.0","ret":["SUCCESS::接口调用成功"],"data":{"device_id":"AiuFWFp14GTkb7pU23cNr4cfMCeIitnwXFK0FGK9yI8D","push_key":"4224bbc4"}}]
05-10 17:50:53.253: D/MtopResponseHelper(8468): MtopResponseHelper:[Result [isSuccess=true, httpCode=200, data={"push_key":"4224bbc4","device_id":"AiuFWFp14GTkb7pU23cNr4cfMCeIitnwXFK0FGK9yI8D"}, retDesc=接口调用成功, retCode=SUCCESS, headers={}]]
05-10 17:50:53.300: I/umengsocial(8306): response: no error
05-10 17:50:53.300: I/umengsocial(8306): response has error: no error
05-10 17:50:53.313: W/System.err(8468): Invalid int: ""
05-10 17:50:53.317: D/ControlService(8468): register--->[result:{"push_key":"4224bbc4","device_id":"AiuFWFp14GTkb7pU23cNr4cfMCeIitnwXFK0FGK9yI8D"}]
05-10 17:50:53.383: D/com.umeng.message.UmengBaseIntentService(8468): callAgooElectionReceiver
05-10 17:50:53.403: E/MtaSDK(8306): [pool-5-thread-1(342): null:-1] - Server response error code:404, error:{"ret":-1, "msg":"invalid appkey"}
05-10 17:50:53.410: D/ElectionService(8468): registerApp save[{"com.insul.poker":"{\"appInstallTime\":1462873845444,\"appSdkVersion\":20151208,\"appVersionHash\":46670518,\"appKey\":\"umeng:55d7cc9c67e58eed9100790a\"}"}]
05-10 17:50:53.510: D/cocos2d-x debug info(8306): jni-java函数执行完毕,得到的stsring：
05-10 17:50:53.510: D/cocos2d-x debug info(8306): jni-java函数执行完毕,得到的stsring：
05-10 17:50:53.527: D/cocos2d-x debug info(8306): open file /data/data/com.insul.poker/files/account.json failed
05-10 17:50:53.527: I/lzf(8306): gethostbyname normal
05-10 17:50:53.540: D/cocos2d-x debug info(8306): net_interface2::connect startup 2013
05-10 17:50:53.540: D/cocos2d-x debug info(8306): 连接索引：2013
05-10 17:50:53.580: D/com.umeng.message.RegistrationReceiver(8306): RegistrationReceiver
05-10 17:50:53.583: D/BaseIntentService(8306): runIntent to receive one messageIntent { act=com.insul.poker.intent.action.COMMAND flg=0x10 pkg=com.insul.poker cmp=com.insul.poker/com.umeng.message.RegistrationReceiver (has extras) }|com.umeng.message.UmengIntentService
05-10 17:50:53.583: V/BaseIntentService(8468): auto update [updateSwitchConfig]
05-10 17:50:53.583: V/ProxyFactroy(8468): getInstance[base] successfully
05-10 17:50:53.680: D/httpClient(8468): isRedirectRequested[Server==nginxDate==Tue, 10 May 2016 09:50:53 GMTContent-Type==application/json;charset=UTF-8Content-Length==90Connection==keep-alivex-paramkey==mtop.wswitch.syncconfiggroups-rt==2]
05-10 17:50:53.683: D/SyncHttp.client(8468): request url:[http://api.m.taobao.com/rest/api3.do?t=1462873853&deviceId=AiuFWFp14GTkb7pU23cNr4cfMCeIitnwXFK0FGK9yI8D&imei=umeng&appKey=umeng%3A55d7cc9c67e58eed9100790a&v=1.0&sign=7b29fc9cf63964036f351db2fb0c5bf9&data=%7B%22configName%22%3A%22client_sdk_switch%22%7D&api=mtop.wswitch.syncconfiggroup&imsi=umeng] : result code [200]:[{"api":"mtop.wswitch.syncconfiggroup","v":"1.0","ret":["FAIL::接口不存在"],"data":{}}]
05-10 17:50:53.683: D/MtopResponseHelper(8468): MtopResponseHelper:[Result [isSuccess=false, httpCode=200, data={}, retDesc=接口不存在, retCode=FAIL, headers={}]]
05-10 17:50:53.707: V/ProxyFactroy(8468): getInstance[base] successfully
05-10 17:50:53.707: D/ControlService(8468): onHandleIntent [com.insul.poker][com.insul.poker.intent.action.COMMAND]
05-10 17:50:53.707: D/ControlService(8468): command --->[registration]
05-10 17:50:53.710: D/com.umeng.message.UmengBaseIntentService(8468): onRegistered()[AiuFWFp14GTkb7pU23cNr4cfMCeIitnwXFK0FGK9yI8D]
05-10 17:50:53.710: D/com.umeng.message.MessageSharedPrefs(8468): Constructor()
05-10 17:50:53.753: E/com.umeng.message.UTrack(8468): RegistrationId is empty
05-10 17:50:53.763: D/com.umeng.message.UmengMessageCallbackHandlerService(8306): processName=com.insul.poker
05-10 17:50:53.777: I/ElectionService(8468): checkPackage[pack:com.insul.poker][enabled]
05-10 17:50:53.777: D/ElectionService(8468): getCurrentSudo[currentSudo:com.insul.poker]
05-10 17:50:53.780: D/ElectionService(8468): doElection[targetPackage:com.insul.poker]
05-10 17:50:53.787: D/com.umeng.message.UmengMessageCallbackHandlerService(8306): enable(): register-->:AiuFWFp14GTkb7pU23cNr4cfMCeIitnwXFK0FGK9yI8D
05-10 17:50:53.787: E/==device_token==(8306): AiuFWFp14GTkb7pU23cNr4cfMCeIitnwXFK0FGK9yI8D
05-10 17:50:53.787: E/==device_token==(8306): sendDeviceToken
05-10 17:50:53.787: D/cocos2d-x debug info(8306): Java_com_insul_poker_AppActivity_sendDeviceToken
05-10 17:50:53.787: D/cocos2d-x debug info(8306): AiuFWFp14GTkb7pU23cNr4cfMCeIitnwXFK0FGK9yI8D
05-10 17:50:53.793: I/ElectionService(8468): agoo client packs==>[{"com.insul.poker":"{\"appInstallTime\":1462873845444,\"appSdkVersion\":20151208,\"appVersionHash\":46670518,\"appKey\":\"umeng:55d7cc9c67e58eed9100790a\"}"}]
05-10 17:50:53.797: I/ElectionService(8468): checkPackage[pack:com.insul.poker][enabled]
05-10 17:50:53.800: I/ElectionService(8468): application[pack:com.insul.poker]--->[agooVersion:20151208][enabled]
05-10 17:50:53.800: I/ElectionService(8468): resetAgooPacks--->[{"com.insul.poker":"{\"appInstallTime\":1462873845444,\"appSdkVersion\":20151208,\"appVersionHash\":46670518,\"appKey\":\"umeng:55d7cc9c67e58eed9100790a\"}"}]
05-10 17:50:53.803: D/ElectionService(8468): election application packs[{com.insul.poker=20151208}]
05-10 17:50:53.843: D/cocos2d-x debug info(8306): cocos2d: fullPathForFilename: No file found at Helvetica. Possible missing file.
05-10 17:50:53.850: D/cocos2d-x debug info(8306): cocos2d: fullPathForFilename: No file found at Helvetica. Possible missing file.
05-10 17:50:53.863: D/cocos2d-x debug info(8306): cocos2d: fullPathForFilename: No file found at Helvetica. Possible missing file.
05-10 17:50:53.867: D/cocos2d-x debug info(8306): cocos2d: fullPathForFilename: No file found at Helvetica. Possible missing file.
05-10 17:50:53.880: D/cocos2d-x debug info(8306): cocos2d: fullPathForFilename: No file found at Thonburi. Possible missing file.
05-10 17:50:53.883: D/cocos2d-x debug info(8306): cocos2d: fullPathForFilename: No file found at Thonburi. Possible missing file.
05-10 17:50:53.893: D/cocos2d-x debug info(8306): cocos2d: fullPathForFilename: No file found at Thonburi. Possible missing file.
05-10 17:50:53.907: D/cocos2d-x debug info(8306): cocos2d: fullPathForFilename: No file found at Thonburi. Possible missing file.
05-10 17:50:53.910: D/cocos2d-x debug info(8306): cocos2d: fullPathForFilename: No file found at Thonburi. Possible missing file.
05-10 17:50:53.913: D/cocos2d-x debug info(8306): cocos2d: fullPathForFilename: No file found at Thonburi. Possible missing file.
05-10 17:50:53.920: D/httpClient(8468): isRedirectRequested[Server==nginxDate==Tue, 10 May 2016 09:50:53 GMTContent-Type==application/json;charset=UTF-8Content-Length==140Connection==keep-aliveMres-length==140m-retcode==SUCCESSx-paramkey==mtop.push.channel.votes-rt==3]
05-10 17:50:53.923: D/SyncHttp.client(8468): request url:[http://api.m.taobao.com/rest/api3.do?ttid=android%40umeng&t=1462873853&deviceId=AiuFWFp14GTkb7pU23cNr4cfMCeIitnwXFK0FGK9yI8D&imei=umeng&appKey=umeng%3A55d7cc9c67e58eed9100790a&v=7.0&sign=d64e8f3210dfb2cface0083bd35a7d44&data=%7B%22vote_factors%22%3A%22%7B%5C%22com.insul.poker%5C%22%3A20151208%7D%22%7D&api=mtop.push.channel.vote&imsi=umeng] : result code [200]:[{"api":"mtop.push.channel.vote","v":"7.0","ret":["SUCCESS::调用成功"],"data":{"sudo_pack":"com.insul.poker","time_out":"1462957871239"}}]
05-10 17:50:53.923: D/MtopResponseHelper(8468): MtopResponseHelper:[Result [isSuccess=true, httpCode=200, data={"sudo_pack":"com.insul.poker","time_out":"1462957871239"}, retDesc=调用成功, retCode=SUCCESS, headers={}]]
05-10 17:50:53.923: D/cocos2d-x debug info(8306): local_string
05-10 17:50:53.923: D/ElectionService(8468): remoteElection--->[result:{"sudo_pack":"com.insul.poker","time_out":"1462957871239"}]
05-10 17:50:53.923: D/ElectionService(8468): remoteElection--->[result11:{"sudo_pack":"com.insul.poker","time_out":"1462957871239"}]
05-10 17:50:53.923: D/cocos2d-x debug info(8306): language_path.......:assets/client_language_zh.json
05-10 17:50:53.923: D/cocos2d-x debug info(8306): language_path:assets/client_language_zh.json
05-10 17:50:53.923: D/ElectionService(8468): remoteElection--->[sudo_pack:com.insul.poker]
05-10 17:50:53.930: D/cocos2d-x debug info(8306): local_string
05-10 17:50:53.930: D/cocos2d-x debug info(8306): local_string
05-10 17:50:53.930: I/ElectionService(8468): checkPackage[pack:com.insul.poker][enabled]
05-10 17:50:53.930: D/ElectionService(8468): getCurrentSudo[currentSudo:com.insul.poker]
05-10 17:50:53.933: D/ElectionService(8468): noticeElectionResult[lastSudoPack:com.insul.poker]
05-10 17:50:53.933: D/ElectionService(8468): noticeElectionResult[sudoPack:com.insul.poker][timeout:1462957871239][electionSource:remote]
05-10 17:50:53.933: E/(8306): scaleX = 0.960000
05-10 17:50:53.937: D/cocos2d-x debug info(8306): Wraning! You can't change Andriod Hint fontName and fontSize
05-10 17:50:53.937: D/cocos2d-x debug info(8306): [Edit text] content size = (470.000000, 84.000000)
05-10 17:50:53.937: D/cocos2d-x debug info(8306): [Edit text] content size = (470.000000, 84.000000)
05-10 17:50:53.937: D/cocos2d-x debug info(8306): Wraning! You can't change Andriod Hint fontName and fontSize
05-10 17:50:53.937: D/cocos2d-x debug info(8306): local_string
05-10 17:50:53.940: E/(8306): scaleX = 0.960000
05-10 17:50:53.940: D/cocos2d-x debug info(8306): Wraning! You can't change Andriod Hint fontName and fontSize
05-10 17:50:53.940: D/cocos2d-x debug info(8306): [Edit text] content size = (470.000000, 84.000000)
05-10 17:50:53.940: D/cocos2d-x debug info(8306): [Edit text] content size = (470.000000, 84.000000)
05-10 17:50:53.940: D/cocos2d-x debug info(8306): Wraning! You can't change Andriod Hint fontName and fontSize
05-10 17:50:53.940: D/cocos2d-x debug info(8306): local_string
05-10 17:50:53.940: D/cocos2d-x debug info(8306): account:, 
05-10 17:50:53.990: D/com.umeng.message.ElectionReceiver(8468): ElectionReceiver
05-10 17:50:53.990: D/ElectionService(8468): this election sudupack:com.insul.poker,action=org.agoo.android.intent.action.ELECTION_RESULT_V4
05-10 17:50:53.990: D/BaseIntentService(8468): runIntent to receive one messageIntent { act=org.agoo.android.intent.action.ELECTION_RESULT_V4 cat=[umeng] flg=0x30 pkg=com.insul.poker cmp=com.insul.poker/com.umeng.message.ElectionReceiver (has extras) }|com.umeng.message.UmengIntentService
05-10 17:50:53.993: D/ElectionService(8468): start to service...
05-10 17:50:53.993: D/ElectionService(8468): this message pack:com.insul.poker
05-10 17:50:53.997: D/ElectionService(8468): start service ret:true
05-10 17:50:54.010: D/ElectionService(8468): ElectionConnection conneted:ComponentInfo{com.insul.poker/com.umeng.message.UmengMessageIntentReceiverService}
05-10 17:50:54.010: D/ElectionService(8468): onConnected current tid:1
05-10 17:50:54.010: D/ElectionService(8468): ElectionConnection sent:Intent { act=org.agoo.android.intent.action.ELECTION_RESULT_V4 cat=[umeng] flg=0x20 pkg=com.insul.poker (has extras) }
05-10 17:50:54.010: D/ElectionService(8468): onConnected running tid:333
05-10 17:50:54.010: D/BaseIntentService(8468): runIntent to receive one messageIntent { act=org.agoo.android.intent.action.ELECTION_RESULT_V4 cat=[umeng] flg=0x20 pkg=com.insul.poker (has extras) }|com.umeng.message.UmengIntentService
05-10 17:50:54.013: D/ElectionService(8468): lastSudoPack=com.insul.poker
05-10 17:50:54.013: D/ElectionService(8468): !TextUtils.isEmpty(lastSudoPack)=true
05-10 17:50:54.013: D/ElectionService(8468): !TextUtils.equals(currentSudoPack, lastSudoPack)=false
05-10 17:50:54.023: D/GLSurfaceView(8306): HideSoftInput
05-10 17:50:54.023: V/ProxyFactroy(8468): getInstance[base] successfully
05-10 17:50:54.023: D/ControlService(8468): onHandleIntent [com.insul.poker][org.agoo.android.intent.action.ELECTION_RESULT_V4]
05-10 17:50:54.070: D/com.umeng.message.UmengBaseIntentService(8468): getAgooService
05-10 17:50:54.070: D/ControlService(8468): restart---->[currentSudoPack:com.insul.poker]:[start]
05-10 17:50:54.070: D/ServiceUtil(8468): command --->[com.insul.poker.service]:[start]
05-10 17:50:54.077: V/ProxyFactroy(8468): getInstance[base] successfully
05-10 17:50:54.077: V/ProxyFactroy(8468): getInstance[base] successfully
05-10 17:50:54.077: D/ControlService(8468): onHandleIntent [com.insul.poker][org.agoo.android.intent.action.ELECTION_RESULT_V4]
05-10 17:50:54.077: D/ControlService(8468): handleElection---->[noticeResult is true, result has came]
05-10 17:50:54.087: D/AgooService(8468): pushService==[org.android.agoo.impl.PushService]
05-10 17:50:54.087: W/PushService(8468): >>> agoo system is creating >>>
05-10 17:50:54.087: W/a(8468): /data/data/com.insul.poker/files/agoo.pid
05-10 17:50:54.093: W/PushService(8468): create currentPack=com.insul.poker
05-10 17:50:54.100: I/ElectionService(8468): checkPackage[pack:com.insul.poker][enabled]
05-10 17:50:54.100: D/ElectionService(8468): getCurrentSudo[currentSudo:com.insul.poker]
05-10 17:50:54.100: W/PushService(8468): getCurrentSudo(context)com.insul.poker
05-10 17:50:54.107: I/ElectionService(8468): checkPackage[pack:com.insul.poker][enabled]
05-10 17:50:54.107: D/ElectionService(8468): getCurrentSudo[currentSudo:com.insul.poker]
05-10 17:50:54.107: W/PushService(8468): com.insul.poker,  true
05-10 17:50:54.107: D/com.umeng.message.proguard.F(8468): api level is:17
05-10 17:50:54.107: W/com.umeng.message.proguard.F(8468): time is daytime, wakeup cpu for keeping connecntion
05-10 17:50:54.110: W/com.umeng.message.proguard.F(8468): open assets from = armeabi/DaemonServer
05-10 17:50:54.110: D/com.umeng.message.proguard.F(8468): >>>soDataSize:datasize:18080
05-10 17:50:54.133: D/com.umeng.message.proguard.F(8468): >>>soDataSize:13560
05-10 17:50:54.147: W/TAG.(8468): chmod 500 /data/data/com.insul.poker/files/DaemonServer
05-10 17:50:54.187: W/TAG.(8468): /data/data/com.insul.poker/files/DaemonServer -s "/data/data/com.insul.poker/lib/" -n "runServer" -p "startservice -n com.insul.poker/com.umeng.message.UmengService --es cockroach cockroach-PPreotect --es pack com.insul.poker --user 0" -f "/data/data/com.insul.poker" -t "600" -c "agoo.pid" -P /data/data/com.insul.poker -K 1009527 -U tb_android_daemon_1.1.0 -L http://agoodm.m.taobao.com/agoo/report -D %7B%22package%22%3A%22com.insul.poker%22%2C%22appKey%22%3A%22umeng%3A55d7cc9c67e58eed9100790a%22%2C%22utdid%22%3A%22VutzrtGJR3kDAA6sLYLZAStr%22%2C%22sdkVersion%22%3A%2220151208%22%7D -I agoodm.m.taobao.com -O 80 -T -Z 
05-10 17:50:54.217: W/com.umeng.message.proguard.F(8468): /data/data/com.insul.poker/files/DaemonServer -s "/data/data/com.insul.poker/lib/" -n "runServer" -p "startservice -n com.insul.poker/com.umeng.message.UmengService --es cockroach cockroach-PPreotect --es pack com.insul.poker --user 0" -f "/data/data/com.insul.poker" -t "600" -c "agoo.pid" -P /data/data/com.insul.poker -K 1009527 -U tb_android_daemon_1.1.0 -L http://agoodm.m.taobao.com/agoo/report -D %7B%22package%22%3A%22com.insul.poker%22%2C%22appKey%22%3A%22umeng%3A55d7cc9c67e58eed9100790a%22%2C%22utdid%22%3A%22VutzrtGJR3kDAA6sLYLZAStr%22%2C%22sdkVersion%22%3A%2220151208%22%7D -I agoodm.m.taobao.com -O 80 -T -Z 
05-10 17:50:54.233: I/ElectionService(8468): checkPackage[pack:com.insul.poker][enabled]
05-10 17:50:54.233: D/ElectionService(8468): getCurrentSudo[currentSudo:com.insul.poker]
05-10 17:50:54.233: D/PushService(8468): hasComeFromCock[cockroach==null]or[cockroach!=null]
05-10 17:50:54.237: D/PushService(8468): registerNotKill--->[current-thread-name:se-service][action:com.insul.poker.intent.action.COCKROACH]
05-10 17:50:54.237: D/PushService(8468): action [com.insul.poker.intent.action.START]
05-10 17:50:54.240: D/C(8468): MessagePush [starting]
05-10 17:50:54.247: V/C(8468): reConnect[mLastConnectTime:-1]<[currentTime:1462873854249]
05-10 17:50:54.247: D/C(8468): forceConnect[interval:2000][connectContext:init_connect_2695]
05-10 17:50:54.247: V/AbsMessage(8468): stopTask action[agoo_action_heart]
05-10 17:50:54.250: D/AbsMessage(8468): startTaskAtDelayed[agoo_action_re_connect][init_connect_2695]
05-10 17:50:54.257: D/PushService(8468): election next time[current-thread-name:se-service][2016-05-11 17:11:11][timeout:1462957871239] 
05-10 17:50:54.260: D/ReceiverUtil(8468): PushService is failed ....................
05-10 17:50:54.270: D/dalvikvm(8468): GC_CONCURRENT freed 1606K, 18% free 11357K/13752K, paused 2ms+3ms, total 56ms
05-10 17:50:54.277: D/cocos2d-x debug info(8306): AppDelegate::applicationScreenSizeChanged:720, 1232
05-10 17:50:54.290: D/C(8468): NewMessagePush connect is change.....current connect=android.net.conn.CONNECTIVITY_CHANGE
05-10 17:50:54.323: D/cocos2d-x debug info(8306): [Edit text] content size = (470.000000, 84.000000)
05-10 17:50:54.323: D/cocos2d-x debug info(8306): [Edit text] content size = (470.000000, 84.000000)
05-10 17:50:54.327: D/cocos2d-x debug info(8306): cocos2d: fullPathForFilename: No file found at Helvetica. Possible missing file.
05-10 17:50:54.337: D/C(8468): [network connected success]state[isconnected]
05-10 17:50:54.337: V/C(8468): reConnect[interval:1462873856338]>[1462873856249][state:network_error_connect_8695]
05-10 17:50:54.343: D/cocos2d-x debug info(8306): cocos2d: fullPathForFilename: No file found at Helvetica. Possible missing file.
05-10 17:50:54.367: D/cocos2d-x debug info(8306): jni-java函数执行完毕,得到的stsring：
05-10 17:50:54.367: D/cocos2d-x debug info(8306): jni-java函数执行完毕,得到的stsring：
05-10 17:50:54.367: D/cocos2d-x debug info(8306): ----- android 0 --------
05-10 17:50:54.367: D/cocos2d-x debug info(8306): ----- android 0_1 --------
05-10 17:50:54.427: A/libc(8306): Fatal signal 11 (SIGSEGV) at 0xdeadbaad (code=1), thread 8459 (Thread-326)
05-10 17:50:56.267: D/C(8468): onHandlerConnect spdyCount=0,httpCount=0
05-10 17:50:56.267: V/C(8468): onHandlerConnect refreshhost[true]state[init_connect_2695][threadName:IntentService[timeThread]]
05-10 17:50:56.293: D/DNSManager(8468): apollUrlhttp://apoll.m.taobao.com/spdyip/
05-10 17:50:56.397: D/httpClient(8468): isRedirectRequested[Server==nginxDate==Tue, 10 May 2016 09:50:56 GMTContent-Length==37Connection==keep-alive]
05-10 17:50:56.397: D/SyncHttp.client(8468): request url:[http://apoll.m.taobao.com/spdyip/?appkey=umeng%3A55d7cc9c67e58eed9100790a&ttid=android%40umeng&deviceId=AiuFWFp14GTkb7pU23cNr4cfMCeIitnwXFK0FGK9yI8D&imei=umeng&nt=wifi&app_version_code=1.0&agoo_version_code=20151208&imsi=umeng] : result code [200]:[140.205.203.78:80|140.205.203.78:8013]
05-10 17:50:56.400: D/DNSManager(8468):  initChannel[spdy]--> resultChannel[spdy]
05-10 17:50:56.467: W/System.err(8468): java.lang.UnsatisfiedLinkError: Couldn't load tnet-2.1.20.1-agoo from loader dalvik.system.PathClassLoader[dexPath=/data/app/com.insul.poker-1.apk,libraryPath=/data/app-lib/com.insul.poker-1]: findLibrary returned null
05-10 17:50:56.470: W/System.err(8468):     at java.lang.Runtime.loadLibrary(Runtime.java:365)
05-10 17:50:56.470: W/System.err(8468):     at java.lang.System.loadLibrary(System.java:535)
05-10 17:50:56.473: W/System.err(8468):     at org.android.spdy.SoInstallMgrSdk.initSo(SoInstallMgrSdk.java:86)
05-10 17:50:56.473: W/System.err(8468):     at org.android.spdy.SoInstallMgrSdk.initSo(SoInstallMgrSdk.java:62)
05-10 17:50:56.473: W/System.err(8468):     at org.android.spdy.SpdyAgent.<init>(SpdyAgent.java:115)
05-10 17:50:56.473: W/System.err(8468):     at org.android.spdy.SpdyAgent.getInstance(SpdyAgent.java:91)
05-10 17:50:56.473: W/System.err(8468):     at org.android.agoo.net.channel.spdy.SpdyChannel.<init>(SpdyChannel.java:65)
05-10 17:50:56.473: W/System.err(8468):     at org.android.agoo.net.channel.ChannelManager.g(ChannelManager.java:359)
05-10 17:50:56.473: W/System.err(8468):     at org.android.agoo.net.channel.ChannelManager.e(ChannelManager.java:438)
05-10 17:50:56.473: W/System.err(8468):     at org.android.agoo.net.channel.ChannelManager.connenct(ChannelManager.java:407)
05-10 17:50:56.473: W/System.err(8468):     at org.android.agoo.net.channel.ChannelManager$1.onHost(ChannelManager.java:76)
05-10 17:50:56.473: W/System.err(8468):     at org.android.agoo.net.channel.DNSManager.a(DNSManager.java:530)
05-10 17:50:56.477: W/System.err(8468):     at org.android.agoo.net.channel.DNSManager.b(DNSManager.java:388)
05-10 17:50:56.477: W/System.err(8468):     at org.android.agoo.net.channel.DNSManager.b(DNSManager.java:31)
05-10 17:50:56.477: W/System.err(8468):     at org.android.agoo.net.channel.DNSManager$a.run(DNSManager.java:160)
05-10 17:50:56.480: W/System.err(8468):     at java.lang.Thread.run(Thread.java:856)
05-10 17:50:56.480: W/dalvikvm(8468): No implementation found for native Lorg/android/spdy/SpdyAgent;.initAgent:(III)J
05-10 17:50:56.480: W/System.err(8468): java.lang.UnsatisfiedLinkError: Native method not found: org.android.spdy.SpdyAgent.initAgent:(III)J
05-10 17:50:56.480: W/System.err(8468):     at org.android.spdy.SpdyAgent.initAgent(Native Method)
05-10 17:50:56.483: W/System.err(8468):     at org.android.spdy.SpdyAgent.<init>(SpdyAgent.java:121)
05-10 17:50:56.483: W/System.err(8468):     at org.android.spdy.SpdyAgent.getInstance(SpdyAgent.java:91)
05-10 17:50:56.483: W/System.err(8468):     at org.android.agoo.net.channel.spdy.SpdyChannel.<init>(SpdyChannel.java:65)
05-10 17:50:56.487: W/System.err(8468):     at org.android.agoo.net.channel.ChannelManager.g(ChannelManager.java:359)
05-10 17:50:56.487: W/System.err(8468):     at org.android.agoo.net.channel.ChannelManager.e(ChannelManager.java:438)
05-10 17:50:56.487: W/System.err(8468):     at org.android.agoo.net.channel.ChannelManager.connenct(ChannelManager.java:407)
05-10 17:50:56.487: W/System.err(8468):     at org.android.agoo.net.channel.ChannelManager$1.onHost(ChannelManager.java:76)
05-10 17:50:56.490: W/System.err(8468):     at org.android.agoo.net.channel.DNSManager.a(DNSManager.java:530)
05-10 17:50:56.490: W/System.err(8468):     at org.android.agoo.net.channel.DNSManager.b(DNSManager.java:388)
05-10 17:50:56.493: W/System.err(8468):     at org.android.agoo.net.channel.DNSManager.b(DNSManager.java:31)
05-10 17:50:56.493: W/System.err(8468):     at org.android.agoo.net.channel.DNSManager$a.run(DNSManager.java:160)
05-10 17:50:56.493: W/System.err(8468):     at java.lang.Thread.run(Thread.java:856)
05-10 17:50:56.493: V/ChannelManager(8468): time[1462873856497]mClientCreateTime[1462873856497]mLastReconnectTime[1462873856497]
05-10 17:50:56.530: D/signHelp(8468): getNetWork c=1|0|0|�%���
05-10 17:50:56.550: D/ChannelManager(8468): mCurrentChannelType = SPDY
05-10 17:50:56.550: D/ChannelManager(8468): connenct  [SpdyChannel]
05-10 17:50:56.587: D/SpdyClient(8468): connect url[http://140.205.203.78:80/m/umeng:55d7cc9c67e58eed9100790a/1/AiuFWFp14GTkb7pU23cNr4cfMCeIitnwXFK0FGK9yI8D?s=MDUxMzUzOTQ1NmVjUmxXbHY2UGNva0h3UmJhb25xZjcvSU5uOD0=&c=MXwwfDB877%2B9Je%2B%2Fve%2B%2Fve%2B%2FvXwxfDF8MDUxMzUzOTQ1NXwwNTEzNTM5NDU1fDF8MQ%3D%3D&pm=SFVBV0VJIFA2LVQwMA%3D%3D&sv=20151208&ov=4.2.2]
05-10 17:50:56.590: D/SpdyClient(8468): header--->[hb:300]
05-10 17:50:56.590: W/dalvikvm(8468): No implementation found for native Lorg/android/spdy/SpdyAgent;.createSession:(JLorg/android/spdy/SpdySession;[BC[BC[B[BLjava/lang/Object;II)J
05-10 17:50:56.593: D/C(8468): mPushHandler onError............error=1207
05-10 17:50:56.593: D/C(8468): connect [onDisconnected] [init_connect_2695] sessionId[-1]tcpKeepaliveInterval [1462873856598]
05-10 17:50:56.597: W/C(8468): handlerError[init_connect_2695]
05-10 17:50:56.597: W/C(8468): handlerError[1207]
05-10 17:50:56.600: W/C(8468): handlerError
05-10 17:50:56.600: W/C(8468): java.lang.UnsatisfiedLinkError: Native method not found: org.android.spdy.SpdyAgent.createSession:(JLorg/android/spdy/SpdySession;[BC[BC[B[BLjava/lang/Object;II)J
05-10 17:50:56.600: W/C(8468):  at org.android.spdy.SpdyAgent.createSession(Native Method)
05-10 17:50:56.600: W/C(8468):  at org.android.spdy.SpdyAgent.createSession(SpdyAgent.java:339)
05-10 17:50:56.600: W/C(8468):  at org.android.spdy.SpdyAgent.submitRequest(SpdyAgent.java:438)
05-10 17:50:56.600: W/C(8468):  at org.android.agoo.net.channel.spdy.SpdyChannel.connect(SpdyChannel.java:117)
05-10 17:50:56.600: W/C(8468):  at org.android.agoo.net.channel.ChannelManager.e(ChannelManager.java:501)
05-10 17:50:56.600: W/C(8468):  at org.android.agoo.net.channel.ChannelManager.connenct(ChannelManager.java:407)
05-10 17:50:56.600: W/C(8468):  at org.android.agoo.net.channel.ChannelManager$1.onHost(ChannelManager.java:76)
05-10 17:50:56.600: W/C(8468):  at org.android.agoo.net.channel.DNSManager.a(DNSManager.java:530)
05-10 17:50:56.600: W/C(8468):  at org.android.agoo.net.channel.DNSManager.b(DNSManager.java:388)
05-10 17:50:56.600: W/C(8468):  at org.android.agoo.net.channel.DNSManager.b(DNSManager.java:31)
05-10 17:50:56.600: W/C(8468):  at org.android.agoo.net.channel.DNSManager$a.run(DNSManager.java:160)
05-10 17:50:56.600: W/C(8468):  at java.lang.Thread.run(Thread.java:856)
05-10 17:50:56.603: V/AbsMessage(8468): stopTask action[agoo_action_re_connect]
05-10 17:50:56.603: V/AbsMessage(8468): stopTask action[agoo_action_heart]
05-10 17:50:56.610: W/C(8468): handlerError[force_chunked]
05-10 17:50:56.610: V/C(8468): reConnect[mLastConnectTime:1462873856249]<[currentTime:1462873856613]
05-10 17:50:56.610: D/C(8468): forceConnect[interval:2000][connectContext:error_connect_1775]
05-10 17:50:56.613: V/AbsMessage(8468): stopTask action[agoo_action_heart]
05-10 17:50:56.613: D/AbsMessage(8468): startTaskAtDelayed[agoo_action_re_connect][error_connect_1775]
05-10 17:50:59.213: D/C(8468): onHandlerConnect spdyCount=1,httpCount=0
05-10 17:50:59.213: V/C(8468): onHandlerConnect refreshhost[true]state[error_connect_1775][threadName:IntentService[timeThread]]
05-10 17:50:59.243: D/DNSManager(8468): currentChannleType[spdy]!=channelType[chunked]
05-10 17:50:59.243: D/DNSManager(8468): apollUrlhttp://apoll.m.taobao.com/activeip/
05-10 17:50:59.290: D/httpClient(8468): isRedirectRequested[Server==nginxDate==Tue, 10 May 2016 09:50:59 GMTContent-Length==41Connection==keep-alive]
05-10 17:50:59.290: D/SyncHttp.client(8468): request url:[http://apoll.m.taobao.com/activeip/?appkey=umeng%3A55d7cc9c67e58eed9100790a&ttid=android%40umeng&deviceId=AiuFWFp14GTkb7pU23cNr4cfMCeIitnwXFK0FGK9yI8D&imei=umeng&nt=wifi&app_version_code=1.0&agoo_version_code=20151208&imsi=umeng] : result code [200]:[140.205.203.108:8029|140.205.203.107:8029]
05-10 17:50:59.293: D/DNSManager(8468):  initChannel[chunked]--> resultChannel[chunked]
05-10 17:50:59.323: V/ChannelManager(8468): time[1462873859326]mClientCreateTime[1462873856497]mLastReconnectTime[1462873856497]
05-10 17:50:59.360: D/signHelp(8468): getNetWork c=1|0|0|�%���
05-10 17:50:59.367: D/ChannelManager(8468): mCurrentChannelType = CHUNKED
05-10 17:50:59.367: D/ChannelManager(8468): connenct  [ChunkedChannel]
05-10 17:50:59.390: D/HttpURLChunked(8468): http chunked connectId:[2410]==>http://140.205.203.108:8029/m/umeng:55d7cc9c67e58eed9100790a/1/AiuFWFp14GTkb7pU23cNr4cfMCeIitnwXFK0FGK9yI8D?s=MDUxMzUzOTQ1OGQzZUZYVEsraUZqQnpQa1Q0L1JVd0lLMEQ2UT0=&c=MXwwfDB877%2B9Je%2B%2Fve%2B%2Fve%2B%2FvXwwfDJ8MDUxMzUzOTQ1NnwwNTEzNTM5NDU2fDF8MQ%3D%3D&pm=SFVBV0VJIFA2LVQwMA%3D%3D&sv=20151208&ov=4.2.2
05-10 17:50:59.497: D/C(8468): onConnected [error_connect_1775]sessionId[2410]tcpConnectedInterval[50]header[{content-type=text/html;charset=UTF-8, connection=keep-alive, transfer-encoding=chunked, x-android-sent-millis=1462873859444, server=nginx, hb=300, date=Tue, 10 May 2016 09:50:59 GMT, x-android-received-millis=1462873859498}]
05-10 17:50:59.500: D/C(8468): onConnected initChannel[error_connect_1775][chunked]-->targetChannel[chunked]
05-10 17:50:59.500: D/AbsMessage(8468): startTaskAtDelayed[action_get_his_message]
05-10 17:50:59.513: D/C(8468): heart[onherror_connect_1775]heart--->[start checktime:210|timeout:330s]
05-10 17:50:59.513: D/AbsMessage(8468): startTaskRepeating[agoo_action_heart]
05-10 17:50:59.567: D/HttpChunked(8468): http msg is comming...........result=[]
05-10 17:50:59.570: D/C(8468): onData,mLastHeartTime--->[1462873859571]
05-10 17:50:59.570: D/C(8468): message--->[error_connect_1775][[]]
05-10 17:50:59.607: D/com.umeng.message.RegistrationReceiver(8547): RegistrationReceiver
05-10 17:50:59.613: D/com.umeng.message.MessageSharedPrefs(8547): Constructor()
05-10 17:50:59.623: D/BaseIntentService(8547): runIntent to receive one messageIntent { act=com.insul.poker.intent.action.COMMAND flg=0x10 pkg=com.insul.poker cmp=com.insul.poker/com.umeng.message.RegistrationReceiver (has extras) }|com.umeng.message.UmengIntentService
05-10 17:50:59.640: V/ProxyFactroy(8468): getInstance[base] successfully
05-10 17:50:59.640: D/ControlService(8468): onHandleIntent [com.insul.poker][com.insul.poker.intent.action.COMMAND]
05-10 17:50:59.643: D/ControlService(8468): command --->[error]
05-10 17:50:59.643: D/ControlService(8468): handleError:EVENT_CONNECTED
05-10 17:50:59.643: W/BaseIntentService(8468): callError[EVENT_CONNECTED]
05-10 17:50:59.660: D/com.umeng.message.UmengBaseIntentService(8468): onError()[EVENT_CONNECTED]
</code></pre>
** 查找问题
> 当看到
05-10 17:50:51.780: E/com.umeng.message.UmengRegistrar(8306): isRegisteredToUmeng: empty registration id
05-10 17:50:51.863: W/System.err(8306): Invalid int: ""
05-10 17:50:51.970: E/com.umeng.message.UTrack(8306): RegistrationId is empty
# 刚开始怀疑Umeng的消息推送在android端没处理好，于是找umeng的问题，在论坛找，用google、baidu。
发现找了一圈、各种尝试、还是crash.
于是打日志、代码屏蔽、发现还是崩溃，找了一下老版本的代码，发现代码写的一模一样，老代码没崩溃，新代码有问题。于是把查找中心放在新代码的c++代码socket协议里，
最后通过日志终于把问题定点到了protobuf的协议里相关文件，在Pocker的net_manager.cpp 里 send_data(...)方法里有一段：

    
    package.bodysize = processor.ByteSize();
    package.body = new char[package.bodysize];
    log("----- android 2 --------");
    processor.SerializeToArray(package.body, package.bodysize);  //崩溃的地方
    log("----- android 3 --------");
    


不太明白这个代码的意思，问了同事，这段代码意思是把数据放入数据流里，发送到服务器，服务器取出后再进行解析操作。在这链接中，
http://www.cnblogs.com/brainy/archive/2012/05/13/2498660.html 也有相关说明。

客户端发协议，服务器没收着~~，WHY？？？

#经痛苦的测试，发现上面的这三段代码造成下面崩溃的语句
05-10 17:50:54.427: A/libc(8306): Fatal signal 11 (SIGSEGV) at 0xdeadbaad (code=1), thread 8459 (Thread-326)
的crash，google发现很多人遇到过，多数是内存操作中经常引起的
http://stackoverflow.com/questions/23541535/fatal-signal-6-sigabrt-code-6-on-first-launch

由于是内存操作问题，于是调试
1、老代码的方法去写数据流
2、断点调试ios后进行比较
3、把new了内存，没delete内存的地方delete
4、把processor.SerializeToArray(package.body, package.bodysize)放到send_data(...)前执行
还是crash。
最后还是老大张驰发现了protobuf的上行协议写的有问题:

  
    //检查版本号
    void LoginManager::send_versioncheck()
    {
      msg::Response processor;
      net_manager::shared_instence().send_data(processor, REQ_VERSIONCHECK);
    }
    
    //protobuf协议字段
    message Response
    {
      required Status Status = 1;
      optional string Message = 2; //提示信息
    }
    

该上行协议中，默认定义一个局部 msg::Response processor; 发送到服务器就可以，不需要客户端传参数。
实际上Response中Status字段是required，必须传入参数！
但是IOS一直没任何问题（很诡异），也没任何人发现。。。。。。。
到Android移植才暴露出来。

** 解决问题
> 解决办法二选一：
1、加入processor.set_status(msg::Status::SUCCESS);
    //检查版本号
    void LoginManager::send_versioncheck()
    {
      msg::Response processor;
      processor.set_status(msg::Status::SUCCESS); //加入这句代码
      net_manager::shared_instence().send_data(processor, REQ_VERSIONCHECK);
    }

  
或者：
2、把 required 改成 optional
  message Response
    {
      optional Status Status = 1; //把 required 改成 optional
      optional string Message = 2; //提示信息
    }



重新编译打包运行.

fix bug !!





