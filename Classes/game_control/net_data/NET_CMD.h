//
//  NET_CMD.h
//  pokerclient
//
//  Created by MirrorMiko on 15/5/3.
//
//

#ifndef pokerclient_NET_CMD_h
#define pokerclient_NET_CMD_h

enum {
    //登录、充值
    REQ_VERSIONCHECK = 1,
    REQ_LOGIN = 99,	//登录
    REQ_SEND_VERIFICATION_BY_USER = 97, //用户名获取验证码
    REQ_RESET_PASSWORD = 98, //重置密码
	REQ_LOGIN_RESOURCES = 100,	//登录资源服务器
	REQ_SIGNAL_RESOURCE = 101,	//资源服务器心跳包
	REQ_OFFLINE = 102,	//注销
	REQ_FEED_BACK = 103, //反馈与举报
    REQ_REGISTER = 104,	//注册
    REQ_SEND_DEVICE_ID = 105,
    REQ_SEND_RECEIPT = 106, //发送购买凭证
    REQ_SEND_GET_GOODS_LIST = 107,//获取商品列表
    REQ_SEND_BUY_GOODS = 108,//购买商品
    REQ_SEND_ANDROID_CHARGE =109,//安卓充值
    //个人资料
    REQ_GET_USER_INFO = 110,	//获取玩家资料
	REQ_GET_USER_SETTING = 111,	//获取个人资料
	REQ_CHANGE_USER_SETTING = 112,	//修改个人资料
    REQ_GET_PERMISSION = 113,  //获取玩家信用值
    REQ_CHANGE_PERMISSION = 114,  //更改玩家信用值
    REQ_GET_GROUP_SETTING = 115, //获取俱乐部设置
    REQ_CHANGE_GROUP_SETTING =116,  //设置俱乐部推送
    REQ_SEND_VERIFICATION = 118, //请求验证码
    REQ_BIND_PHONE = 119, //绑定手机号
    
    //好友
    REQ_GET_FRIENDS_LIST=120, //刷新好友列表
    REQ_SEARCH_USER=121, //搜索玩家
    REQ_ADD_FRIEND=122, //添加好友
    RECEIVE_REQUEST_LIST=123, //下发请求添加好友请求列表
    REQ_DEAL_REQUEST=124, //处理添加好友请求
    REQ_DELETE_FRIEND=125, //删除好友
    REQ_SHIELD_FRIEND=126, //屏蔽好友
    REQ_PHONE_CONTACTS_LIST = 127, //添加手机联系人
    REQ_MOMENT_BLOCK_LIST = 128, //已屏蔽朋友圈列表
    REQ_GIFT_DIAMOUNT = 130, //赠送朋友钻石
	//俱乐部
	REQ_CREATE_GROUP = 140,	//创建俱乐部
	REQ_EDIT_GROUP = 141,	//编辑俱乐部
	REQ_SEARCH_GROUP = 142,	//请求俱乐部列表
	REQ_ENTER_GROUP = 143,	//加入俱乐部
	REQ_KICK_FROM_GROUP = 144,	//踢出玩家
	REQ_GET_GROUP_INFO = 145,	//俱乐部成员信息
    REQ_GET_GROUP_DETAIL_INFO = 146,	//俱乐部成员信息
    REQ_SET_GROUP_MANAGE_SETTING = 147,	//俱乐部成员信息
    REQ_SET_IS_ADMIN = 148,	//俱乐部成员信息
    REQ_GET_ROOM_INFO = 150,	//获取房间信息
    REQ_CHANGE_LANGUAGE_ROOM = 151,	//更改语言获取房间信息
    REQ_CREATE_ROOM = 152,	//创建房间
    REQ_CANCEL_MATCH = 154,  //取消比赛
	REQ_SEND_CHAT_MESSAGE = 160,	//新消息
	RECEIVE_CHAT_MESSAGE = 161,	//获取到新消息（先发下行后返回上行）
   
    REQ_SEND_JOIN = 181,// baoming
    RECEIVE_ENTER_MATCH = 182,//服务器下发，比赛开始
    REQ_PLYAER_HISTORY_SOCRE = 170, //玩家历史战绩
    REQ_GET_GROUP_USER_RANK = 171,  //获取俱乐部成员排行
    REQ_190 = 190,
    REQ_NOTIFY_CHANGE = 191,
	//朋友圈
	REQ_PUBLISH_MOMENT = 200, //上传朋友圈
	REQ_GET_MOMENT = 201,   //请求朋友圈
	REQ_DELETE_MOMENT = 202,  //删除信息
	REQ_PUBLISH_COMMENT = 203,  //评论朋友圈
	REQ_GET_COMMENT = 204,   //请求评论
    REQ_DELETE_COMMENT = 205,  //删除评论
    REQ_GET_LIKE = 206,  //获取点赞
    
//    IOS_PAY_LIST_REFRESH = 207,//苹果账单刷新 不与服务器交互
    
