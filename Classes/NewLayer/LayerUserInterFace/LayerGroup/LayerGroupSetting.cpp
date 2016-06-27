//
//  LayerGroupSetting.cpp
//  pokerclient
//
//  Created by qqronaldz on 16/4/20.
//
//

#include "LayerGroupSetting.h"
#include "ProcessorManager.h"
#include "NET_CMD.h"
#include "GameTools.h"
#include "GameDataManager.h"
#include "FileManager.h"
#include "cmd_def.h"
#include "ChatMessageManager.h"
#include "LayerUserInfo.h"
#include "TipView.h"
#include "LayerFriendList.h"
#include "LayerChoosePhoto.h"
#include "utils.h"
#include "LayerShare.h"
#include "LayerChat.h"
#include "LayerGroupList.h"
#include "Toast.h"

LayerGroupSetting::LayerGroupSetting()
:mRow(0),
mPreWidth(0),
mPreHeight(0),
mImageHeadSize(Size(120, 120)),
mLayoutMemberSize(Size::ZERO),
mInsertHeight(0),
mGroupNum(0),
mListener(nullptr),
isRemoving(false),
mInsertRow(-1),
mGroupUserStatus(msg::GroupUserStatus::NOT_GROUP_MEMBER),
isDeleteGroup(false),
mSelectTag(0),
isNotifications(false),
isPremissionTemplateModify(false),
isScroll(false),
isExitLayer(false),
isCreateManagerCell(false),
isRefreshGroupInfo(false)
{
    
}

LayerGroupSetting::~LayerGroupSetting()
{
    
    if(mListener)
    {
        getEventDispatcher()->removeEventListener(mListener);
        getEventDispatcher()->removeEventListenersForTarget(mImageHead);
    }
    
}

LayerGroupSetting *LayerGroupSetting::create(uint32_t groupId)
{
    auto layer = new LayerGroupSetting();
    if(layer)
    {
        
        layer->mGroupId = groupId;
        if(layer->init())
        {
            layer->autorelease();
            return layer;
        }
    }
    delete layer;
    layer = nullptr;
    return nullptr;
    
    
}
bool LayerGroupSetting::init()
{
    if(!BaseLayer::init())
    {
        return false;
    }
    
    
    mRoot = CSLoader::createNode("LayerGroupSetting.csb");
    mListviewAll = dynamic_cast<ListView*>(CSLoader::seekNodeByName(mRoot, "listview"));
    mListviewAll->setClippingEnabled(true);
    mListviewAll->setScrollEndedCallback([&](){GameTools::editBoxLoseInputFocus();});
    mListviewAllHead = dynamic_cast<ListView*>(CSLoader::seekNodeByName(mRoot, "listview_head"));
    mLayoutAllHead   = dynamic_cast<Layout*>(CSLoader::seekNodeByName(mRoot, "layout_head_bg"));
    mLayoutMember    = dynamic_cast<Layout*>(CSLoader::seekNodeByName(mRoot, "layout_member"));
    mLayoutMemberSize = mLayoutMember->getContentSize();
    auto layoutGroupHead = dynamic_cast<Layout*>(CSLoader::seekNodeByName(mRoot,"layout_head"));
    mGroupHeadSize = layoutGroupHead->getContentSize();
    auto btnBack = dynamic_cast<Button *>(CSLoader::seekNodeByName(mRoot, "btn_back"));
    mTitleGroupName = dynamic_cast<Text*>(CSLoader::seekNodeByName(mRoot, "text_title"));
    auto layoutImgHead = dynamic_cast<Layout*>(CSLoader::seekNodeByName(mRoot, "layout_head"));
    auto imgHead = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(mRoot, "img_head"));
    imgHead->setVisible(false);
    //头像
    mImageHead = ShaderSprite::createAndDownload("Default/image_defualt_user_icon_small.png","Default/image_defualt_user_icon_small.png", "Default/image_mask_circle.png");
    mImageHead->setPosition(imgHead->getPosition());
    auto eventDispatcher = getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);//不向下传递触摸
    listener->onTouchBegan = [&](Touch *touch, Event *event)
    {
        if(!isManagerOrMaster())
        {
            return false;
        }
        auto spriteHead = dynamic_cast<Sprite*>(event->getCurrentTarget());
        auto size = spriteHead->getContentSize();
        auto rect = cocos2d::Rect(0, 0, size.width, size.height);
        auto locationInNode = spriteHead->convertToNodeSpace(touch->getLocation());
        if(rect.containsPoint(locationInNode))
        {
            return true;
        }
        
        return false;
    };
    listener->onTouchEnded = [&](Touch *touch, Event *event)
    {
        log("换头像imgHead->addClickEventListener");
        pickerImage();
    };
    eventDispatcher->addEventListenerWithSceneGraphPriority(listener, mImageHead);
    layoutImgHead->addChild(mImageHead, imgHead->getLocalZOrder());
    
    btnBack->addClickEventListener([this](Ref *ref)
    {
        isExitLayer = true;
        removeFromParent();
    });
    
    
    
    
    //Layout俱乐部管理
    mLayoutManager = dynamic_cast<Layout*>(CSLoader::seekNodeByName(mRoot, "layout_manager_bg"));
    mSizeLayoutManager = mLayoutManager->getContentSize();
//    mLayoutManager->setScale(0);
    mLayoutRank = dynamic_cast<Layout*>(CSLoader::seekNodeByName(mRoot, "layout_rank"));
    mCheckboxRank = dynamic_cast<CheckBox*>(CSLoader::seekNodeByName(mRoot, "checkbox_rank"));
    mCheckboxRank->addEventListener([&](Ref*ref,CheckBox::EventType type)
                                         {
                                             auto checkbox = dynamic_cast<CheckBox*>(ref);
                                             setChectboxTexture(checkbox);
                                             requestPublicRank(checkbox->isSelected());
                                         });
    
    //公用设置
    mLayoutCommonSetting = dynamic_cast<Layout*>(CSLoader::seekNodeByName(mRoot, "layout_setting_bg"));
    mListviewCommonSetting= dynamic_cast<ListView*>(CSLoader::seekNodeByName(mRoot, "listview_common_setting"));

    //改俱乐部名
    mTextGroupName = dynamic_cast<Text*>(CSLoader::seekNodeByName(mRoot, "text_group_name"));
    auto textfieldGroupName = dynamic_cast<TextField*>(CSLoader::seekNodeByName(mRoot, "textfield_group_name"));
    auto layoutGroupName = CSLoader::seekNodeByName(mRoot, "layout_group_name");
    mGroupNameCellSize = layoutGroupName->getContentSize();
    mEditBoxGroupName = GameTools::createEditBoxFromTextField(layoutGroupName, textfieldGroupName);
    mEditBoxGroupName->setTag(0);
    mEditBoxGroupName->setText("");
    mEditBoxGroupName->setFontColor(Color3B::WHITE);
    mEditBoxGroupName->setInputMode(EditBox::InputMode::SINGLE_LINE);
    mEditBoxGroupName->setReturnType(EditBox::KeyboardReturnType::DONE);
    mEditBoxGroupName->setDelegate(this);
    mEditBoxGroupName->setTouchEnabled(false);
    mEditBoxGroupName->setMaxLength(TITLE_MAX_LENGHT);
    mBtnShare = dynamic_cast<Button*>(CSLoader::seekNodeByName(mRoot, "btn_share"));
    mBtnShare->setTitleText(tools::local_string("sharemoment","分享"));
    mBtnShare->addClickEventListener([&](Ref *ref)
    {
        log("俱乐部分享 btnShare->addClickEventListener");
        auto layerShare = LayerShare::create();
        layerShare->setShareGroupInfo(GDM->get_user_id(), mGroupId);
        AddGlobalLayer(layerShare);
    });
    //改俱乐部备注
    mTextRemark = dynamic_cast<Text*>(CSLoader::seekNodeByName(mRoot, "text_remark"));
    auto layoutGroupRemark = CSLoader::seekNodeByName(mRoot, "layout_remark");
    auto textfieldGroupRemark = dynamic_cast<TextField*>(CSLoader::seekNodeByName(mRoot, "textfield_remark"));
    mEditBoxRemark = GameTools::createEditBoxFromTextField(layoutGroupRemark, textfieldGroupRemark);
    mEditBoxRemark->setTag(1);
    mEditBoxRemark->setText("");
    mEditBoxRemark->setFontColor(Color3B::WHITE);
    mEditBoxRemark->setInputMode(EditBox::InputMode::SINGLE_LINE);
    mEditBoxRemark->setReturnType(EditBox::KeyboardReturnType::DONE);
    mEditBoxRemark->setDelegate(this);
    mEditBoxRemark->setTouchEnabled(false);
    mEditBoxRemark->setMaxLength(TITLE_MAX_LENGHT);
    
    mCheckboxNotification = dynamic_cast<CheckBox*>(CSLoader::seekNodeByName(mRoot, "checkbox_notifications"));
    mCheckboxNotification->addClickEventListener([&](Ref *ref)
    {
        log("消息免打扰");
        requestIgnoreNotifications(!isNotifications);
    });
                                                 
    mButtonCleanChatHistory = dynamic_cast<Button*>(CSLoader::seekNodeByName(mRoot, "btn_clean_chat_history"));
    mButtonCleanChatHistory->addClickEventListener([&](Ref *ref)
    {
        log("清空聊天记录");
        cleanGroupChatData();
        
    });
    mButtonDeleteLeave = dynamic_cast<Button*>(CSLoader::seekNodeByName(mRoot, "btn_delete_leave"));
    mButtonDeleteLeave->addClickEventListener([this](Ref *ref)
    {
        log("解散俱乐部");
       
        auto layer = TipView::showAlertView(tools::local_string("quit_club","确定退出/解散俱乐部？"), true);
        layer->set_tipview_close_callback([&]()
        {
             requestDeleteLeave();
        });
        
        
    });
    
    
    
    addChild(mRoot);
    requestGroupInfo();
    setLanguage(mRoot);//设置语言
    return true;
}

