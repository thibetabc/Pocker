//
//  SendMsgToWeChatViewController.m
//  ApiClient
//
//  Created by Tencent on 12-2-27.
//  Copyright (c) 2012年 Tencent. All rights reserved.
//

#import "SendMsgToWeChatViewController.h"
#import "WXApiRequestHandler.h"
#import "WXApiManager.h"
#import "RespForWeChatViewController.h"
#import "Constant.h"

@interface SendMsgToWeChatViewController ()<WXApiManagerDelegate,UITextViewDelegate>

@property (nonatomic) enum WXScene currentScene;
@property (nonatomic, strong) NSString *appId;
@property (nonatomic, strong) UIScrollView *footView;

@end

@implementation SendMsgToWeChatViewController

@synthesize currentScene = _currentScene;
@synthesize appId = _appId;

#pragma mark - View Lifecycle
- (void)viewDidLoad {
    NSLog(@"enter");
    [super viewDidLoad];
    [self setupHeadView];
    [self setupLineView];
   // [self setupSceneView];
    [self setupFootView];
    [WXApiManager sharedManager].delegate = self;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
    } else {
        return YES;
    }
}

- (void)dealloc {
    [_appId release];
    [_footView release];
    [super dealloc];
}

#pragma mark - User Actions
- (void)onSelectSessionScene {
    _currentScene = WXSceneSession;
    
    UILabel *tips = (UILabel *)[self.view viewWithTag:TIPSLABEL_TAG];
    tips.text = @"分享场景:会话";
}

- (void)onSelectTimelineScene {
    _currentScene = WXSceneTimeline;
    
    UILabel *tips = (UILabel *)[self.view viewWithTag:TIPSLABEL_TAG];
    tips.text = @"分享场景:朋友圈";
}

- (void)onSelectFavoriteScene {
    _currentScene = WXSceneFavorite;
    
    UILabel *tips = (UILabel *)[self.view viewWithTag:TIPSLABEL_TAG];
    tips.text = @"分享场景:收藏";
}

- (void)sendTextContent {
    [WXApiRequestHandler sendText:kTextMessage
                          InScene:_currentScene];
}

- (void)sendImageContent {
    NSString *filePath = [[NSBundle mainBundle] pathForResource:@"res1" ofType:@"jpg"];
    NSData *imageData = [NSData dataWithContentsOfFile:filePath];
    
    UIImage *thumbImage = [UIImage imageNamed:@"res1thumb.png"];
    [WXApiRequestHandler sendImageData:imageData
                               TagName:kImageTagName
                            MessageExt:kMessageExt
                                Action:kMessageAction
                            ThumbImage:thumbImage
                               InScene:_currentScene];
}

- (void)sendLinkContent {
    UIImage *thumbImage = [UIImage imageNamed:@"res2.png"];
    [WXApiRequestHandler sendLinkURL:kLinkURL
                             TagName:kLinkTagName
                               Title:kLinkTitle
                         Description:kLinkDescription
                          ThumbImage:thumbImage
                             InScene:_currentScene];
}

- (void)sendMusicContent {
    UIImage *thumbImage = [UIImage imageNamed:@"res3.jpg"];
    [WXApiRequestHandler sendMusicURL:kMusicURL
                              dataURL:kMusicDataURL
                                Title:kMusicTitle
                          Description:kMusicDescription
                           ThumbImage:thumbImage
                              InScene:_currentScene];
}

- (void)sendVideoContent {
    UIImage *thumbImage = [UIImage imageNamed:@"res7.jpg"];
    [WXApiRequestHandler sendVideoURL:kVideoURL
                                Title:kVideoTitle
                          Description:kVideoDescription
                           ThumbImage:thumbImage
                              InScene:_currentScene];
}

