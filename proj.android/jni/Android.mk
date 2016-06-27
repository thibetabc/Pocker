LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := boost_system_static
LOCAL_SRC_FILES := ../../3rd/android/lib/libboost_system.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := protobuf_static
LOCAL_SRC_FILES := ../../3rd/protobuf/lib/libprotobuf_android.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external/libmobclickcpp)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos/network)
$(call import-add-path,$(LOCAL_PATH)/../../3rd/android/)
$(call import-add-path,$(LOCAL_PATH)/../../3rd/protobuf/include)




LOCAL_CPPFLAGS += -std=c++11
LOCAL_CFLAGS := -w

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp


LOCAL_SRC_FILES := 	hellocpp/main.cpp \
					../../network/NotificationObject.cpp \
					../../network/TcpManager.cpp \
					
					
LOCAL_SRC_FILES += $(shell find $(LOCAL_PATH)/../../Classes | grep -v ios | grep -e \.cpp$$ | sed 's^jni/^^g' )
LOCAL_SRC_FILES += $(shell find $(LOCAL_PATH)/../../Classes | grep -v ios | grep -e \.cc$$ | sed 's^jni/^^g' )

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(shell ls -FR $(LOCAL_C_INCLUDES) | grep -v ios | grep $(LOCAL_PATH)/$ )
LOCAL_C_INCLUDES += $(LOCAL_C_INCLUDES:$(LOCAL_PATH)/%:=$(LOCAL_PATH)/%)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../network

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../cocos2d/cocos/ui
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../cocos2d/cocos/network

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../3rd/android/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../3rd/protobuf/include
LOCAL_C_INCLUDES += $(SYSROOT)/usr/include

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_SHARED_LIBRARIES := mobclickcpp_shared
LOCAL_STATIC_LIBRARIES += boost_system_static
LOCAL_STATIC_LIBRARIES += protobuf_static

LOCAL_CFLAGS := -D__STDC_CONSTANT_MACROS -Wl,-Map=test.map -g
# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END


include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,libmobclickcpp)
# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