//俱乐部管理
void LayerGroupSetting::initManagerNode()
{
    //俱乐部管理
    mCheckboxManager = dynamic_cast<CheckBox*>(CSLoader::seekNodeByName(mRoot, "checkbox_manager"));
    mCheckboxManager->setTag(0);
    mCheckboxManager->setSelected(mGroupManageSetting.isclub());
    mCheckboxManager->addEventListener(std::bind(&LayerGroupSetting::managerEventListener, this, std::placeholders::_1, std::placeholders::_2));
    setChectboxTexture(mCheckboxManager);
    
    //信用值返还
    mCheckboxCreditValueBack = dynamic_cast<CheckBox*>(CSLoader::seekNodeByName(mRoot, "checkbox_credit_back"));
    mCheckboxCreditValueBack->setTag(1);
    mCheckboxCreditValueBack->setSelected(mGroupManageSetting.creditrefillaftergame());
    mCheckboxCreditValueBack->addEventListener(std::bind(&LayerGroupSetting::managerEventListener, this, std::placeholders::_1, std::placeholders::_2));
    setChectboxTexture(mCheckboxCreditValueBack);
    
    //允许玩家查看群成员
    mCheckboxClickViewPlayer = dynamic_cast<CheckBox*>(CSLoader::seekNodeByName(mRoot, "checkbox_check_member"));
    mCheckboxClickViewPlayer->setTag(2);
    mCheckboxClickViewPlayer->setSelected(mGroupManageSetting.canseeothermember());
    mCheckboxClickViewPlayer->addEventListener(std::bind(&LayerGroupSetting::managerEventListener, this, std::placeholders::_1, std::placeholders::_2));
    setChectboxTexture(mCheckboxClickViewPlayer);
    
    
    //群成员禁言
    mCheckboxSilenced = dynamic_cast<CheckBox*>(CSLoader::seekNodeByName(mRoot, "checkbox_silence"));
    mCheckboxSilenced->setTag(3);
    mCheckboxSilenced->setSelected(mGroupManageSetting.canchat());
    mCheckboxSilenced->addEventListener(std::bind(&LayerGroupSetting::managerEventListener, this, std::placeholders::_1, std::placeholders::_2));
    setChectboxTexture(mCheckboxSilenced);
    
    
    //抽水比例
    mSliderManager = dynamic_cast<Slider*>(CSLoader::seekNodeByName(mRoot, "slider_manager_deduct"));
    mSliderManager->setPercent(mGroupManageSetting.rakepercent() * MAX_DEDUCT_PERCENT);
    mSliderManager->addClickEventListener(std::bind(&LayerGroupSetting::deductSlide, this, std::placeholders::_1));
    mSliderManager->addEventListener(std::bind(&LayerGroupSetting::deductSlideEventListener, this, std::placeholders::_1, std::placeholders::_2));
    auto textSliderPercent = dynamic_cast<Text*>(CSLoader::seekNodeByName(mRoot, "text_deduct_percent"));
    textSliderPercent->setString(tools::anything2string(mGroupManageSetting.rakepercent()) + "%");
    auto textOwnCreditValue = dynamic_cast<Text*>(CSLoader::seekNodeByName(mRoot, "text_own_credit"));
    textOwnCreditValue->setString(tools::anything2string(mPermissionOwn.credit()));
    
    setManagerColor(isMaster() && mCheckboxManager->isSelected());
}

void LayerGroupSetting::setManagerColor(bool isSelected)
{
    auto listview = dynamic_cast<ListView*>(mLayoutManager->getChildByName("listview_manager"));
    if(listview)
    {
        auto childCount = listview->getChildrenCount();
        for(int i = 2; i != childCount; ++i)
        {
            auto widget = listview->getItem(i);
            if(widget)
            {
                widget->setColor(isSelected ? Color3B::WHITE : Color3B(191, 191, 191));
            }
        }
        
        mCheckboxCreditValueBack->setEnabled(isSelected);
        mCheckboxClickViewPlayer->setEnabled(isSelected);
        mCheckboxSilenced->setEnabled(isSelected);
        mSliderManager->setEnabled(isSelected);
    }
}

