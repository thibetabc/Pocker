#include "ActionSheet.h"
#include "utils.h"
#include "ImagePickerBridge.h"
#include "cmd_def.h"

bool ActionSheet::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto layerColor = LayerColor::create(Color4B(100, 100, 100, 80));
    this->addChild(layerColor, -1);
    
    auto layout = Layout::create();
    layout->setContentSize(this->getContentSize());
    layout->setTouchEnabled(true);
    layout->addClickEventListener([this](Ref*) {
        click_image_cancel(nullptr);
    });
    this->addChild(layout);
    
    origin_ = Point(0, -189);
    
    root_ = CSLoader::createNode("LayerActionSheet.csb");
    root_->setPosition(origin_);
    this->addChild(root_);
    
    auto image_camera = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_camera"));
    auto image_album = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_album"));
    auto image_cancel = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_cancel"));
    
    image_camera->addClickEventListener(std::bind(&ActionSheet::click_image_camera, this, std::placeholders::_1));
    image_album->addClickEventListener(std::bind(&ActionSheet::click_image_album, this, std::placeholders::_1));
    image_cancel->addClickEventListener(std::bind(&ActionSheet::click_image_cancel, this, std::placeholders::_1));
    
    auto text_camera = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_camera"));
    auto text_photo = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_photo"));
    auto text_cancel = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_cancel"));
    
    text_camera->setString(tools::local_string("take_photo", "拍照"));
    text_photo->setString(tools::local_string("select_from_album", "从手机相册选择"));
    text_cancel->setString(tools::local_string("cancel", "取消"));
    
    auto& size = Director::getInstance()->getWinSize();
    
    image_camera->setContentSize(Size(size.width, image_camera->getContentSize().height));
    image_album->setContentSize(Size(size.width, image_album->getContentSize().height));
    image_cancel->setContentSize(Size(size.width, image_cancel->getContentSize().height));
    
    image_camera->setPosition(Point(size.width/2, image_camera->getPosition().y));
    image_album->setPosition(Point(size.width/2, image_album->getPosition().y));
    image_cancel->setPosition(Point(size.width/2, image_cancel->getPosition().y));
    
    text_camera->setPosition(Point(size.width/2, text_camera->getPosition().y));
    text_photo->setPosition(Point(size.width/2, text_photo->getPosition().y));
    text_cancel->setPosition(Point(size.width/2, text_cancel->getPosition().y));
    return true;
}

void ActionSheet::click_image_camera(Ref* sender)
{
    ImagePickerType type = CameraWithEdit;
    if (m_bIsChat)
    {
        type = CameraWithoutEdit;
    }
    picker_ = __createImagePicker(type);
    
    picker_->finish_callback_ = after_choose_pic_callback_;
    
    if(dis_miss_callback_) {
        dis_miss_callback_();
    }
}

void ActionSheet::click_image_album(Ref* sender)
{
    ImagePickerType type = PhotoLibraryWithEdit;
    if (m_bIsChat)
    {
        type = PhotoLibraryWithoutEdit;
    }
    picker_ = __createImagePicker(type);
    
    picker_->finish_callback_ = std::bind(&ActionSheet::after_choose_pic, this, std::placeholders::_1);
    
    if(dis_miss_callback_) {
        dis_miss_callback_();
    }
}

void ActionSheet::click_image_cancel(Ref* sender)
{
    if(dis_miss_callback_) {
        dis_miss_callback_();
    }
    removeFromParent();
}

void ActionSheet::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    root_->runAction(MoveTo::create(0.2f, Point::ZERO));
}

void ActionSheet::after_choose_pic(const std::string& filename)
{
    after_choose_pic_callback_(filename);
    removeFromParent();
}

void ActionSheet::close()
{
    if(dis_miss_callback_) {
        dis_miss_callback_();
    }
    MoveTo* move = MoveTo::create(0.2f, origin_);
    Sequence* seq = Sequence::create(move, CallFunc::create([this]() {
        this->removeFromParent();
    }), NULL);
    
    root_->runAction(seq);
}