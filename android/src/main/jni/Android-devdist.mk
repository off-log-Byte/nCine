LOCAL_PATH := $(call my-dir)

##################################

include $(CLEAR_VARS)
LOCAL_MODULE := openal
LOCAL_SRC_FILES := openal/$(TARGET_ARCH_ABI)/libopenal.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/openal/include
include $(PREBUILT_SHARED_LIBRARY)

##################################

include $(CLEAR_VARS)

LOCAL_MODULE := ncine
LOCAL_SRC_FILES := ncine/$(TARGET_ARCH_ABI)/libncine.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/ncine/include
include $(PREBUILT_SHARED_LIBRARY)

##################################

include $(CLEAR_VARS)

LOCAL_MODULE := game
LOCAL_CFLAGS := -Wall -ffast-math

LOCAL_SRC_FILES := main.cpp \
	tests/apptest_texformats.cpp

LOCAL_LDLIBS := -landroid
LOCAL_STATIC_LIBRARIES := android_native_app_glue
LOCAL_SHARED_LIBRARIES := ncine openal

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
