//
//  ConnectingView.h
//  pokerclient
//  网络转圈连接中
//
//  Created by HuangFeng on 16/5/18.
//
//

#ifndef ConnectingView_h
#define ConnectingView_h
#include "SceneManager.h"

class ConnectingView : public Layer
{
public:
    CREATE_FUNC(ConnectingView);
private:
    ConnectingView *create(Vec2 position);
    ConnectingView();
    ~ConnectingView();
    virtual bool init();
    Vec2 connectingPosition;
};
#endif /* ConnectingView_h */