void LayerGroupSetting::setLanguage(Node *node)
{
    std::string text_ch[] = {"俱乐部化管理", "公开玩家排行", "俱乐部名称",
                            "公开玩家排行","俱乐部分享","俱乐部备注",
                            "消息免打扰","清空聊天记录","退出/解散群",
                            "信用值返还","我的信用值","允许玩家查看群成员",
                            "群成员禁言","抽水比例","群主权限"
    };
    std::string text_name[]={"title_manager","title_rank","title_group_name",
                        "title_rank","title_share","title_remark",
                        "title_notifications","text_clean_chat_history","text_delete_leave",
                        "title_credit_back","title_own_credit","title_check_member",
                        "title_silence","title_manager_deduct","title_tip"
    };
    std::string text_en[] ={"club_management","show_player_records","group_name",
                            "public_rank","club_share","group_note",
                            "no_message","clear_mes_record","quit_group",
                            "return_credit","my_redit","allow_check_club_member",
                            "mute","take_a_share","group_advocate"
    };
    for(int i=0;i<15;i++)
    {
        printf("text_name is %s",text_name[i].c_str());
        auto title_setup_new = dynamic_cast<Text*>(CSLoader::seekNodeByName(node, text_name[i]));
        if(NULL !=title_setup_new)
        {
            title_setup_new->setString(tools::local_string(text_en[i],text_ch[i]));
        }
    }

}
//版本更改，下面方法将废弃； 先屏蔽，以防再改回来。。。
//#pragma mark - managerCell
//void LayerGroupSetting::managerCell(bool isShow)
//{
//    //俱乐部化管理cell数量
//    const int MAX_MANAGER_CELL = 5;
//    ssize_t index = mListviewCommonSetting->getIndex(mLayoutManager);
//    if(isShow)
//    {
//        isCreateManagerCell = true;
//        std::string text[] = {"信用值返还", "允许玩家查看群成员", "群成员禁言"};
//        bool isCheckboxSelect[] = {
//            mGroupManageSetting.creditrefillaftergame(),
//            mGroupManageSetting.canseeothermember(),
//            mGroupManageSetting.canchat(),
//            false
//        };
//        for(int i = 0; i != MAX_MANAGER_CELL - 2; ++i)
//        {
//            auto widget = mLayoutManager->clone();
//            auto title = dynamic_cast<Text*>(CSLoader::seekNodeByName(widget, "title_manager"));
//            auto checkboxCell = dynamic_cast<CheckBox*>(CSLoader::seekNodeByName(widget, "checkbox_manager"));
//            title->setString(text[i]);
//            checkboxCell->setTag(i);
//            checkboxCell->setSelected(isCheckboxSelect[i]);
//            checkboxCell->addEventListener(std::bind(&LayerGroupSetting::managerEventListener, this, std::placeholders::_1, std::placeholders::_2));
//            setChectboxTexture(checkboxCell);
//            mListviewCommonSetting->insertCustomItem(widget, i + index + 1);
//            
//            
//        }
//       
//        
//        //抽水比例 deductSlide
//        auto widget = mLayoutManager->clone();
//        auto title = dynamic_cast<Text*>(CSLoader::seekNodeByName(widget, "title_manager"));
//        auto fontSize = title->getFontSize();
//        auto checkboxCell = dynamic_cast<CheckBox*>(CSLoader::seekNodeByName(widget, "checkbox_manager"));
//        title->setString("抽水比例");
//        checkboxCell->removeFromParent();
//        auto slider = Slider::create("login_res/creategameroom_res/slider_cell_bg_2.png", "login_res/creategameroom_res/slider_cell_bg_1.png");
//        auto texture = "login_res/creategameroom_res/slider_cell_btn.png";
//        slider->loadSlidBallTextures(texture, texture, texture);
//        slider->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
//        slider->setPosition(Vec2(widget->getContentSize().width, widget->getContentSize().height / 2 + 8));
//        auto sizeScreen = Director::getInstance()->getVisibleSize();
//        slider->setContentSize(Size(sizeScreen.width/2, slider->getContentSize().height));
//        slider->ignoreContentAdaptWithSize(false);
//        slider->setPercent(0);
//        slider->addClickEventListener(std::bind(&LayerGroupSetting::deductSlide, this, std::placeholders::_1));
//        slider->addEventListener(std::bind(&LayerGroupSetting::deductSlideEventListener, this, std::placeholders::_1, std::placeholders::_2));
//        auto sizeSlider = slider->getContentSize();
//        auto textMin = Text::create();
//        textMin->setString("0");
//        textMin->setFontSize(fontSize);
//        textMin->setName("text_min");
//        textMin->setPosition(Size(0,  -sizeSlider.height/2));
//        textMin->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//        slider->addChild(textMin);
//        
//        auto textMax = Text::create();
//        textMax->setString("10");
//        textMax->setFontSize(fontSize);
//        textMax->setName("text_max");
//        textMax->setPosition(Size(sizeSlider.width,  -sizeSlider.height/2));
//        textMax->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
//        slider->addChild(textMax);
//        
//        auto textPercent = Text::create();
//        textPercent->setString("0%");
//        textPercent->setFontSize(fontSize);
//        textPercent->setName("text_percent");
//        textPercent->setPosition(Vec2(sizeSlider.width/2, sizeSlider.height * 2));
//        slider->addChild(textPercent);
//        
//        widget->addChild(slider);
//        mListviewCommonSetting->insertCustomItem(widget, index + MAX_MANAGER_CELL-1);
//        
//        createCellCreditValue(MAX_MANAGER_CELL + index);
//        
//        
//        
//    }
//    else
//    {
//        if(isCreateManagerCell)
//        {
//            isCreateManagerCell = false;
//            for(int i = 0; i != MAX_MANAGER_CELL; ++i)
//            {
//                mListviewCommonSetting->removeItem(1 + index);
//            }
//        }
//        
//    }
//   
//    updateCommonSetting();
//}
//void LayerGroupSetting::createCellCreditValue(ssize_t index)
//{
//    //我的信用值
//    auto layoutCreditValue = mLayoutManager->clone();
//    auto titleCreditValue = dynamic_cast<Text*>(CSLoader::seekNodeByName(layoutCreditValue, "title_manager"));
//    auto fontSize = titleCreditValue->getFontSize();
//    auto checkboxCreditValue = dynamic_cast<CheckBox*>(CSLoader::seekNodeByName(layoutCreditValue, "checkbox_manager"));
//    titleCreditValue->setString("我的信用值");
//    checkboxCreditValue->removeFromParent();
//    auto textCreditValue = Text::create();
//    textCreditValue->setString(tools::anything2string(mPermissionOwn.credit()));
//    textCreditValue->setFontSize(fontSize);
//    textCreditValue->setPosition(Size(layoutCreditValue->getContentSize().width,  layoutCreditValue->getContentSize().height/2));
//    textCreditValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
//    layoutCreditValue->addChild(textCreditValue);
//    mListviewCommonSetting->insertCustomItem(layoutCreditValue, index);
//}

#pragma mark - createMember
/*
 mLayoutMember：高度和位置根据人数动态改变一切
 mLayoutMember 总高度 = （行数+1） * （headHeight + 固定行间隔） + 临时设置界面的高度。
 mLayoutAllHead:内容高度 = mLayoutMember的内容 + mGroupHeadSize内容高度
 mListviewAllHead：内容高度 = 位置高度 = mLayoutAllHead 的内容高度；
 */
