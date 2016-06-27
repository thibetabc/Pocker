//
//  SceneManager.h
//  pokerclient
//
//  Created by duanmt on 15/4/30.
//
//

#ifndef __pokerclient__SceneManager__
#define __pokerclient__SceneManager__

#include <stdio.h>
#include "cocos2d.h"
#include "net_manager.h"
#include "net_data_recv.h"
#include "global_header.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;

#define NAME_LAYERGAMEROOM "GameRoom"
#define TipViewTag 255
#define LayerShareTag 256
#define WaitingViewTag 257

typedef enum {
    DIRECTION_NONE,
    BOTTOM_TO_TOP,
    TOP_TO_BOTTOM,
    LEFT_TO_RIGHT,
    RIGHT_TO_LEFT,
    DEFAULT_DIR = DIRECTION_NONE,
} LAYER_SHOW_DIRECTION;

typedef LAYER_SHOW_DIRECTION LAYER_DISMISS_DIRECTION;

class BaseLayer: public Layer {
public:
    CREATE_FUNC(BaseLayer);
    BaseLayer();
    ~BaseLayer();
    friend class LayerPool;
    friend class SceneManager;
    virtual const std::string GetName();
    virtual bool init();
    virtual void onEnterTransitionDidFinish();
    
    virtual void set_show_dir(const LAYER_SHOW_DIRECTION& dir);
    virtual void set_dismiss_callback(const std::function<void()>& callback) { dismiss_callback_ = callback;}
    void ShowMessage(const std::string& message, bool autoclose = false);
    void ShowError(const std::string& message);
    void ClearError();
    virtual void RefreshLayer();
    virtual void LayerDisconnect();
    void ShowTipMessage(bool show,std::string message="");
    void showWaiting();
    void removeWaiting();
protected:
    LAYER_SHOW_DIRECTION show_dir_;
    events::event_dispatch dispatch_;
    std::function<void()> dismiss_callback_;
    
    cocos2d::ui::Layout* layout_;
    std::map<uint32_t, std::map<uint64_t, bool>> requestlist;
    virtual int GetServerID() {return net_manager::SERVER_RESOURCE;}
    virtual void onExit();
    void AddGlobalLayer(BaseLayer* layer);
    bool check_request(std::shared_ptr<net_data_recv_package> package);
    void send_data(const MyMessage& processor, int request_code,bool isNeedUserid=true);
    virtual bool IsIndepedent() {return true;}
    virtual cocos2d::Size GetLayerSize();
    virtual cocos2d::Vec2 GetAnchorPoint();
    virtual cocos2d::Vec2 GetErrorPoint();
    virtual void register_events() {}
    virtual void handle_android_back();
    virtual void handle_connect_break(const std::shared_ptr<server_status_data>& data);
    virtual void handle_connect_success(const std::shared_ptr<server_status_data>& data);
};

class SceneManager {
public:
    static bool get_landscape() { return landscape_;}
    static void refresh(cocos2d::Node *n);
    static void addLoginLayer(bool enable = true,std::string s = "");
    static void addMainLayer();
    static void addWelcomeLayer();
    static BaseLayer* addTipLayer();
    static void addWaitingLayer();
    static void clear();
    static BaseLayer* GetLoginLayer();
    static BaseLayer* GetMainLayer();
    static BaseLayer* GetRoomLayer();
    static BaseLayer* GetTipLayer();
    static BaseLayer* GetWaitingLayer();
    static BaseLayer* get_layer_by_name(std::string name);
    static void add_layer(BaseLayer* node, int zorder = 0);
    static void HideOtherLayer();
    static void ShowLastLayer();
    static bool is_layer_on_top(BaseLayer* node);
private:
    static void createAndroidBack(Scene *scene);
private:
    static bool landscape_;
};

class LayerPool: public SINGLETON<LayerPool>{
public:
    void handle_android_back(cmd_data_pointer data);
    void pop_layer(BaseLayer* Layer);
    events::event_dispatch dispatch_;
    bool Inited = false;
    void push_layer(BaseLayer* node);
    void clear() {
        layers_.clear();
    }
private:
    void connect_success(cmd_data_pointer data);
    void connect_failed(cmd_data_pointer data);
    void connect_break(cmd_data_pointer data);
    void recv_error(cmd_data_pointer data);
private:
    BaseLayer* GetTopLayer();
    static std::list<BaseLayer*> layers_;
};

#endif /* defined(__pokerclient__SceneManager__) */
