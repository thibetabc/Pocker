//
//  LayerLanguageSetting.cpp
//  pokerclient
//
//  Created by HuangFeng on 16/5/20.
//
//

#include "LayerLanguageSetting.h"
#include "GameTools.h"
#include "utils.h"
#include "TipView.h"
#include "GameDataManager.h"

LayerLanguageSetting::LayerLanguageSetting() : lastIndex(0)
{
    
}
LayerLanguageSetting::~LayerLanguageSetting()
{
    
}

bool LayerLanguageSetting::init()
{
    if(!BaseLayer::init())
    {
        return false;
    }
    auto sysLanguage = PlatformSdk::getInstance()->getCurrentLanguage();
    initData();
    auto root = CSLoader::createNode("LayerLanguageSetting.csb");
    auto btnBack = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    auto listview = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root, "listview"));
    auto text_title=dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_title"));
    mTextConfirm = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_confirm"));
    text_title->setString(tools::local_string("choose_language","选择语言"));
    mTextConfirm->setString(tools::local_string("confirm_change_password","确定"));
    
    btnBack->addClickEventListener([&](Ref *ref)
    {
        this->removeFromParent();
    });
    mTextConfirm->setTouchEnabled(true);
    mTextConfirm->setVisible(false);
    mTextConfirm->addClickEventListener(std::bind(&LayerLanguageSetting::confirmClickEvent, this, std::placeholders::_1));
    
    auto tableview = TableView::create(this, listview->getContentSize());
    tableview->setAnchorPoint(listview->getAnchorPoint());
    tableview->setContentOffset(Vec2(0, 0));
    tableview->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview->setPosition(listview->getPosition());
    tableview->setDelegate(this);
    tableview->setClippingToBounds(true);
    tableview->setClippingEnabled(true);
    tableview->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::NONE);
    root->addChild(tableview);
    listview->removeFromParent();
    addChild(root);
    return true;
}

void LayerLanguageSetting::initData()
{
    //name和type要正确匹配
    std::string name[] = {"中文","繁体中文", "English"};
    PlatformSdk::LanguageType type[] =
    {
        PlatformSdk::LanguageType::CHINESE,
        PlatformSdk::LanguageType::TRADITIONAL_CHINESE,
        PlatformSdk::LanguageType::ENGLISH
    };
    int size = sizeof(name) / sizeof(name[0]);
    for(int i = 0; i != size; ++i)
    {
        SelectLanguage selectLanguage;
        selectLanguage.name = name[i];
        selectLanguage.isSelect = false;
        selectLanguage.type = type[i];
        mVectorLanguage.push_back(selectLanguage);
    }
    
}

void LayerLanguageSetting::confirmClickEvent(Ref *ref)
{
    auto languageType = (int)mVectorLanguage[lastIndex].type;
    auto lastType = GameTools::getLanguage();
    if(languageType != lastType)
    {
        TipView* tip = TipView::showAlertView(tools::local_string("change_langage","改变当前语言？"));
        tip->show_cancel(true);
        tip->set_tipview_close_callback([&, languageType]()
        {
            GameTools::setLanguage(languageType);
            tools::reset_current_language_file();
            GDM->request_change_language_room();
            SceneManager::addMainLayer();
        });
    }
    else
    {
        this->removeFromParent();
    }
}


#pragma mark - TableviewDatasource
TableViewCell* LayerLanguageSetting::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *tableViewCell = dynamic_cast<TableViewCell *>(table->dequeueCell());
    if(!tableViewCell)
    {
        
//        auto layout = Layout::create();
//        layout->setContentSize(Size(DESIGN_SIZE, TABLEVIEW_CELL_HEIGHT));
//        tableViewCell->addChild(layout);
        
        auto imageview = ImageView::create("image_club/club_group_bg.png");
        imageview->setName("imageview");
        imageview->setContentSize(Size(DESIGN_WIDTH, TABLEVIEW_CELL_HEIGHT));
        imageview->setScale9Enabled(true);
        imageview->setPosition(Size(DESIGN_WIDTH / 2, TABLEVIEW_CELL_HEIGHT/2 - 13));
        
        
        tableViewCell = new TableViewCell();
        tableViewCell->autorelease();
        tableViewCell->addChild(imageview);
        
        auto name = Text::create();
        name->setString(mVectorLanguage[idx].name);
        name->setFontSize(26);
        name->setColor(Color3B::WHITE);
        name->setPosition(Vec2(DESIGN_WIDTH/6, TABLEVIEW_CELL_HEIGHT/2));
        tableViewCell->addChild(name);
        
        Sprite *spSelect = Sprite::create("login_res/selftrackrecord/btn_choose_status.png");
        spSelect->setPosition(Vec2(imageview->getContentSize().width - DESIGN_WIDTH/6, TABLEVIEW_CELL_HEIGHT/2));
        spSelect->setName("sprite");
        tableViewCell->addChild(spSelect);
    }
    auto imageview = dynamic_cast<ImageView*>(tableViewCell->getChildByName("imageview"));
    auto spriteSelect = dynamic_cast<Sprite*>(tableViewCell->getChildByName("sprite"));
    imageview->loadTexture("image_club/club_group_bg.png");
//    imageview->loadTexture("chat_res/btn_record_normal.png");
    spriteSelect->setVisible(mVectorLanguage[idx].isSelect);
    
    return tableViewCell;
}

ssize_t LayerLanguageSetting::numberOfCellsInTableView(TableView *table)
{
    return mVectorLanguage.size();
}
Size LayerLanguageSetting::cellSizeForTable(TableView *table)
{
    return Size(DESIGN_WIDTH, TABLEVIEW_CELL_HEIGHT);
};

#pragma mark - TableviewDelegate
void LayerLanguageSetting::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
    auto lastTableViewCell = table->cellAtIndex(lastIndex);
    if(lastTableViewCell)
    {
        auto lastSpriteSelect = dynamic_cast<Sprite*>(lastTableViewCell->getChildByName("sprite"));
        lastSpriteSelect->setVisible(false);
        mVectorLanguage[lastIndex].isSelect = false;
    }
    auto idx = cell->getIdx();
    auto spriteSelect = dynamic_cast<Sprite*>(cell->getChildByName("sprite"));
    mVectorLanguage[idx].isSelect = true;
    mTextConfirm->setVisible(true);
    spriteSelect->setVisible(true);
    if(lastIndex != idx)
    {
        lastIndex = idx;
    }
}

void LayerLanguageSetting::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    auto imageview = dynamic_cast<ImageView*>(cell->getChildByName("imageview"));
    imageview->loadTexture("image_club/club_group_bg_press.png");
}

void LayerLanguageSetting::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    
    auto imageview = dynamic_cast<ImageView*>(cell->getChildByName("imageview"));
    imageview->loadTexture("image_club/club_group_bg.png");
}





















