#pragma once

#include <cerrno>
#include <cstring>
#include <android/log.h>
#include <sys/stat.h>

namespace meowfytag {

#ifndef TAG
#define TAG    "Meowfy"
#endif

extern bool g_debug_enabled;

static inline void init_debug() {
    struct stat st;
    g_debug_enabled = (stat("/data/adb/modules/UnlimitedPhotosStorage/debug", &st) == 0);
}

// Only use debug flag for verbose logs
#define LOGD(...) do { if (meowfytag::g_debug_enabled) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__); } while(0)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

} // namespace meowfytag
