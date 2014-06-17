LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

FILE_LIST += $(wildcard $(LOCAL_PATH)/Classes/Model/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/Classes/Object/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/Classes/Utils/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/Classes/Weapon/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/Classes/Xml/*.cpp)

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   Classes/AppDelegate.cpp \
                   Classes/HelloWorldScene.cpp
                   
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)


FILE_INCLUDES := $(shell find $(LOCAL_PATH)/Classes -type d)                    

LOCAL_C_INCLUDES := $(LOCAL_PATH)/Classes
LOCAL_C_INCLUDES += FILE_INCLUDES
#LOCAL_C_INCLUDES += $(LOCAL_PATH)/Classes/Utils
#LOCAL_C_INCLUDES += $(LOCAL_PATH)/Classes/Model
#LOCAL_C_INCLUDES += $(LOCAL_PATH)/Classes/Object
#LOCAL_C_INCLUDES += $(LOCAL_PATH)/Classes/Weapon


LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
