#include "ImagePickerBridge.h"
#include "SceneManager.h"

void ImagePickerBridge::SetCallBack(const std::function<void (const std::string&)> callback)
{
    finish_callback_ = callback;
    SceneManager::add_layer(BaseLayer::create());
}

void ImagePickerBridge::do_cancel()
{
    auto layer = SceneManager::get_layer_by_name(typeid(BaseLayer).name());
    layer->removeFromParent();
}

void ImagePickerBridge::change_head_pic(const std::string& pic_name)
{
    auto layer = SceneManager::get_layer_by_name(typeid(BaseLayer).name());
    layer->removeFromParent();
    if (finish_callback_)
    {
        finish_callback_(pic_name);
    }
    delete(this);
}