- (void)sendAppContent {
    Byte* pBuffer = (Byte *)malloc(BUFFER_SIZE);
    memset(pBuffer, 0, BUFFER_SIZE);
    NSData* data = [NSData dataWithBytes:pBuffer length:BUFFER_SIZE];
    free(pBuffer);

    UIImage *thumbImage = [UIImage imageNamed:@"res2.jpg"];
    [WXApiRequestHandler sendAppContentData:data
                                    ExtInfo:kAppContentExInfo
                                     ExtURL:kAppContnetExURL
                                      Title:kAPPContentTitle
                                Description:kAPPContentDescription
                                 MessageExt:kAppMessageExt
                              MessageAction:kAppMessageAction
                                 ThumbImage:thumbImage
                                    InScene:_currentScene];
}

- (void)sendNonGifContent {
    NSString *filePath = [[NSBundle mainBundle] pathForResource:@"res5"
                                                         ofType:@"jpg"];
    NSData *emoticonData = [NSData dataWithContentsOfFile:filePath];
    
    UIImage *thumbImage = [UIImage imageNamed:@"res5thumb.png"];
    [WXApiRequestHandler sendEmotionData:emoticonData
                              ThumbImage:thumbImage
                                 InScene:_currentScene];
}

- (void)sendGifContent {
    NSString *filePath = [[NSBundle mainBundle] pathForResource:@"res6"
                                                         ofType:@"gif"];
    NSData *emoticonData = [NSData dataWithContentsOfFile:filePath];
    
    UIImage *thumbImage = [UIImage imageNamed:@"res6thumb.png"];
    [WXApiRequestHandler sendEmotionData:emoticonData
                              ThumbImage:thumbImage
                                 InScene:_currentScene];
}

- (void)sendAuthRequest {
    [WXApiRequestHandler sendAuthRequestScope: kAuthScope
                                        State:kAuthState
                                       OpenID:kAuthOpenID
                             InViewController:self];
}

- (void)sendFileContent {
    UIImage *thumbImage = [UIImage imageNamed:@"res2.jpg"];
    NSString* filePath = [[NSBundle mainBundle] pathForResource:kFileName
                                                         ofType:kFileExtension];
    NSData *fileData = [NSData dataWithContentsOfFile:filePath];

    [WXApiRequestHandler sendFileData:fileData
                        fileExtension:kFileExtension
                                Title:kFileTitle
                          Description:kFileDescription
                           ThumbImage:thumbImage
                              InScene:_currentScene];
}

- (void)addCardToWXCardPackage {
    /*
    WXCardItem* cardItem = [[WXCardItem alloc] init];
    cardItem.cardId = @"pDF3iY9tv9zCGCj4jTXFOo1DxHdo";
    cardItem.extMsg = @"{\"code\": \"\", \"openid\": \"\", \"timestamp\": \"1418301401\", \"signature\":\"f54dae85e7807cc9525ccc127b4796e021f05b33\"}";
    [WXApiRequestHandler addCardsToCardPackage:[NSArray arrayWithObject:cardItem]];
     */
}

- (void)batchAddCardToWXCardPackage {
    /*
    WXCardItem* cardItem = [[WXCardItem alloc] init];
    cardItem.cardId = @"pDF3iY9tv9zCGCj4jTXFOo1DxHdo";
    cardItem.extMsg = @"{\"code\": \"\", \"openid\": \"\", \"timestamp\": \"1418301401\", \"signature\":\"f54dae85e7807cc9525ccc127b4796e021f05b33\"}";
    [WXApiRequestHandler addCardsToCardPackage:[NSArray arrayWithObjects:cardItem,cardItem, nil]];
     */
}

#pragma mark -UIAlertViewDelegate
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (buttonIndex == alertView.cancelButtonIndex)
        return;
    
    if (alertView.tag == kRecvGetMessageReqAlertTag) {
        RespForWeChatViewController* controller = [[RespForWeChatViewController alloc]autorelease];
        [self presentViewController:controller animated:YES completion:nil];
    }
}

