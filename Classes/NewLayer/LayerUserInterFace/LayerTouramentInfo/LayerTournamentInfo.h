//
//  LayerTournamentInfo.hpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/11.
//
//

#ifndef LayerTournamentInfo_hpp
#define LayerTournamentInfo_hpp

#include <stdio.h>
#include "SceneManager.h"
class LayerTournamentInfo : public BaseLayer
{
public:
    CREATE_FUNC(LayerTournamentInfo);
    virtual bool init();
protected:
    void register_events();
    void handle_cancel_match(cmd_data_pointer data);
    void handle_join(cmd_data_pointer data);
private:
};

#endif /* LayerTournamentInfo_hpp */