void LayerGroupSetting::createMember()
{
    if(mGroupNum <= 0)
    {
        return;
    }
    int num = (int)mGroupNum + BUTTON_NUM;
    isRemoving = false;
    mRow = std::max(1,(num - 1) / MAX_COL);//得到显示的行数, index从0开始
    mPreWidth = (mLayoutMemberSize.width - mBgOpacitySize.width) / (MAX_COL + 1) ; //平均width
    mPreHeight = mImageHeadSize.height; //平均height
    //总高度//高度加上背景框底边40的像素透明高度
    mLayoutMemberSize.height = (mRow + 1) * (mPreHeight + SPACE_HEIGHT) + mInsertHeight + mBgOpacitySize.height;
    
   
    auto eventDispatcher = getEventDispatcher();
    if(mListener)
    {
        eventDispatcher->removeEventListener(mListener);
    }
    mListener = EventListenerTouchOneByOne::create();
//    mListener->setSwallowTouches(true);//不向下传递触摸
    mListener->onTouchBegan = [&](Touch *touch, Event *event)
    {
        
        mTouchPosition = touch->getLocation();
        auto spriteHead = dynamic_cast<Sprite*>(event->getCurrentTarget());
        auto size = spriteHead->getContentSize();
        auto rect = cocos2d::Rect(0, 0, size.width, size.height);
        auto locationInNode = spriteHead->convertToNodeSpace(mTouchPosition);
        if(rect.containsPoint(locationInNode))
        {
            
            isScroll = false;
            return true;
        }
        
        return false;
    };
    
    
    mListener->onTouchMoved = [&](Touch *touch, Event *event)
    {
        float MAX_RANGE = 10;
        auto touchPos = touch->getLocation();
        if(abs(touchPos.x - mTouchPosition.x) > MAX_RANGE || abs(touchPos.y - mTouchPosition.y) > MAX_RANGE)
        {
            isScroll = true;
        }
    };
    
    mListener->onTouchEnded = std::bind(&LayerGroupSetting::headIconOnTouchEnded, this, std::placeholders::_1, std::placeholders::_2);

    
    //摆头像
    int preX = (mLayoutMemberSize.width - mPreWidth * MAX_COL) / 2 + mBgOpacitySize.width; //头像x开始的位置
    for(int i = 0; i != mGroupNum; ++i)
    {
        auto user = mGroupUserInfo[i].userdetailinfo();
        auto imgName = user.picname();
        auto nickName = user.nickname();
        if(imgName == "")
        {
            imgName = "Default/image_defualt_user_icon_small.png";
        }
        ShaderSprite *imgIcon = ShaderSprite::createAndDownload(imgName, "Default/image_defualt_user_icon_small.png", "Default/image_mask_circle.png");

        float x = (i%MAX_COL) * (mPreWidth + SPACE_WIDTH);
        int row = i / MAX_COL;
        float y = row * (mPreHeight + SPACE_HEIGHT) + (row > mInsertRow ? mInsertHeight : 0);
        log("===========> mRow = %d, tmpRow = %d, headWidth = %2f", mRow, i/MAX_COL, imgIcon->getContentSize().width);
        imgIcon->setTag(i);
        imgIcon->setPosition(Vec2(x + preX,  mLayoutMemberSize.height - y - mImageHeadSize.height/2));
        mLayoutMember->addChild(imgIcon);
        
       
        //点击成员头像事件
        eventDispatcher->addEventListenerWithSceneGraphPriority(i == 0 ? mListener : mListener->clone(), imgIcon);
        
        //名字
        auto name = Text::create();
        name->setString(GameTools::stringOverflow(nickName, 4));
        name->setFontSize(26);
        name->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        name->setPosition(Vec2(imgIcon->getPositionX() , imgIcon->getPositionY() - 30 - mImageHeadSize.height / 2));
        name->setColor(Color3B::WHITE);
        mLayoutMember->addChild(name);
        
    }
    
    auto isManageOpen = mGroupManageSetting.isclub();
    auto isChinese = GameTools::isChineseLanguage();
    if(isChinese || !isManageOpen || (isManageOpen && isManagerOrMaster()))
    {
        auto buttonAdd = Button::create("image_club/club_setting_add.png","image_club/club_setting_add_press.png");
        float x = (mGroupNum) % MAX_COL * (mPreWidth + SPACE_WIDTH);
        int row = (int)mGroupNum / MAX_COL;
        float y = row * (mPreHeight + SPACE_HEIGHT) + (row > mInsertRow ? mInsertHeight : 0);
        buttonAdd->setTouchEnabled(true);
        buttonAdd->setPosition(Vec2(x + preX,  mLayoutMemberSize.height - y -  mImageHeadSize.height/2));
        buttonAdd->addClickEventListener([this](Ref *ref)
                                         {
                                             if(isRemoving)
                                             {
                                                 removeRemoveButton();
                                                 return;
                                             }
                                             
                                             layerFriendList();
                                         });
        mLayoutMember->addChild(buttonAdd);
        
    }
    
    
    // 俱乐部主或管理员[-]号
    if(isManagerOrMaster())
    {
        auto buttonDelete = Button::create("image_club/club_setting_delete.png","image_club/club_setting_delete_press.png");
        buttonDelete->setTouchEnabled(true);
        float x = ((1 + mGroupNum) % MAX_COL) * (mPreWidth + SPACE_WIDTH);
        int row = (int)(1 + mGroupNum) / MAX_COL;
        float y = row * (mPreHeight + SPACE_HEIGHT) + (row > mInsertRow ? mInsertHeight : 0);
        buttonDelete->setPosition(Vec2(x + preX,  mLayoutMemberSize.height - y -  mImageHeadSize.height/2));
        buttonDelete->addClickEventListener([this](Ref *ref)
                                            {
                                                if(isRemoving)
                                                {
                                                    removeRemoveButton();
                                                    return;
                                                }
                                                createRemoveButton();
                                                
                                            });
        mLayoutMember->addChild(buttonDelete);
    }
    
    
    //动态头像layout
    mLayoutMember->setContentSize(mLayoutMemberSize);
    //整个头像layout
    mLayoutAllHead->setContentSize(cocos2d::Size(mLayoutMemberSize.width, mLayoutMemberSize.height +  mGroupHeadSize.height));
    mListviewAllHead->setContentSize(mLayoutAllHead->getContentSize());
    mListviewAllHead->setPositionY(mLayoutAllHead->getContentSize().height);
    mListviewAll->refreshView();
}


#pragma mark - 创建权限设置/新人模板
void LayerGroupSetting::createPermissionSetting(Vec2 pos)
{
    log("worldPosition (%2f, %2f)",pos.x, pos.y);
    //显示高度不够，移动到listview显示的下得高度
    if(pos.y < MANAGER_SETTING_HEIGHT)
    {
        mListviewAll->moveChildren(0, MANAGER_SETTING_HEIGHT - pos.y);
        pos.y += MANAGER_SETTING_HEIGHT - pos.y - mBgOpacitySize.height;
    }
    mPremissionRoot = CSLoader::createNode("LayerGroupSettingChild.csb");
    
    auto title_personal_info=dynamic_cast<Text*>(CSLoader::seekNodeByName(mPremissionRoot, "title_personal_info"));
    auto title_setup_adm=dynamic_cast<Text*>(CSLoader::seekNodeByName(mPremissionRoot, "title_setup_adm"));
    auto title_credit_value=dynamic_cast<Text*>(CSLoader::seekNodeByName(mPremissionRoot, "title_credit_value"));
    auto btn_personal_info=dynamic_cast<Button*>(CSLoader::seekNodeByName(mPremissionRoot, "btn_personal_info"));
    title_personal_info->setString(tools::local_string("check_peseronal_info","查看个人资料"));
    title_setup_adm->setString(tools::local_string("set_admin","设置管理员"));
    title_credit_value->setString(tools::local_string("set_credit_amount","设置信用额度"));
    btn_personal_info->setTitleText(tools::local_string("check_info","查看"));
    
    auto imgCursor = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(mPremissionRoot, "img_cursor_move"));//游标位置
    mListviewHeadSetting = dynamic_cast<ListView*>(CSLoader::seekNodeByName(mPremissionRoot, "listview"));
    auto childrens = mListviewHeadSetting->getChildren();
    for (auto children : childrens)
    {
        auto layout = dynamic_cast<Layout*>(children);
        if(layout)
        {
            layout->addClickEventListener([&](Ref *ref)
                                          {
                                              GameTools::editBoxLoseInputFocus();
                                          });
        }
    }
    mListviewHeadSetting->setPositionY(pos.y - mImageHeadSize.height / 2);//listview位置锚点(0.5,1.0)
    mInsertHeight = mListviewHeadSetting->getContentSize().height;
    imgCursor->setPositionX(pos.x - mImageHeadSize.width / 4);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [](Touch *touch, Event *event)
    {
        GameTools::editBoxLoseInputFocus();
        return true;
    };
    listener->onTouchEnded = [&](Touch *touch, Event *event)
    {
        
        log("关闭设置界面");
        mInsertHeight = 0;
        getEventDispatcher()->removeEventListenersForTarget(mLayerPermissionSetting);
        mLayerPermissionSetting->removeFromParent();
        mLayerPermissionSetting = nullptr;
        if(mPremissionSetting == PremissionSetting::MEMBER)
        {
            updateMember();
        }
    };
    listener->setSwallowTouches(true);//不向下传递触摸
    
    mLayerPermissionSetting = Layer::create();
    mLayerPermissionSetting->setContentSize(Director::getInstance()->getVisibleSize());
    mLayerPermissionSetting->addChild(mPremissionRoot);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,mLayerPermissionSetting);
    addChild(mLayerPermissionSetting);
    
   
    //改信用额度
    auto textfieldCreditValue = dynamic_cast<TextField*>(CSLoader::seekNodeByName(mPremissionRoot, "textfield_credit_value"));//信用值
    mImageCreditBg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(mPremissionRoot, "img_credit_value_bg"));
    
    mEditboxCreditValue = GameTools::createEditBoxFromTextField(mImageCreditBg, textfieldCreditValue);
    mEditboxCreditValue->setTag(2);
    mEditboxCreditValue->setInputMode(ui::EditBox::InputMode::NUMERIC);
    mEditboxCreditValue->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    mEditboxCreditValue->setDelegate(this);
    mEditboxCreditValue->setFontColor(Color4B::WHITE);
    mEditboxCreditValue->setTouchEnabled(true);
    mEditboxCreditValue->setMaxLength(TITLE_MAX_LENGHT);
    
    //管理员不能更改群主信用值
    if(mGroupUserStatus == msg::GroupUserStatus::GROUP_ADMIN && mGroupUserInfo[mSelectTag].userstatus() == msg::GroupUserStatus::GROUP_OWNER)
    {
        mImageCreditBg->setVisible(false);
    }
    
    if(mPremissionSetting == PremissionSetting::MEMBER)
    {
        requestUserPermission(mMemberId);
    }
    else
    {
        updataPermissionSetting();
    }
      
    
}

