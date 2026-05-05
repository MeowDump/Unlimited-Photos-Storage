#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include <android/log.h>
#include <sys/system_properties.h>

#include "module.h"
#include "zygisk.hpp"

using zygisk::Api;
using zygisk::AppSpecializeArgs;
using zygisk::ServerSpecializeArgs;

namespace meowfytag {
    bool g_debug_enabled = false;
}

using namespace meowfytag;

static const char PIXEL_XL_FP[] = "google/marlin/marlin:10/QP1A.191005.007.A3/5972272:user/release-keys";

// Direct spoof
static void do_spoof(JNIEnv *env) {
    LOGI(">>> do_spoof start");
    
    if (!env) {
        LOGE("env is NULL!");
        return;
    }

    jclass build = env->FindClass("android/os/Build");
    if (!build) {
        LOGE("Build class not found");
        return;
    }

    // Get all field IDs first
    jfieldID fid_brand = env->GetStaticFieldID(build, "BRAND", "Ljava/lang/String;");
    jfieldID fid_manuf = env->GetStaticFieldID(build, "MANUFACTURER", "Ljava/lang/String;");
    jfieldID fid_device = env->GetStaticFieldID(build, "DEVICE", "Ljava/lang/String;");
    jfieldID fid_product = env->GetStaticFieldID(build, "PRODUCT", "Ljava/lang/String;");
    jfieldID fid_model = env->GetStaticFieldID(build, "MODEL", "Ljava/lang/String;");
    jfieldID fid_fp = env->GetStaticFieldID(build, "FINGERPRINT", "Ljava/lang/String;");
    jfieldID fid_id = env->GetStaticFieldID(build, "ID", "Ljava/lang/String;");
    jfieldID fid_tags = env->GetStaticFieldID(build, "TAGS", "Ljava/lang/String;");
    jfieldID fid_type = env->GetStaticFieldID(build, "TYPE", "Ljava/lang/String;");
    jfieldID fid_board = env->GetStaticFieldID(build, "BOARD", "Ljava/lang/String;");
    jfieldID fid_hw = env->GetStaticFieldID(build, "HARDWARE", "Ljava/lang/String;");

    // Set fields
    if (fid_brand) {
        jstring s = env->NewStringUTF("google");
        env->SetStaticObjectField(build, fid_brand, s);
        env->DeleteLocalRef(s);
    }
    if (fid_manuf) {
        jstring s = env->NewStringUTF("Google");
        env->SetStaticObjectField(build, fid_manuf, s);
        env->DeleteLocalRef(s);
    }
    if (fid_device) {
        jstring s = env->NewStringUTF("marlin");
        env->SetStaticObjectField(build, fid_device, s);
        env->DeleteLocalRef(s);
    }
    if (fid_product) {
        jstring s = env->NewStringUTF("marlin");
        env->SetStaticObjectField(build, fid_product, s);
        env->DeleteLocalRef(s);
    }
    if (fid_model) {
        jstring s = env->NewStringUTF("Pixel XL");
        env->SetStaticObjectField(build, fid_model, s);
        env->DeleteLocalRef(s);
    }
    if (fid_fp) {
        jstring s = env->NewStringUTF(PIXEL_XL_FP);
        env->SetStaticObjectField(build, fid_fp, s);
        env->DeleteLocalRef(s);
    }
    if (fid_id) {
        jstring s = env->NewStringUTF("QP1A.191005.007.A3");
        env->SetStaticObjectField(build, fid_id, s);
        env->DeleteLocalRef(s);
    }
    if (fid_tags) {
        jstring s = env->NewStringUTF("release-keys");
        env->SetStaticObjectField(build, fid_tags, s);
        env->DeleteLocalRef(s);
    }
    if (fid_type) {
        jstring s = env->NewStringUTF("user");
        env->SetStaticObjectField(build, fid_type, s);
        env->DeleteLocalRef(s);
    }
    if (fid_board) {
        jstring s = env->NewStringUTF("marlin");
        env->SetStaticObjectField(build, fid_board, s);
        env->DeleteLocalRef(s);
    }
    if (fid_hw) {
        jstring s = env->NewStringUTF("marlin");
        env->SetStaticObjectField(build, fid_hw, s);
        env->DeleteLocalRef(s);
    }

    env->DeleteLocalRef(build);

    LOGI(">>> do_spoof done");
}

class MeowfyModule : public zygisk::ModuleBase
{
public:
    void onLoad(Api *api, JNIEnv *env) override {
        init_debug();
        
        if (g_debug_enabled) {
            LOGI("====================");
            LOGI("onLoad: api=%p env=%p", (void*)api, (void*)env);
            LOGI("debug=ON");
        }
        
        this->api = api;
        this->env = env;
        
        if (api) {
            api->setOption(zygisk::Option::DLCLOSE_MODULE_LIBRARY);
            LOGD("DLCLOSE set");
        }
        
        LOGD("onLoad done");
    }

    void preAppSpecialize(AppSpecializeArgs *args) override {
        LOGD("====================");
        LOGD("preAppSpecialize: args=%p", (void*)args);
        
        if (!args) {
            LOGE("args NULL");
            return;
        }
        
        if (!env) {
            LOGE("env NULL");
            return;
        }

        const char *name = nullptr;
        if (args->nice_name) {
            name = env->GetStringUTFChars(args->nice_name, nullptr);
        }
        
        if (!name) {
            LOGW("no process name");
            return;
        }

        LOGD("process=%s", name);
        
        bool is_photos = (strstr(name, "com.google.android.apps.photos") != nullptr);
        LOGD("is_photos=%d", is_photos);
        
        env->ReleaseStringUTFChars(args->nice_name, name);

        if (is_photos) {
            LOGI("*** PHOTOS DETECTED - SPOOFING NOW ***");
            do_spoof(env);
        }
        
        LOGD("preAppSpecialize done");
    }

    void postAppSpecialize(const AppSpecializeArgs *) override {
        LOGD("postAppSpecialize");
    }

    void preServerSpecialize(ServerSpecializeArgs *) override {
        LOGD("preServerSpecialize");
    }

    void postServerSpecialize(const ServerSpecializeArgs *) override {
        LOGD("postServerSpecialize");
    }

private:
    Api *api = nullptr;
    JNIEnv *env = nullptr;
};

REGISTER_ZYGISK_MODULE(MeowfyModule)
