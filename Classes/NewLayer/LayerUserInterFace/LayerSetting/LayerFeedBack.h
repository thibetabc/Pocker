//
//  LayerFeedBack.hpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/11.
//
//

#ifndef LayerFeedBack_hpp
#define LayerFeedBack_hpp

#include <stdio.h>
#include "SceneManager.h"
class LayerFeedBack : public BaseLayer
{
protected:
    void register_events();
    void handle_feed_back(cmd_data_pointer data);
public:
    CREATE_FUNC(LayerFeedBack);
    virtual bool init();
};
#endif /* LayerFeedBack_hpp */