#pragma mark - 更新权限设置/新人模板
void LayerGroupSetting::updataPermissionSetting()
{
    if(!mLayerPermissionSetting)
    {
        return;
    }
    auto btnPersonalInfo = dynamic_cast<Button*>(CSLoader::seekNodeByName(mPremissionRoot, "btn_personal_info"));//个人资料
    auto checkboxSetupAdm = dynamic_cast<CheckBox*>(CSLoader::seekNodeByName(mPremissionRoot, "checkbox_setup_adm"));//设置管理员
    auto layoutSetupAdm = dynamic_cast<Layout*>(CSLoader::seekNodeByName(mPremissionRoot, "layout_setup_adm"));//设置管理员cell
    
    
    checkboxSetupAdm->setTag(0);
    checkboxSetupAdm->addClickEventListener(std::bind(&LayerGroupSetting::checkboxPerssiomEvent, this, std::placeholders::_1));
    //俱乐部员权限
    if(mPremissionSetting == PremissionSetting::MEMBER)
    {
        
        btnPersonalInfo->addClickEventListener([this](Ref *ref)
                                                    {
                                                        log("个人资料");
                                                        layerUserInfo(mGroupUserInfo[mSelectTag].userdetailinfo());
                                                    });
        //updata data
        if(!isMaster())
        {
            layoutSetupAdm->removeFromParent();
        }
        else
        {
            checkboxSetupAdm->setSelected(mGroupUserInfo[mSelectTag].userstatus() == msg::GroupUserStatus::GROUP_ADMIN);
        }
        mEditboxCreditValue->setText(tools::anything2string(mPermissionMember.credit()).c_str());
        setChectboxTexture(checkboxSetupAdm);
    }
    
}
void LayerGroupSetting::setChectboxTexture(CheckBox *checkbox)
{
    if(checkbox->isSelected())
    {
        
        checkbox->loadTextures(CHECKBOX_SELECT, CHECKBOX_SELECT, CHECKBOX_SELECT, CHECKBOX_SELECT, CHECKBOX_SELECT);
    }
    else
    {
        checkbox->loadTextures(CHECKBOX_UNSELECT, CHECKBOX_UNSELECT, CHECKBOX_UNSELECT, CHECKBOX_UNSELECT, CHECKBOX_UNSELECT);
    }
       
}

bool LayerGroupSetting::isManagerOrMaster()
{
    return mGroupUserStatus == msg::GroupUserStatus::GROUP_ADMIN || mGroupUserStatus == msg::GroupUserStatus::GROUP_OWNER;
}
bool LayerGroupSetting::isMaster()
{
    return mGroupUserStatus == msg::GroupUserStatus::GROUP_OWNER;
}

#pragma mark - 更新俱乐部信息
void LayerGroupSetting::updateGroupInfo()
{
    //名字头像
    auto imgName = mGroupInfo.grouppic();
    setGroupName(mGroupInfo.groupname());
    mImageHead->setSpriteTexture(imgName);

    
    bool iMaster = isMaster();
    auto isChinese = GameTools::isChineseLanguage();
    
    if(isChinese)
    {
        if(mLayoutManager)
        {
            mLayoutManager->removeFromParent();
            mLayoutManager = nullptr;
        }
        mSizeLayoutManager = Size::ZERO;
    }
    else
    {
//        mLayoutManager->setScale(100);
        initManagerNode();
    }
   
    if(iMaster)
    {
        
        mCheckboxRank->setSelected(mGroupInfo.rankvisible());
        setChectboxTexture(mCheckboxRank);
        auto textQuit = dynamic_cast<Text*>(CSLoader::seekNodeByName(mRoot, "text_delete_leave"));
        if(textQuit)
        {
            textQuit->setString(tools::local_string("quit_group_owner"));
        }
    }
    else
    {
        if(mLayoutRank)
        {
            mLayoutRank->removeFromParent();
            mLayoutRank = nullptr;
        }
    }
   
    
    bool isOpen = isManagerOrMaster();
    mTextRemark->setString(mGroupInfo.groupcomment().empty() ? tools::local_string("nono_group","(无)") : mGroupInfo.groupcomment());
    mEditBoxGroupName->setTouchEnabled(isOpen);
    mEditBoxRemark->setTouchEnabled(isOpen);
    mCheckboxNotification->setSelected(isNotifications);
    setChectboxTexture(mCheckboxNotification);
    updateCommonSetting();
}




void LayerGroupSetting::updateMember()
{
    mLayoutMember->removeAllChildren();
    createMember();
}

void LayerGroupSetting::updateCommonSetting()
{
    ssize_t childrenCount = mListviewCommonSetting->getChildrenCount();
    mLayoutCommonSetting->setContentSize(Size(mLayoutCommonSetting->getContentSize().width, mGroupNameCellSize.height * (childrenCount + 1)));
    mListviewCommonSetting->setContentSize(Size(mListviewCommonSetting->getContentSize().width, mLayoutCommonSetting->getContentSize().height));
    mListviewCommonSetting->setPositionY(mLayoutCommonSetting->getContentSize().height - mBgOpacitySize.height / 2);
    mListviewAll->refreshView();
    
}

void LayerGroupSetting::setGroupName(const std::string name)
{
    auto strNum = StringUtils::format("(%zu)", mGroupNum);
    mTextGroupName->setString(name);
    mTitleGroupName->setString(name + strNum);
}

#pragma mark - headIconOnTouchEnded 点击头像
void LayerGroupSetting::headIconOnTouchEnded(Touch *touch, Event *event)
{
 
    if(isScroll)
    {
        return;
    }
    
    if(isRemoving)
    {
        removeRemoveButton();
        return;
    }
    
    mPremissionSetting = PremissionSetting::MEMBER;
    ShaderSprite *spriteHead = dynamic_cast<ShaderSprite*>(event->getCurrentTarget());
    mSelectTag = spriteHead->getTag();
    mMemberId = mGroupUserInfo[mSelectTag].userdetailinfo().userid();
    auto chinese = GameTools::isChineseLanguage();
    auto  isOpen = mGroupManageSetting.isclub();
    auto iManagerOrMaster = isManagerOrMaster();
    //直接到个人资料
    if(!isOpen || chinese || !iManagerOrMaster)
    {
        layerUserInfo(mGroupUserInfo[mSelectTag].userdetailinfo());
        return;
    }
    
    mInsertRow = mSelectTag / MAX_COL; //点击行数
    
    auto pos = spriteHead->convertToWorldSpace(Vec2::ZERO);//转世界坐标
    createPermissionSetting(pos);
    updateMember();
}

