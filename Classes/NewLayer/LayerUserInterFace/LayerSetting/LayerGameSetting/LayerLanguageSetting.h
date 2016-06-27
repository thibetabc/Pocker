//
//  LayerLanguageSetting.h
//  pokerclient
//
//  Created by HuangFeng on 16/5/20.
//
//

#ifndef LayerLanguageSetting_h
#define LayerLanguageSetting_h
#include "SceneManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "PlatformSdk.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerLanguageSetting : public BaseLayer, public TableViewDataSource, public TableViewDelegate
{
public:
    LayerLanguageSetting();
    ~LayerLanguageSetting();
    CREATE_FUNC(LayerLanguageSetting);
    virtual bool init();
    
private:
    void initData();
    void confirmClickEvent(Ref *ref);
#pragma mark - TableviewDatasource
    virtual Size cellSizeForTable(TableView *table);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
#pragma mark - TableviewDelegate
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell);
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);
    
private:
    typedef struct SelectLanguage
    {
        std::string name;
        bool isSelect;
        PlatformSdk::LanguageType type;
    };
    
    std::vector<SelectLanguage> mVectorLanguage;
    size_t lastIndex;
    Text *mTextConfirm;
    const int TABLEVIEW_CELL_HEIGHT = 160;
    
};

#endif /* LayerLanguageSetting_h */
