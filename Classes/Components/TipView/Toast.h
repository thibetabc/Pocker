//
//  Toast.h
//  pokerclient
//
//  Created by HuangFeng on 16/5/13.
//
//

#ifndef Toast_h
#define Toast_h

#include <stdio.h>
#include "SceneManager.h"

class Toast
{
public:
    inline long long millisecondNow()
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec * 1000 + tv.tv_usec / 1000;
    }
    static void ToastShow(std::string txt, float time = 0.5f, float rotation = 0.0f);
};
#endif /* Toast_h */