#pragma mark - managerClickEventListener
void LayerGroupSetting::managerEventListener(Ref *ref, CheckBox::EventType type)
{
    if(!isMaster())
    {
        Toast::ToastShow(tools::local_string("only_group_advocate","只有群主才能操作"));
        return;
    }
    
    auto checkbox = dynamic_cast<CheckBox*>(ref);
    int tag = checkbox->getTag();
    
    if(tag != 0 && !mGroupManageSetting.isclub())
    {
//        Toast::ToastShow("只有群主才能操作");
        return;
    }
    log("tag = %d is %s", tag, type==CheckBox::EventType::SELECTED ? "selected" : "unSelected");
    setChectboxTexture(checkbox);
    requestManager(checkbox);
    
}

//抽水
void LayerGroupSetting::deductSlide(Ref *ref)
{
    auto slider = dynamic_cast<Slider*>(ref);
    int percent = (int)slider->getPercent();
    requestManagerRakePercent(percent / MAX_DEDUCT_PERCENT);
}

void LayerGroupSetting::deductSlideEventListener(Ref *ref, Slider::EventType type)
{
    auto slider = dynamic_cast<Slider*>(ref);
    int percent = (int)slider->getPercent() / MAX_DEDUCT_PERCENT;
    auto text = dynamic_cast<Text*>(slider->getChildByName("text_deduct_percent"));
    text->setString(tools::anything2string(percent) + "%");
}


#pragma mark - checkboxPressiomEvent
void LayerGroupSetting::checkboxPerssiomEvent(Ref *ref)
{
    auto checkbox = dynamic_cast<CheckBox*>(ref);
    auto isSelected = checkbox->isSelected();
    int tag = checkbox->getTag();
    //俱乐部员权限
    if(mPremissionSetting == PremissionSetting::MEMBER)
    {
        switch (tag)
        {
            case 0:
            {
                log("设置管理员");
                isSelected = mGroupUserInfo[mSelectTag].userstatus() == msg::GroupUserStatus::GROUP_MEMBER;
                requestBeAdministrator(mMemberId,isSelected);
            }
                break;
                
            default:
                break;
        }
    }
    checkbox->setSelected(isSelected);
    setChectboxTexture(checkbox);
}

#pragma mark - 踢人
void LayerGroupSetting::createRemoveButton()
{
    if(mGroupNum <= 0)
    {
        return;
    }
    uint32_t ownId = GDM->get_user_id();
    
    for(int i = 0; i < mGroupNum; ++i)
    {
        if(mGroupUserInfo[i].userdetailinfo().userid() == ownId || mGroupUserInfo[i].userstatus() == msg::GroupUserStatus::GROUP_OWNER)
        {
            continue;
        }
        auto imgIcon = mLayoutMember->getChildByTag(i);
        auto btnRemove = Button::create("image_club/club_btn_remove.png");
        btnRemove->setTag(MAX_MEMBER_NUM + i);
        btnRemove->setPosition(Vect(imgIcon->getPositionX()- mImageHeadSize.width / 2, imgIcon->getPositionY() + mImageHeadSize.height / 2));
        btnRemove->setTouchEnabled(true);
        btnRemove->addClickEventListener([this](Ref *ref)
                                         {
                                             auto tag = dynamic_cast<Button*>(ref)->getTag() - MAX_MEMBER_NUM;
                                             log("删除的头像是：%d", tag);
                                             auto layer = TipView::showAlertView(tools::local_string("sure_to_delete","确定删除?"), true);
                                             layer->set_tipview_close_callback([&, tag]()
                                                                               {
                                                                                   requestDelMember(mGroupUserInfo[tag].userdetailinfo().userid());
                                                                                   
                                                                               });
                                             
                                         });
        btnRemove->setScale(0.0f);
        btnRemove->runAction(ScaleTo::create(0.2f, 1.0f));
        mLayoutMember->addChild(btnRemove);
        
        
    }
    isRemoving = true;
}

void LayerGroupSetting::removeRemoveButton()
{
    isRemoving = false;
    for(int i = 0; i < mGroupNum; ++i)
    {
        auto btnRemove = mLayoutMember->getChildByTag(MAX_MEMBER_NUM + i);
        if(btnRemove != nullptr)
        {
            auto action = Sequence::createWithTwoActions(ScaleTo::create(0.2f,0.0f), CallFunc::create([btnRemove]()
                                                                                                      {
                                                                                                          btnRemove->removeFromParent();
                                                                                                      }));
            btnRemove->runAction(action);
        }
    }
    
}


void LayerGroupSetting::cleanGroupChatData()
{
    ChatMessageManager::getInstance()->clear_chat_record(mGroupId, ChatMessageManager::MESSAGETYPE::GROUPCHAT);
    //刷新聊天界面
    auto layerChat = SceneManager::get_layer_by_name(typeid(LayerChat).name());
    if(layerChat)
    {
        auto layer = dynamic_cast<LayerChat*>(layerChat);
        if(layer)
        {
            layer->update_chat_message_datas();
        }
    }
    
    TipView::showAlertView(tools::local_string("operate_successed","操作成功"));
}

void LayerGroupSetting::layerUserInfo(msg::UserDetailInfo info)
{
    addChild(LayerUserInfo::create(info));
}

void LayerGroupSetting::layerFriendList()
{
    std::vector<uint32_t> userId;
    for(auto iter : mGroupUserInfo)
    {
        userId.push_back(iter.userdetailinfo().userid());
        log("俱乐部邀请， 现有成员ID = %d", iter.userdetailinfo().userid());
    }
    auto layer = LayerFriendList::create(LayerFriendList::Type::SELECT_MULTIPLE, userId);
    layer->set_select_callback(std::bind(&LayerGroupSetting::requestAddMember, this, std::placeholders::_1));
    addChild(layer, 0, "LayerFriendList");
}

#pragma mark - EditBoxDetegate
void LayerGroupSetting::editBoxEditingDidBegin(EditBox* editBox)
{
    int tag = editBox->getTag();
    switch (tag) {
        case 0:
            mTextGroupName->setVisible(false);
            break;
        case 1:
            mTextRemark->setVisible(false);
        default:
            break;
    }
}

void LayerGroupSetting::editBoxEditingDidEnd(EditBox* editBox)
{
    log("editBoxEditingDidEnd(...) called, tag = %d, input = %s", editBox->getTag(), editBox->getText());
}

void LayerGroupSetting::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
    
}

void LayerGroupSetting::editBoxReturn(EditBox* editBox)
{
    if(isExitLayer)
    {
        return;
    }
    int tag = editBox->getTag();
    const char *text = editBox->getText();
    switch (tag) {
        case 0:
            
            mTextGroupName->setVisible(true);
            editBox->setText("");
            if(!GameTools::isRegexMath(text, GameTools::INPUT_TYPE::CN_EN_NUM_UL))
            {
                TipView::showAlertView(tools::local_string("language_format","只支持中文、英文、数字和下划线"));
                return;
            }
            requestEditGroupInfo(1,text);
            setGroupName(text);
            break;
        case 1:            
            mTextRemark->setVisible(true);
            editBox->setText("");
            if(!GameTools::isRegexMath(text, GameTools::INPUT_TYPE::CN_EN_NUM_UL))
            {
                TipView::showAlertView(tools::local_string("language_format","只支持中文、英文、数字和下划线"));
                return;
            }
            requestEditGroupInfo(2,text);
            mTextRemark->setString(text);
            break;
            
        case 2:
            if(!GameTools::isRegexMath(text, GameTools::INPUT_TYPE::NUMBER))
            {
                TipView::showAlertView(tools::local_string("support_number","只支持数字"));
                return;
            }
            if(mPremissionSetting == PremissionSetting::MEMBER)
            {
                //改信用额度
                requestCreditValue(mMemberId, tools::stoi(text));
            }
            break;
        default:
            break;
    }
    log("editBoxReturn(...) called, tag = %d, input = %s", tag, text);
    
}