#pragma mark - WXApiManagerDelegate
- (void)managerDidRecvGetMessageReq:(GetMessageFromWXReq *)req {
    // 微信请求App提供内容， 需要app提供内容后使用sendRsp返回
    NSString *strTitle = [NSString stringWithFormat:@"微信请求App提供内容"];
    NSString *strMsg = [NSString stringWithFormat:@"openID: %@", req.openID];
    
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:strTitle
                                                    message:strMsg
                                                   delegate:self
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil, nil];
    alert.tag = kRecvGetMessageReqAlertTag;
    [alert show];
    [alert release];
}

- (void)managerDidRecvShowMessageReq:(ShowMessageFromWXReq *)req {
    WXMediaMessage *msg = req.message;
    
    //显示微信传过来的内容
    WXAppExtendObject *obj = msg.mediaObject;
    
    NSString *strTitle = [NSString stringWithFormat:@"微信请求App显示内容"];
    NSString *strMsg = [NSString stringWithFormat:@"openID: %@, 标题：%@ \n内容：%@ \n附带信息：%@ \n缩略图:%lu bytes\n附加消息:%@\n", req.openID, msg.title, msg.description, obj.extInfo, (unsigned long)msg.thumbData.length, msg.messageExt];
    
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:strTitle
                                                    message:strMsg
                                                   delegate:self
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil, nil];
    [alert show];
    [alert release];
}

- (void)managerDidRecvLaunchFromWXReq:(LaunchFromWXReq *)req {
    WXMediaMessage *msg = req.message;
    
    //从微信启动App
    NSString *strTitle = [NSString stringWithFormat:@"从微信启动"];
    NSString *strMsg = [NSString stringWithFormat:@"openID: %@, messageExt:%@", req.openID, msg.messageExt];
    
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:strTitle
                                                    message:strMsg
                                                   delegate:self
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil, nil];
    [alert show];
    [alert release];
}

- (void)managerDidRecvMessageResponse:(SendMessageToWXResp *)response {
    NSString *strTitle = [NSString stringWithFormat:@"发送媒体消息结果"];
    NSString *strMsg = [NSString stringWithFormat:@"errcode:%d", response.errCode];
    
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:strTitle
                                                    message:strMsg
                                                   delegate:self
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil, nil];
    [alert show];
    [alert release];
}

- (void)managerDidRecvAddCardResponse:(AddCardToWXCardPackageResp *)response {
        NSMutableString* cardStr = [[[NSMutableString alloc] init] autorelease];
    for (WXCardItem* cardItem in response.cardAry) {
        [cardStr appendString:[NSString stringWithFormat:@"cardid:%@ cardext:%@ cardstate:%u\n",cardItem.cardId,cardItem.extMsg,(unsigned int)cardItem.cardState]];
    }
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"add card resp"
                                                    message:cardStr
                                                   delegate:self
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil, nil];
    [alert show];
    [alert release];
}

- (void)managerDidRecvAuthResponse:(SendAuthResp *)response {
    NSString *strTitle = [NSString stringWithFormat:@"Auth结果"];
    NSString *strMsg = [NSString stringWithFormat:@"code:%@,state:%@,errcode:%d", response.code, response.state, response.errCode];
    
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:strTitle
                                                    message:strMsg
                                                   delegate:self
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil, nil];
    [alert show];
    [alert release];
}

#pragma mark - Private Methods
- (void)setupHeadView {
    UIView *headView = [[UIView alloc]initWithFrame:CGRectMake(0, 0, SCREEN_WIDTH, kHeadViewHeight)];
    [headView setBackgroundColor:RGBCOLOR(0xe1, 0xe0, 0xde)];
    UIImage *image = [UIImage imageNamed:@"micro_messenger.png"];
    NSInteger tlx = (headView.frame.size.width -  image.size.width) / 2;
    NSInteger tly = 20;
    
    UIImageView *imageView = [[UIImageView alloc]initWithFrame:CGRectMake(tlx, tly, image.size.width, image.size.height)];
    [imageView setImage:image];
    [headView addSubview:imageView];
    [imageView release];
    
    UILabel *title = [[UILabel alloc]initWithFrame:CGRectMake(0, tly + image.size.height, SCREEN_WIDTH, 40)];
    [title setText:@"微信OpenAPI Sample Demo"];
    title.font = [UIFont systemFontOfSize:17];
    title.textColor = RGBCOLOR(0x11, 0x11, 0x11);
    title.textAlignment = NSTextAlignmentCenter;
    title.backgroundColor = [UIColor clearColor];
    [headView addSubview:title];
    [title release];
    
    [self.view addSubview:headView];
    [headView release];
}

