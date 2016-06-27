//
//  LayerPublishMoment.hpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/11.
//
//

#ifndef LayerPublishMoment_hpp
#define LayerPublishMoment_hpp

#include <stdio.h>
#include "SceneManager.h"

class LayerPublishMoment : public BaseLayer
{
protected:
    void register_events();
    void handle_publish_moment(cmd_data_pointer data);
public:
    CREATE_FUNC(LayerPublishMoment);
    virtual bool init();
};
#endif /* LayerPublishMoment_hpp */
