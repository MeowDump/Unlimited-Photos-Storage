#pragma once

#include <cerrno>
#include <cstring>
#include <android/log.h>
#include <sys/stat.h>

namespace meowfytag {

#ifndef TAG
#define TAG    "Meowfy"
#endif

// Check debug flag once at module load
extern bool g_debug_enabled;

// Initialize debug state
static inline void init_debug() {
    struct stat st;
    g_debug_enabled = (stat("/data/adb/modules/UnlimitedPhotosStorage/debug", &st) == 0);
}

#define LOGD(...) do { if (meowfytag::g_debug_enabled) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__); } while(0)
#define LOGI(...) do { if (meowfytag::g_debug_enabled) __android_log_print(ANDROID_LOG_INFO,  TAG, __VA_ARGS__); } while(0)
#define LOGW(...) do { if (meowfytag::g_debug_enabled) __android_log_print(ANDROID_LOG_WARN,  TAG, __VA_ARGS__); } while(0)
#define LOGE(...) do { if (meowfytag::g_debug_enabled) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__); } while(0)
#define LOGERRNO(...) do { if (meowfytag::g_debug_enabled) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__ ": %d (%s)", errno, strerror(errno)); } while(0)

}
