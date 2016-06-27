//
//  EditBoxInterface.h
//  pokerclient
//
//  Created by MirrorMiko on 6/13/15.
//
//

#ifndef __pokerclient__EditBoxInterface__
#define __pokerclient__EditBoxInterface__

#include <iostream>

class EditBoxInterface{
public:
    EditBoxInterface();
    virtual ~EditBoxInterface();
    virtual bool is_focus();
    virtual bool is_password();
    virtual bool is_hidden();
    virtual std::string font_name();
    
    virtual void set_text(const std::string& text);
    virtual void set_text_color(int red, int blue, int green);
    virtual void set_background_color(int red, int blue, int green);
    virtual void set_keyborad_type(int type);
    virtual void set_keyborad_input_flag(int flag);
    virtual void set_input_flag(int flag);
    virtual void set_return_type(int type);
    virtual void set_editable(bool able);
    virtual void set_visible(bool visible);
    virtual void set_placeholder(const std::string& text);
    virtual void set_size(float w, float h);
    virtual void set_position(float x, float y);
    
    
    virtual bool is_editable();
    
    virtual void set_mult_line(bool);

    virtual std::string text();
    
    virtual void open_keyborad();
    virtual void close_keyborad();

    virtual void doAnimationWhenKeyboardMoveWithDuration(float duration, float distence);
    
    
    const int get_control_id() { return control_id;}
protected:
    int control_id;
};

#endif /* defined(__pokerclient__EditBoxInterface__) */