#pragma mark - pickerImage
void LayerGroupSetting::pickerImage()
{
    LayerChoosePhoto* as = LayerChoosePhoto::create();
    as->setIsChat(false);
    as->set_choose_photo_callback([&](const std::string& filename)
                                  {
                                      log("上传头像 file = %s", filename.c_str());
                                      mStrUploadPickerImage = filename;
                                      FM.upload_file(filename);
                                  });
    as->set_dismiss_callback([this](){
        
    });
    this->addChild(as);
}
void LayerGroupSetting::RefreshLayer()
{
    requestGroupInfo();
}

#pragma mark - requestProtobuf
void LayerGroupSetting::register_events()
{
    
    dispatch_.register_event(REQ_GET_GROUP_DETAIL_INFO, BIND_FUNC(this, LayerGroupSetting::receiveGroupInfo));
    dispatch_.register_event(REQ_EDIT_GROUP, BIND_FUNC(this, LayerGroupSetting::receiveEditGroupInfo));
    dispatch_.register_event(REQ_CHANGE_GROUP_SETTING, BIND_FUNC(this, LayerGroupSetting::receiveIgonreNotifications));
    dispatch_.register_event(REQ_KICK_FROM_GROUP, BIND_FUNC(this, LayerGroupSetting::receiveDeleteLeave));
    dispatch_.register_event(EVENT_UPLOAD_FILE_SUCCESS, BIND_FUNC(this, LayerGroupSetting::receiveUploadFile));
    dispatch_.register_event(REQ_CHANGE_PERMISSION, BIND_FUNC(this, LayerGroupSetting::receiveGroupPermission));
    dispatch_.register_event(REQ_GET_PERMISSION, BIND_FUNC(this, LayerGroupSetting::receiveUserPermission));
    dispatch_.register_event(REQ_SET_IS_ADMIN, BIND_FUNC(this, LayerGroupSetting::receiveBeAdministrator));
    dispatch_.register_event(REQ_SET_GROUP_MANAGE_SETTING, BIND_FUNC(this, LayerGroupSetting::receiverManager));
    dispatch_.register_event(REQ_ADD_FRIEND, BIND_FUNC(this, LayerGroupSetting::receiveAddMember));
    
    
}

//请求俱乐部信息
void LayerGroupSetting::requestGroupInfo()
{
    auto processor = PM->GetGroupDetailInfo_UP(mGroupId);
    send_data(processor, REQ_GET_GROUP_DETAIL_INFO);
}


void LayerGroupSetting::requestManager(CheckBox *checkbox)
{
    isRefreshGroupInfo = false;
    bool isSelect = checkbox->isSelected();
    int tag = checkbox->getTag();
    msg::GroupManageSetting setting;
    switch (tag) {
        case 0:
            setting.set_isclub(isSelect);
            mGroupManageSetting.set_isclub(isSelect);
            setManagerColor(isSelect);
            log("请求管理：%s",isSelect ? "开" : "关");
            isRefreshGroupInfo = true;
            break;
        case 1: //信用返回
            setting.set_creditrefillaftergame(isSelect);
            mGroupManageSetting.set_creditrefillaftergame(isSelect);
            log("请求信用值返：%s",isSelect ? "开" : "关");
            break;
        case 2:
            setting.set_canseeothermember(isSelect);
            mGroupManageSetting.set_canseeothermember(isSelect);
            log("请求查看其它玩家：%s",isSelect ? "开" : "关");
            break;
        case 3:
            setting.set_canchat(!isSelect);//是否可以说话，开关跟服务器刚好相反
            mGroupManageSetting.set_canchat(isSelect);
            log("请求禁言：%s",isSelect ? "开" : "关");
            break;
        default:
            return;
    }
    
    auto processor = PM->SetGroupManageSetting_UP(mGroupId, setting);
    send_data(processor, REQ_SET_GROUP_MANAGE_SETTING);
}

void LayerGroupSetting::requestManagerRakePercent(int percent)
{
    msg::GroupManageSetting setting;
    setting.set_rakepercent(percent);
    auto processor = PM->SetGroupManageSetting_UP(mGroupId, setting);
    send_data(processor, REQ_SET_GROUP_MANAGE_SETTING);

}

//邀请玩家入俱乐部
void LayerGroupSetting::requestAddMember(std::vector<msg::UserDetailInfo> userDatailInfo)
{
    auto layer = this->getChildByName("LayerFriendList");
    if(layer)
    {
        layer->removeFromParent();
    }
    if(!userDatailInfo.empty())
    {
        msg::Processor_122_SendRequest processor;
        msg::Request request;
        uint32_t ownId = GDM->get_user_id();
        for(auto iter : userDatailInfo)
        {
            request.set_type(msg::RequestType::ENTERGROUP);
            request.set_id(mGroupId);
            request.mutable_groupinfo()->set_groupid(mGroupId);
            
            request.set_receiverid(iter.userid());
            request.mutable_userdetailinfo()->set_userid(ownId);
            processor.mutable_request()->Add()->CopyFrom(request);
        }
        send_data(processor, REQ_ADD_FRIEND);
    }
}

//删除玩家
void LayerGroupSetting::requestDelMember(uint32_t userId)
{
    isDeleteGroup = false;
    auto processor = PM->KickUserFromGroup_UP(mGroupId, userId);
    send_data(processor, REQ_KICK_FROM_GROUP);
}

//请求俱乐部编辑
void LayerGroupSetting::requestEditGroupInfo(int type, std::string content)
{
    msg::GroupInfo groupInfo;
    if(type == 0) //换头像
    {
        groupInfo.set_grouppic(content); //先上传图片，成功后（换头像）再发协议
    }
    else if(type == 1) //改俱乐部名
    {
        groupInfo.set_groupname(content);
    }
    else if(type == 2) //改备注
    {
        groupInfo.set_groupcomment(content);
    }
    else
    {
        return;
    }
    groupInfo.set_groupid(mGroupId);
    auto processor = PM->EditGroup_UP(groupInfo);
    send_data(processor, REQ_EDIT_GROUP);
}
//公开玩家排行榜
void LayerGroupSetting::requestPublicRank(bool isPublic)
{
    msg::GroupInfo groupInfo;
    groupInfo.set_rankvisible(isPublic);
    groupInfo.set_groupid(mGroupId);
    auto processor = PM->EditGroup_UP(groupInfo);
    send_data(processor, REQ_EDIT_GROUP);
}
//请求免打扰
void LayerGroupSetting::requestIgnoreNotifications(bool ignore)
{
    msg::GroupSetting setting;
    setting.set_groupid(mGroupId);
    setting.set_receivepush(ignore);
    auto processor = PM->changeGroupSetting_up(setting);
    send_data(processor, REQ_CHANGE_GROUP_SETTING);
}

//请求退出/解散俱乐部
void LayerGroupSetting::requestDeleteLeave()
{
    isDeleteGroup = true;
    auto processor = PM->KickUserFromGroup_UP(mGroupId, GDM->get_user_id());
    send_data(processor, REQ_KICK_FROM_GROUP);
}

//请求俱乐部成员信用值等权限
void LayerGroupSetting::requestUserPermission(uint32_t userId)
{
    auto processor = PM->GetUserPermission_up(mGroupId, userId);
    send_data(processor, REQ_GET_PERMISSION);
}

//请求是否管理员
void LayerGroupSetting::requestBeAdministrator(uint32_t userId, bool isAdmin)
{
    auto processor = PM->SetGroupAdmin_UP(mGroupId, userId, isAdmin);
    send_data(processor, REQ_SET_IS_ADMIN);
}