    //
    POKER_GET_HAND_HISTORY = 300,	//请求牌谱

	REQ_LOGIN_ROOM = 400,//登录房间服务器
	REQ_SIGNAL_ROOM = 401,	//房间服务器心跳包
    //游戏内协议
    REQ_ENTER_ROOM = 410,	//请求进入房间
    REQ_SEND_SEAT_INFO = 411,	//发送玩家座位属性
    RECEIVE_SEAT_INFO = 412,	//接收玩家座位属性（先发下行后返回上行）
    RECEIVE_GAME_START = 413,	//接收游戏开始（先发下行后返回上行）
    RECEIVE_GAME_END = 414,        //接收游戏结束
    REQ_DISABLE_AUTO_ACTION = 415, //解除托管
	RECEIVE_OTHER_PLAYER_ACTION = 417,	//接收其他玩家动作标志（先发下行后返回上行）
    REQ_GET_SCORE = 420,	//请求战绩

	REQ_SEND_GAME_VOICE = 430,
	RECEIVE_GAME_VOICE = 431,
    REQ_SET_GAME_STATUS = 440,  //调整游戏暂停或开始
    REQ_KICK_PLAYER = 441,  //踢人
    REQ_RECEIVE_GAME_STATUS = 442, //游戏状态 start or pause
	RECEIVE_ROOM_CLOSE = 490,   //  关闭房间
	RECEIVE_ROOM_MESSAGE = 491, //　告知客户端房间即将关闭

    POKER_REFRESH_ALL = 500,	//请求刷新所有数据
	POKER_REQUEST_PLYAER_ACTION = 501,	//接收玩家动作请求（先发下行后返回上行）
	POKER_SEND_PLAYER_ACTION= 502,	//发送玩家游戏动作
	POKER_RECEIVE_PROP_UPDATE = 503,	//接收游戏内属性更新（先发下行后返回上行）
    POKER_ADD_CHIPS = 510,	//补充计分牌
    POKER_RECEIVE_NEED_CHIP = 511,           //接收需要补充计分牌（先发下行后返回上行）
    POKER_REQUEST_CHOICE = 512,
    POKER_ANSWER_CHOICE = 513,
    POKER_RECEIVE_TABLE_NOTE = 514,
    
    POKER_SEND_SET_CHANGE = 515,	//发送玩家设置属性
    POKER_RECEIVE_SET_CHANGE = 516,
    POKER_SEND_SHOW_CARD = 517,  //发送玩家选择手牌
	POKER_RECEIVE_HAND_RESULT = 520,	//接收手牌结果（先发下行后返回上行）
//	POKER_GET_HAND_HISTORY = 521,	//请求牌谱
    

    POKER_TOURMENT_STATUS = 530, //
    REQ_SEND_TOURMENT_CHANGE_TABLE = 531,
    REQ_RECEIVE_TOURMENT_ADDON = 532,
    REQ_SEND_TOURMENT_ADDON = 533,
    
    //open face chinese poker
	OFC_REFRESH_ALL = 600, //刷新牌局信息
    OFC_RECEIVE_PROP_UPDATE = 601,     //服务器下发牌
	OFC_SEND_PLAYER_CARDS = 602,    //玩家选牌
    OFC_RECEIVE_PLAYERCARDS = 603,      //通知玩家牌型
	OFC_RECEIVE_SCORE = 604, //广播玩家积分
    OFC_RECEIVE_RESULT = 610,       //结果
    
    OFC_SEND_SET_CHANGE = 615,	//发送玩家设置属性
    OFC_RECEIVE_SET_CHANGE = 616,
    
    MAHJONG_REFRESH_ALL = 700, //刷新牌局信息
    MAHJONG_RECEIVE_PROP_UPDATE = 701,
    MAHJONG_SEND_PLAYER_CHOICE = 702,
    MAHJONG_RECEIVE_PLAYER_ACTION = 703,
    
    MAHJONG_SEND_SET_CHANGE = 715,	//发送玩家设置属性
    MAHJONG_RECEIVE_SET_CHANGE = 716,
    
};


#endif
