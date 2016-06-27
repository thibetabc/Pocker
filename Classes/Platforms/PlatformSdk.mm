#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "PlatformSdk.h"
#import <AddressBook/AddressBook.h>
#import <Contacts/Contacts.h>
#include "LayerFriendContactsList.h"
#include "MessageViewController.h"
#import "WXApi.h"
#include "utils.h"
#include "net_manager.h"
#import "WXApiObject.h"

USING_NS_CC;

PlatformSdk *PlatformSdk::mPlatformSdk = nullptr;

PlatformSdk::PlatformSdk()
{
    
}

PlatformSdk::~PlatformSdk()
{
    
}

//读取手机通讯录
bool PlatformSdk::getPhoneContacts()
{
    CFErrorRef error;
    //新建一个通讯录类
    ABAddressBookRef _addressBook = ABAddressBookCreateWithOptions(NULL,&error);
    if ([[UIDevice currentDevice].systemVersion floatValue]>=6.0)
    {
        ABAddressBookRequestAccessWithCompletion(_addressBook, nullptr);
        ABAuthorizationStatus authStatus = ABAddressBookGetAuthorizationStatus();
        if(authStatus != kABAuthorizationStatusAuthorized)
        {
            log("未授权，请打开");
//            UIAlertView *av = [[UIAlertView alloc]
//                               initWithTitle:@"好牌"
//                               message:@"你未开启”允许好牌访问通讯录“，请到系统设置-隐私-通讯录中开启"
//                               delegate:nil
//                               cancelButtonTitle:nil
//                               otherButtonTitles:@"确定", nil, nil];
//            [av show];
            
            return false;
            
        }
    }
    
    
    if (_addressBook)
    {
        __block std::vector<LayerFriendContactsList::Contacts> mContacts;
        if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 9.0)
        {
            CNContactStore *store = [[CNContactStore alloc] init];
            CNContactFetchRequest *request = [[CNContactFetchRequest alloc] initWithKeysToFetch:@[CNContactFamilyNameKey,CNContactGivenNameKey,CNContactPhoneNumbersKey]];
            NSError *error = nil;
            //执行获取通讯录请求，若通讯录可获取，flag为YES，代码块也会执行，若获取失败，flag为NO，代码块不执行
            BOOL flag = [store enumerateContactsWithFetchRequest:request error:&error usingBlock:^(CNContact * _Nonnull contact, BOOL * _Nonnull stop)
                         {
                             
                             LayerFriendContactsList::Contacts tmpContacts;
                             
                             //名字
                             NSString *name = @"";
                             if ([NSString stringWithFormat:@"%@%@",contact.familyName,contact.givenName]) {
                                 name =  [NSString stringWithFormat:@"%@%@",contact.familyName,contact.givenName];
                                 
                             }
                             tmpContacts.name = [name UTF8String];
                             
                             //号码
                             NSString *strPhone = @"";
                             for(CNLabeledValue<CNPhoneNumber*>* phone in contact.phoneNumbers)
                             {
                                 strPhone = phone.value.stringValue;
                                 tmpContacts.allTelephone.push_back([strPhone UTF8String]);
                                 NSLog(@" %@ == %@", name , strPhone);
                             }
                             
                             mContacts.push_back(tmpContacts);
                         }];
            if (flag)
            {
                NSLog(@"通讯录可获取");
            }
            [request autorelease];
            [store autorelease];
            
        }
        else
        {
            //获取所有联系人的数组
            NSArray*  _arrayRef = (NSArray*) ABAddressBookCopyArrayOfAllPeople(_addressBook);
            //循环, 获取每个人的个人信息
            for (id obj in _arrayRef)
            {
                LayerFriendContactsList::Contacts tmpContacts;
                
                //获取个人
                ABRecordRef person = (__bridge ABRecordRef)obj;
                //获取个人名字
                CFTypeRef abName = ABRecordCopyValue(person, kABPersonFirstNameProperty);
                CFTypeRef abLastName = ABRecordCopyValue(person, kABPersonLastNameProperty);
                CFStringRef abFullName = ABRecordCopyCompositeName(person);
                NSString* nameString = (__bridge NSString*)abName;
                NSString* lastNameString = (__bridge NSString*)abLastName;
                if ((__bridge id) abFullName != nil)
                {
                    nameString = (__bridge NSString*)abFullName;
                }
                else
                {
                    if ((__bridge id)abLastName != nil)
                    {
                        nameString = [NSString stringWithFormat:@"%@%@",nameString,lastNameString];
                    }
                }
                
                tmpContacts.name = [nameString UTF8String];
                ABMultiValueRef phones= ABRecordCopyValue(obj, kABPersonPhoneProperty);
                for (NSInteger j=0; j<ABMultiValueGetCount(phones); j++)
                {
                    NSString *phone =(__bridge NSString*)ABMultiValueCopyValueAtIndex(phones, j);
                    NSLog(@"%@ => %@", nameString, phone);
                    tmpContacts.allTelephone.push_back([phone UTF8String]);
                }
                
                if (abName) CFRelease(abName);
                if (abLastName) CFRelease(abLastName);
                if (abFullName) CFRelease(abFullName);
                mContacts.push_back(tmpContacts);
                
            }
        }
        auto layer = dynamic_cast<LayerFriendContactsList*>(SceneManager::get_layer_by_name(typeid(LayerFriendContactsList).name()));
        if(layer)
        {
            layer->setContacts(mContacts);
           
        }
        return true;
    }
    return false;
}