- (void)setupLineView {
    UIView *lineView1 = [[UIView alloc] initWithFrame:CGRectMake(0, kHeadViewHeight, SCREEN_WIDTH, 1)];
    lineView1.backgroundColor = [UIColor blackColor];
    lineView1.alpha = 0.1f;
    [self.view addSubview:lineView1];
    [lineView1 release];
    
    UIView *lineView2 = [[UIView alloc]initWithFrame:CGRectMake(0, kHeadViewHeight + 1, SCREEN_WIDTH, 1)];
    lineView2.backgroundColor = [UIColor whiteColor];
    lineView2.alpha = 0.25f;
    [self.view addSubview:lineView2];
    [lineView2 release];
    
    UIView *lineView3 = [[UIView alloc] initWithFrame:CGRectMake(0, kHeadViewHeight + 2 + kSceneViewHeight, SCREEN_WIDTH, 1)];
    lineView3.backgroundColor = [UIColor blackColor];
    lineView3.alpha = 0.1f;
    [self.view addSubview:lineView3];
    [lineView3 release];
    
    UIView *lineView4 = [[UIView alloc]initWithFrame:CGRectMake(0, kHeadViewHeight + 2 + kSceneViewHeight + 1, SCREEN_WIDTH, 1)];
    lineView4.backgroundColor = [UIColor whiteColor];
    lineView4.alpha = 0.25f;
    [self.view addSubview:lineView4];
    [lineView4 release];
}

