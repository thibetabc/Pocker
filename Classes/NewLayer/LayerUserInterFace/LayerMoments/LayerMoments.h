//
//  LayerMoments.hpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/11.
//
//

#ifndef LayerMoments_hpp
#define LayerMoments_hpp

#include <stdio.h>
#include "SceneManager.h"
enum CIRCLETYPE
{
    TYPE_SQUARE = 0,
    TYPE_RECOMMEND = 1,
    TYPE_FRIEND = 2,
    TYPE_MINE = 3
};

class LayerMoments : public BaseLayer
{
protected:
    void register_events();
    void handle_get_moment(cmd_data_pointer data);
    void handle_delete_moment(cmd_data_pointer data);
    void handle_publish_comment(cmd_data_pointer data);
    void handle_get_comment(cmd_data_pointer data);
public:
    CREATE_FUNC(LayerMoments);
    virtual bool init();
private:
    //朋友圈数据保存
    CIRCLETYPE circletype;
};

#endif /* LayerMoments_hpp */
