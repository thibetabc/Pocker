//
//  CommentCell.cpp
//  pokerclient
//
//  Created by admin on 15-9-10.
//
//

#include "CommentCell.h"
#include "utils.h"
#include "GameDataManager.h"
#include "ShaderSprite.h"
#include "FileManager.h"

bool CommentCell::init()
{
    if(!TableViewCell::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("NodeCommentCell.csb");
    this->addChild(root);
    
    image_bg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_1"));
    image_bg->setContentSize(Size(image_bg->getContentSize().width, 200)); // the next time may change.
    
    image_nickname = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_7"));
    tf_nickname_ = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "TextField_3"));
    tf_nickname_->setTouchEnabled(false);
    tf_comment = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "TextField_5"));
    tf_comment->setTouchEnabled(false);
    tf_time = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "TextField_8"));
    tf_time->setTouchEnabled(false);
    tf_time->setVisible(false);
    
    return true;
}

void CommentCell::update_data()
{

}

void CommentCell::update_property(msg::CommentInfo* comment_data)
{
    comment_data->momentid();
    comment_data->commentid();
    comment_data->type();
    comment_data->mutable_userinfo()->picname();
    comment_data->mutable_userinfo()->nickname();
    comment_data->content();
    comment_data->time();
    

    std::string headPicName = comment_data->mutable_userinfo()->picname();
    if(GDM->is_file_exist(headPicName)) {
        image_nickname->loadTexture(headPicName);
        image_nickname->setVisible(true);
    }
    else {
        image_nickname->loadTexture("msg_pic.png");
        FM.download_file(headPicName);
    }
    image_nickname->loadTexture(headPicName);
    tf_nickname_->setString(comment_data->mutable_userinfo()->nickname());
    
    if (comment_data->type() == 1 || comment_data->type() ==3){
        tf_comment->setString(comment_data->content());
    }else if (comment_data->type() ==2 ){
        tf_comment->setString(tools::local_string("sharemoment", "转发"));
    }
    
    
    // by ronaldz. calcuate the proper size
    auto size = Size(580, 80);
    tf_comment->setFontSize(25);
    tf_comment->setFontName("Thonburi");
    auto render_size = tf_comment->getAutoRenderSize();
    if (render_size.height == 0 ){
        render_size.height = 500;
    }
    
    auto orisize = size;
    if(render_size.width > size.width) {
        int line_count = ceilf(render_size.width / size.width);
        size.height = render_size.height * line_count;
    }
    else {
        size.height = render_size.height;
        
    }
    tf_comment->setContentSize(size);   // ronaldz. here to adjust to the content size.
    
    
    int height_change = size.height-orisize.height;
    
    image_bg->setContentSize(Size(image_bg->getContentSize().width, 200/*design height of bg*/ + height_change));
    image_nickname->setPositionY(image_bg->getContentSize().height-image_nickname->getContentSize().height/2);
    tf_nickname_->setPositionY(image_bg->getContentSize().height-tf_nickname_->getContentSize().height/2);
    tf_comment->setPositionY(tf_nickname_->getPositionY()-tf_nickname_->getContentSize().height/2);
    
     // by ronaldz. calcuate the proper size--- end
    
    
    cocos2d::log("comment:%s",comment_data->content().c_str());
    

}