//请求信用额度
void LayerGroupSetting::requestCreditValue(uint32_t userId, uint32_t value)
{
    mPermissionMember.set_credit(value);
    msg::GroupPermission permission;
    permission.set_credit(value);
    permission.set_groupid(mGroupId);
    auto processor = PM->changePermission_up(userId, permission);
    send_data(processor, REQ_CHANGE_PERMISSION);
}


#pragma mark receiveProtobuf
//接收俱乐部信息
void LayerGroupSetting::receiveGroupInfo(cmd_data_pointer data)
{
    ClearError();
    shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec))
        return;
    
    auto processor = PM->GetGroupDetailInfo_Down(rec);
    auto response = processor.response();
    switch (response.status()) {
        case msg::Status::SUCCESS:
        {
            mGroupUserInfo.clear();
            auto groupUserInfo = processor.groupuserinfo();
            for(auto iter : groupUserInfo)
            {
                mGroupUserInfo.push_back(iter); //成员信息
            }
            mGroupNum = mGroupUserInfo.size(); //人数
            mGroupInfo = processor.groupinfo();//俱乐部信息
            mGroupUserStatus = mGroupInfo.userstatus(); //身份
            isNotifications = processor.setting().receivepush(); //消息免打扰
            mPermissionOwn = processor.permission(); //自己-信用额
            mGroupManageSetting = processor.managesetting();
            mGroupManageSetting.set_canchat(!mGroupManageSetting.canchat());//是否可以说话，开关跟服务器刚好相反
            log("下发数据：\n 管理员设置：%s\n 信用值返还: %s\n 群成员禁言: %s\n 允许玩家查看群成员: %s\n  抽水比例：%d \n",
                mGroupManageSetting.isclub() ? "开启" : "关闭",
                mGroupManageSetting.creditrefillaftergame() ? "开启" : "关闭",
                mGroupManageSetting.canchat() ? "开启" : "关闭",
                mGroupManageSetting.canseeothermember() ? "开启" : "关闭",
                mGroupManageSetting.rakepercent());
            
            
            updateGroupInfo();
            updateMember();
            
        }
            break;
        case msg::Status::FAILED:
            ShowError(response.message());
            break;
            
        default:
            break;
    }
}


void LayerGroupSetting::receiverManager(cmd_data_pointer data)
{
    ClearError();
    shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec))
        return;
    auto processor = PM->response_down(rec);
    switch (processor.status()) {
        case msg::Status::SUCCESS:
            log("群管理设置-成功");
            if(isRefreshGroupInfo)
            {
                isRefreshGroupInfo = false;
                requestGroupInfo();
            }
            break;
        case msg::Status::FAILED:
            log("群管理设置-失败");
            break;
            
        default:
            break;
    }
}



//接收俱乐部编辑
void LayerGroupSetting::receiveEditGroupInfo(cmd_data_pointer data)
{
    ClearError();
    shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec))
        return;
    
    auto processor = PM->response_down(rec);
    
    switch (processor.status()) {
        case msg::Status::SUCCESS:
        {
           
        }
            break;
        case msg::Status::FAILED:
            ShowError(processor.message());
            break;
            
        default:
            break;
    }
}

//接收免打扰
void LayerGroupSetting::receiveIgonreNotifications(cmd_data_pointer data)
{
    ClearError();
    shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec))
        return;
    
    auto processor = PM->response_down(rec);
    
    switch (processor.status()) {
        case msg::Status::SUCCESS:
        {
            isNotifications = !isNotifications;
            mCheckboxNotification->setSelected(isNotifications);
            setChectboxTexture(mCheckboxNotification);
            
        }
            break;
        case msg::Status::FAILED:
            ShowError(processor.message());
            break;
            
        default:
            break;
    }
}
//接收退出/解散俱乐部/踢人删除玩家
void LayerGroupSetting::receiveDeleteLeave(cmd_data_pointer data)
{
    ClearError();
    shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec))
        return;
    
    auto processor = PM->response_down(rec);
    
    switch (processor.status()) {
        case msg::Status::SUCCESS:
        {
            if(isDeleteGroup)
            {
                
                //删除聊天界面
                auto layerChat = SceneManager::get_layer_by_name(typeid(LayerChat).name());
                if(layerChat)
                {
                    layerChat->removeFromParent();
                }
                //刷新俱乐部列表
                auto layer = SceneManager::get_layer_by_name(typeid(LayerGroupList).name());
                if(layer)
                {
                    auto layerGroupList = dynamic_cast<LayerGroupList*>(layer);
                    if(layerGroupList)
                    {
                        layerGroupList->requestGroupList();
                    }
                }
                
                //退俱乐部
                this->removeFromParent();
            }
            else
            {
                //踢人
                requestGroupInfo();//请求刷新俱乐部信息
            }
        }
            break;
        case msg::Status::FAILED:
            Toast::ToastShow(processor.message());
            break;
            
        default:
            break;
    }
    isDeleteGroup = false;
}

//接收上传图片
void LayerGroupSetting::receiveUploadFile(cmd_data_pointer data)
{
    auto s = std::dynamic_pointer_cast<cmd_data_string>(data);
    if (s->get_value() != mStrUploadPickerImage) return;
    mImageHead->setTexture(mStrUploadPickerImage);
    auto size = mImageHead->getContentSize();
    mImageHead->setScale(mImageHeadSize.width/size.width, mImageHeadSize.height/size.height);
    requestEditGroupInfo(0, mStrUploadPickerImage);
    
}

void LayerGroupSetting::receiveAddMember(cmd_data_pointer data)
{
    ClearError();
    shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec))
        return;
    auto processor = PM->response_down(rec);
    switch (processor.status()) {
        case msg::Status::SUCCESS: {
            requestGroupInfo();//再请求更新群信息列表
            Toast::ToastShow(tools::local_string("operate_successed","操作成功"));
            break;
        case msg::Status::FAILED:
            Toast::ToastShow(tools::local_string("operate_failed","操作失败"));
            break;
        default:
            break;
        }
    }
}

//接收管理员
void LayerGroupSetting::receiveBeAdministrator(cmd_data_pointer data)
{
    ClearError();
    shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec))
        return;
    auto processor = PM->response_down(rec);
    switch (processor.status()) {
        case msg::Status::SUCCESS: {
//            TipView::showAlertView("操作成功");
            Toast::ToastShow(tools::local_string("operate_successed","操作成功"));
            if(mGroupUserInfo[mSelectTag].userstatus() == msg::GroupUserStatus::GROUP_MEMBER)
            {
                mGroupUserInfo[mSelectTag].set_userstatus(msg::GroupUserStatus::GROUP_ADMIN);
            }
            else
            {
                mGroupUserInfo[mSelectTag].set_userstatus(msg::GroupUserStatus::GROUP_MEMBER);
            }
            updataPermissionSetting();
            break;
        }
        case msg::Status::FAILED:
            Toast::ToastShow(tools::local_string("operate_failed","操作失败"));
            break;
        default:
            break;
    }
    
}


//接收成员权限
void LayerGroupSetting::receiveUserPermission(cmd_data_pointer data)
{
    shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec)) return;
    auto processor = PM->GetUserPermission_down(rec);
    
    mPermissionMember = processor.permission();
    log("接收成员权限");
    
//    for(auto iter : processor.permission())
//    {
//        mPermissionMember = iter;
//        log("接收成员权限");
//    }
    updataPermissionSetting();
    
}


//接收修改成员的权限
void LayerGroupSetting::receiveGroupPermission(cmd_data_pointer data)
{
    ClearError();
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec)) return;
    auto processor = PM->response_down(rec);
    switch (processor.status()){
        case msg::Status::SUCCESS:
            Toast::ToastShow(tools::local_string("operate_successed","操作成功"));
            break;
        case msg::Status::FAILED:
            Toast::ToastShow(tools::local_string("operate_failed","操作失败"));
            break;
        default:
            break;
    }
}



