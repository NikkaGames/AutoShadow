LOCAL_PATH := $(call my-dir)
MAIN_LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libdobby
LOCAL_CFLAGS := -w -s -Wno-error=format-security -fvisibility=default -fpermissive -fexceptions
LOCAL_CPPFLAGS := -w -s -Wno-error=format-security -fvisibility=default -Werror -std=c++17
LOCAL_CPPFLAGS += -Wno-error=c++11-narrowing -fpermissive -Wall -fexceptions
LOCAL_LDFLAGS += -Wl,--gc-sections,--strip-all,-llog
LOCAL_ARM_MODE := arm
LOCAL_SRC_FILES := libraries/$(TARGET_ARCH_ABI)/libdobby.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := main
LOCAL_CFLAGS := -Wno-error=format-security -fvisibility=default -ffunction-sections -fdata-sections -w
LOCAL_CFLAGS += -fno-rtti -fno-exceptions -fpermissive
LOCAL_CPPFLAGS := -Wno-error=format-security -fvisibility=default -ffunction-sections -fdata-sections -w -Werror -s -std=c++17
LOCAL_CPPFLAGS += -Wno-error=c++11-narrowing -fms-extensions -fno-rtti -fno-exceptions -fpermissive
LOCAL_LDFLAGS += -Wl,--gc-sections,--strip-all, -llog -landroid -lEGL -lGLESv2 -lGLESv3 -lz
LOCAL_ARM_MODE := arm

LOCAL_STATIC_LIBRARIES := libdobby

LOCAL_C_INCLUDES += $(MAIN_LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/ImGui

LOCAL_SRC_FILES := native-lib.cpp \
     Includes/il2cpp_dump.cpp \
     xdl/xdl.c \
     xdl/xdl_iterate.c \
     xdl/xdl_linker.c \
     xdl/xdl_lzma.c \
     xdl/xdl_util.c \
     ImGui/imgui.cpp \
     ImGui/imgui_draw.cpp \
	 ImGui/imgui_demo.cpp \
	 ImGui/imgui_widgets.cpp \
	 ImGui/imgui_tables.cpp \
	 ImGui/backends/imgui_impl_opengl3.cpp \
	 ImGui/backends/imgui_impl_android.cpp \
     KittyMemory/KittyMemory.cpp \
     KittyMemory/MemoryPatch.cpp \
     KittyMemory/MemoryBackup.cpp \
     KittyMemory/KittyUtils.cpp \

LOCAL_LDLIBS := -llog -landroid
include $(BUILD_SHARED_LIBRARY)
