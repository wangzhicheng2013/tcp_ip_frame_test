LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := server
LOCAL_SRC_FILES := server.cpp
LOCAL_LDLIBS := -pthread
LOCAL_CFLAGS := -Wall -Werror -Wextra -Wno-unused-parameter -Wno-return-type -Wno-unused-variable -Wno-unused-function
include $(BUILD_EXECUTABLE)