//程序内调用系统发短信
void PlatformSdk::showMessageView(std::string phone, std::string title, std::string msg)
{
    [[MessageViewController alloc] showMessageView:@[[NSString stringWithUTF8String:phone.c_str()]]
                                             title:[NSString stringWithUTF8String:title.c_str()]
                                              body:[NSString stringWithUTF8String:msg.c_str()]];
}



//微信
void PlatformSdk::shareWeixin(TargetShare targetShare,  ShareType shareType, std::string title, std::string description, std::string linkUrl, std::string image)
{
    WXMediaMessage *mediaMessage = nil;
    SendMessageToWXReq *req = nil;
    WXScene scene;
    
    NSString *tTitle = [NSString stringWithUTF8String:title.c_str()];
    NSString *tDescription = [NSString stringWithUTF8String:description.c_str()];
    NSString *tLinkUrl = [NSString stringWithUTF8String:linkUrl.c_str()];
    NSString *tImage = [NSString stringWithUTF8String:image.c_str()];
    
    switch(targetShare)
    {
        case TargetShare::WeiXinWeChat:
            scene = WXScene::WXSceneSession;
            break;
        case TargetShare::WeiXinCircle:
            scene = WXScene::WXSceneTimeline;
            break;
    }
    
    if(shareType == ShareType::Text)
    {
        
        req = [[SendMessageToWXReq alloc] init];
        req.text = tDescription;
        req.scene = scene;
        req.bText = YES;
    }
    else if(shareType == ShareType::Image)
    {
        
        mediaMessage = [WXMediaMessage message];
        //原图-要生成缩略图：
        UIImage* image = [UIImage imageNamed:tImage];
        
        
        //////////////////独立部分--开始生成缩略图////////////////////////
        CGSize oldsize = image.size;
        CGSize asize = CGSizeMake(oldsize.width/2,oldsize.height/2);
        CGRect rect;
        
        if (asize.width/asize.height > oldsize.width/oldsize.height) {
            
            rect.size.width = asize.height*oldsize.width/oldsize.height;
            
            rect.size.height = asize.height;
            
            rect.origin.x = (asize.width - rect.size.width)/2;
            
            rect.origin.y = 0;
            
        }
        
        else{
            
            rect.size.width = asize.width;
            
            rect.size.height = asize.width*oldsize.height/oldsize.width;
            
            rect.origin.x = 0;
            
            rect.origin.y = (asize.height - rect.size.height)/2;
            
        }
        
        //开始绘图
        
        UIGraphicsBeginImageContext(asize);
        
        CGContextRef context = UIGraphicsGetCurrentContext();
        
        CGContextSetFillColorWithColor(context, [[UIColor clearColor] CGColor]);
        
        UIRectFill(CGRectMake(0, 0, asize.width, asize.height));//clear background
        
        [image drawInRect:rect];
        
        image = UIGraphicsGetImageFromCurrentImageContext();
        
        //结束绘图
        UIGraphicsEndImageContext();
        //////////////////////////独立部分--生成缩略图结束/////////////////////////////////////
        
        
        [mediaMessage setThumbImage:image];
        
        WXImageObject *imageObject = [WXImageObject object];
        NSData *data = [NSData dataWithContentsOfFile:tImage];
        imageObject.imageData = data;
        mediaMessage.mediaObject = imageObject;
        
        req = [[SendMessageToWXReq alloc] init];
        req.bText = NO;
        req.message = mediaMessage;
        req.scene = scene;
        
    }
    
    else if(shareType == ShareType::Web)
    {
        UIImage *thumbImage = [UIImage imageNamed:tImage];
        WXWebpageObject *ext = [WXWebpageObject object];
        ext.webpageUrl = tLinkUrl;
        mediaMessage = [WXMediaMessage messageWithTitle:tTitle
                                        Description:tDescription
                                             Object:ext
                                         MessageExt:nil
                                      MessageAction:nil
                                         ThumbImage:thumbImage
                        MediaTag:@"tag_web"];
        
        req = [SendMessageToWXReq requestWithText:nil
                                   OrMediaMessage:mediaMessage
                                            bText:NO
                                          InScene:scene];
    }
    else
    {
        return;
    }
    
    bool isSucceed = [WXApi sendReq:req];
    log("微信分享请求： %s", isSucceed ? "成功" : "失败");
}