- (void)setupSceneView {
    UIView *sceceView = [[UIView alloc] initWithFrame:CGRectMake(0, kHeadViewHeight + 2, SCREEN_WIDTH, 100)];
    [sceceView setBackgroundColor:RGBCOLOR(0xef, 0xef, 0xef)];
    
    UILabel *tips = [[UILabel alloc]init];
    tips.tag = TIPSLABEL_TAG;
    tips.text = @"分享场景:会话";
    tips.textColor = [UIColor blackColor];
    tips.backgroundColor = [UIColor clearColor];
    tips.textAlignment = NSTextAlignmentLeft;
    tips.frame = CGRectMake(10, 5, 200, 40);
    [sceceView addSubview:tips];
    [tips release];
    
    UIButton *btn_x = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    [btn_x setTitle:@"会话" forState:UIControlStateNormal];
    btn_x.titleLabel.font = [UIFont systemFontOfSize:15];
    [btn_x setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [btn_x setTitleColor:[UIColor grayColor] forState:UIControlStateDisabled];
    [btn_x setFrame:CGRectMake(20, 50, 80, 40)];
    [btn_x addTarget:self action:@selector(onSelectSessionScene) forControlEvents:UIControlEventTouchUpInside];
    [sceceView addSubview:btn_x];
    
    UIButton *btn_y = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    [btn_y setTitle:@"朋友圈" forState:UIControlStateNormal];
    btn_y.titleLabel.font = [UIFont systemFontOfSize:15];
    [btn_y setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [btn_y setTitleColor:[UIColor grayColor] forState:UIControlStateDisabled];
    [btn_y setFrame:CGRectMake(120, 50, 80, 40)];
    [btn_y addTarget:self action:@selector(onSelectTimelineScene) forControlEvents:UIControlEventTouchUpInside];
    [sceceView addSubview:btn_y];
    
    UIButton *btn_z = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    [btn_z setTitle:@"收藏" forState:UIControlStateNormal];
    btn_z.titleLabel.font = [UIFont systemFontOfSize:15];
    [btn_z setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [btn_z setTitleColor:[UIColor grayColor] forState:UIControlStateDisabled];
    [btn_z setFrame:CGRectMake(220, 50, 80, 40)];
    [btn_z addTarget:self action:@selector(onSelectFavoriteScene) forControlEvents:UIControlEventTouchUpInside];
    [sceceView addSubview:btn_z];
    
    [self.view addSubview:sceceView];
    [sceceView release];
}

#define LEFTMARGIN			12
#define TOPMARGIN			15
#define BTNWIDTH			140
#define BTNHEIGHT			40
#define ADDBUTTON_AUTORELEASE(idx, title, sel) [self addBtn:idx tit:title selector:sel]

-(UIButton*) addBtn:(int)idx tit:(NSString*)title selector:(SEL)sel
{
    CGRect rect;
    if(idx % 2 == 1) {
        rect = CGRectMake(LEFTMARGIN, 25*(idx/2) + TOPMARGIN*(idx/2+1), BTNWIDTH, BTNHEIGHT - 4);
    } else {
        rect = CGRectMake(LEFTMARGIN*2 + BTNWIDTH, 25*(idx/2-1) + TOPMARGIN*(idx/2), BTNWIDTH, BTNHEIGHT - 4);
    }
    UIButton *btn = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    btn.frame = rect;
    btn.tag = idx;
    [btn setTitle:title forState:UIControlStateNormal];
    btn.titleLabel.font = [UIFont systemFontOfSize:14.0];
    [btn addTarget:self action:sel forControlEvents:UIControlEventTouchUpInside];
    [self.footView addSubview:btn];
    
    return btn;
}

- (void)setupFootView {
    self.footView = [[UIScrollView alloc]initWithFrame:CGRectMake(0, kHeadViewHeight + 2 + kSceneViewHeight + 2, SCREEN_WIDTH, SCREEN_HEIGHT - kHeadViewHeight - 2 - kSceneViewHeight - 2)];
    [self.footView setBackgroundColor:RGBCOLOR(0xef, 0xef, 0xef)];
    self.footView.contentSize = CGSizeMake(SCREEN_WIDTH, SCREEN_HEIGHT);
    [self.view addSubview:self.footView];
    
    int index = 1;
    ADDBUTTON_AUTORELEASE(index++, @"发送Text消息给微信", @selector(sendTextContent));
    ADDBUTTON_AUTORELEASE(index++, @"发送Photo消息给微信", @selector(sendImageContent));
    ADDBUTTON_AUTORELEASE(index++, @"发送Link消息给微信", @selector(sendLinkContent));
    ADDBUTTON_AUTORELEASE(index++, @"发送Music消息给微信", @selector(sendMusicContent));
    ADDBUTTON_AUTORELEASE(index++, @"发送Video消息给微信", @selector(sendVideoContent));
    ADDBUTTON_AUTORELEASE(index++, @"发送App消息给微信", @selector(sendAppContent));
    ADDBUTTON_AUTORELEASE(index++, @"发送非gif表情给微信", @selector(sendNonGifContent));
    ADDBUTTON_AUTORELEASE(index++, @"发送gif表情给微信", @selector(sendGifContent));
    ADDBUTTON_AUTORELEASE(index++, @"微信授权登录", @selector(sendAuthRequest));
    ADDBUTTON_AUTORELEASE(index++, @"发送文件消息给微信", @selector(sendFileContent));
    ADDBUTTON_AUTORELEASE(index++, @"添加单张卡券至卡包", @selector(addCardToWXCardPackage));
    ADDBUTTON_AUTORELEASE(index++, @"添加多张卡券至卡包", @selector(batchAddCardToWXCardPackage));
}

@end
