//
//  ImagePickerAndroid.h
//  pokerclient
//
//  Created by MirrorMiko on 6/8/15.
//
//

#ifndef __pokerclient__ImagePickerAndroid__
#define __pokerclient__ImagePickerAndroid__

#include <stdio.h>
#include "ImagePickerBridge.h"

class ImagePickerAndroid : public ImagePickerBridge
{
public:
    ImagePickerAndroid(const ImagePickerType type);
};

#endif /* defined(__pokerclient__ImagePickerAndroid__) */