//https://blog.yourtion.com/get-current-language-on-ios9.html
PlatformSdk::LanguageType PlatformSdk::getCurrentLanguage()
{
    // get the current language and country config
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSArray *languages = [defaults objectForKey:@"AppleLanguages"];
    NSString *currentLanguage = [languages objectAtIndex:0];
    
    LanguageType ret = LanguageType::ENGLISH;
    if ([currentLanguage containsString:@"zh-Hans"])
    {
        ret = LanguageType::CHINESE;
    }
    else if ([currentLanguage containsString:@"zh-Hant"])
    {
        ret = LanguageType::TRADITIONAL_CHINESE;
    }
    else if ([currentLanguage containsString:@"en"])
    {
        ret = LanguageType::ENGLISH;
    }
    else if ([currentLanguage containsString:@"fr"]){
        ret = LanguageType::FRENCH;
    }
    else if ([currentLanguage containsString:@"it"]){
        ret = LanguageType::ITALIAN;
    }
    else if ([currentLanguage containsString:@"de"]){
        ret = LanguageType::GERMAN;
    }
    else if ([currentLanguage containsString:@"es"]){
        ret = LanguageType::SPANISH;
    }
    else if ([currentLanguage containsString:@"nl"]){
        ret = LanguageType::DUTCH;
    }
    else if ([currentLanguage containsString:@"ru"]){
        ret = LanguageType::RUSSIAN;
    }
    else if ([currentLanguage containsString:@"ko"]){
        ret = LanguageType::KOREAN;
    }
    else if ([currentLanguage containsString:@"ja"]){
        ret = LanguageType::JAPANESE;
    }
    else if ([currentLanguage containsString:@"hu"]){
        ret = LanguageType::HUNGARIAN;
    }
    else if ([currentLanguage containsString:@"pt"]){
        ret = LanguageType::PORTUGUESE;
    }
    else if ([currentLanguage containsString:@"ar"]){
        ret = LanguageType::ARABIC;
    }
    else if ([currentLanguage containsString:@"nb"]){
        ret = LanguageType::NORWEGIAN;
    }
    else if ([currentLanguage containsString:@"pl"]){
        ret = LanguageType::POLISH;
    }
    else if ([currentLanguage containsString:@"tr"]){
        ret = LanguageType::TURKISH;
    }
    else if ([currentLanguage containsString:@"uk"]){
        ret = LanguageType::UKRAINIAN;
    }
    else if ([currentLanguage containsString:@"ro"]){
        ret = LanguageType::ROMANIAN;
    }
    else if ([currentLanguage containsString:@"bg"]){
        ret = LanguageType::BULGARIAN;
    }
    return ret;
}


#endif