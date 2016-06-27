//
//  LayerMahjongReminder.hpp
//  pokerclient
//
//  Created by zhongyu on 16/6/12.
//
//

#ifndef LayerMahjongReminder_hpp
#define LayerMahjongReminder_hpp

#include "SceneManager.h"
#include "GameLayer.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerMahjongReminder : public Node
{
public:
    CREATE_FUNC(LayerMahjongReminder);
    virtual bool init();
    
    void refresh_data();
    void create_node(int num);
    void set_data(std::vector<msg::MahjongTing> &ting);
    int get_num(){return reminder_num_;}
private:
    int reminder_num_ = 0 ;
    ImageView* image_bg_;
    ImageView* image_hu_;
    std::vector< float > scale_X_ = {0.4,0.7,0.95,1.2,1.38,1.38,1.38,1.38,1.38};
    std::vector< float > scale_Y_ = {0.5,0.5,0.5 ,0.5 ,0.5,1,1,1,1};
    std::vector< std::vector< Point > > pos_ = {
        {Point(70,300)},
        {Point(80,260),Point(230,260)},
        {Point(70,260),Point(220,260),Point(369,260)},
        {Point(70,260),Point(218,260),Point(365,260),Point(513,260)},
        {Point(70,260),Point(206,260),Point(342,260),Point(478,260),Point(614,260)}
    };
    
    
    
};

#endif /* LayerMahjongReminder_hpp */
