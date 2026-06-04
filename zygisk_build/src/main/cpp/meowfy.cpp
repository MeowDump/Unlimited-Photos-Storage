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

static const char PIXEL_FP[] = "google/husky/husky:15/AP4A.250205.002/12025142:user/release-keys";
static const jlong PIXEL_TIME = 1738368000000LL;

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
    jfieldID fid_time = env->GetStaticFieldID(build, "TIME", "J");
    jfieldID fid_bootloader = env->GetStaticFieldID(build, "BOOTLOADER", "Ljava/lang/String;");
    jfieldID fid_radio = env->GetStaticFieldID(build, "RADIO", "Ljava/lang/String;");
    jfieldID fid_display = env->GetStaticFieldID(build, "DISPLAY", "Ljava/lang/String;");
    jfieldID fid_host = env->GetStaticFieldID(build, "HOST", "Ljava/lang/String;");
    jfieldID fid_user = env->GetStaticFieldID(build, "USER", "Ljava/lang/String;");

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
        jstring s = env->NewStringUTF("husky");
        env->SetStaticObjectField(build, fid_device, s);
        env->DeleteLocalRef(s);
    }
    if (fid_product) {
        jstring s = env->NewStringUTF("husky");
        env->SetStaticObjectField(build, fid_product, s);
        env->DeleteLocalRef(s);
    }
    if (fid_model) {
        jstring s = env->NewStringUTF("Pixel 8 Pro");
        env->SetStaticObjectField(build, fid_model, s);
        env->DeleteLocalRef(s);
    }
    if (fid_fp) {
        jstring s = env->NewStringUTF(PIXEL_FP);
        env->SetStaticObjectField(build, fid_fp, s);
        env->DeleteLocalRef(s);
    }
    if (fid_id) {
        jstring s = env->NewStringUTF("AP4A.250205.002");
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
        jstring s = env->NewStringUTF("husky");
        env->SetStaticObjectField(build, fid_board, s);
        env->DeleteLocalRef(s);
    }
    if (fid_hw) {
        jstring s = env->NewStringUTF("husky");
        env->SetStaticObjectField(build, fid_hw, s);
        env->DeleteLocalRef(s);
    }
    if (fid_time) {
        env->SetStaticLongField(build, fid_time, PIXEL_TIME);
    }
    if (fid_bootloader) {
        jstring s = env->NewStringUTF("ripcurrent-15.0-12025142");
        env->SetStaticObjectField(build, fid_bootloader, s);
        env->DeleteLocalRef(s);
    }
    if (fid_radio) {
        jstring s = env->NewStringUTF("g5300i-240605-240729-B-1234567");
        env->SetStaticObjectField(build, fid_radio, s);
        env->DeleteLocalRef(s);
    }
    if (fid_display) {
        jstring s = env->NewStringUTF("AP4A.250205.002");
        env->SetStaticObjectField(build, fid_display, s);
        env->DeleteLocalRef(s);
    }
    if (fid_host) {
        jstring s = env->NewStringUTF("abfarm-release-2025-01-15");
        env->SetStaticObjectField(build, fid_host, s);
        env->DeleteLocalRef(s);
    }
    if (fid_user) {
        jstring s = env->NewStringUTF("android-build");
        env->SetStaticObjectField(build, fid_user, s);
        env->DeleteLocalRef(s);
    }

    env->DeleteLocalRef(build);

    jclass version = env->FindClass("android/os/Build$VERSION");
    if (version) {
        jfieldID fid_sdk = env->GetStaticFieldID(version, "SDK_INT", "I");
        jfieldID fid_release = env->GetStaticFieldID(version, "RELEASE", "Ljava/lang/String;");
        jfieldID fid_security = env->GetStaticFieldID(version, "SECURITY_PATCH", "Ljava/lang/String;");
        jfieldID fid_codename = env->GetStaticFieldID(version, "CODENAME", "Ljava/lang/String;");
        jfieldID fid_preview = env->GetStaticFieldID(version, "PREVIEW_SDK_INT", "I");
        jfieldID fid_incremental = env->GetStaticFieldID(version, "INCREMENTAL", "Ljava/lang/String;");
        jfieldID fid_base_os = env->GetStaticFieldID(version, "BASE_OS", "Ljava/lang/String;");
        jfieldID fid_sdk_ext = env->GetStaticFieldID(version, "SDK_INT", "I");
        jfieldID fid_rel_or_codename = env->GetStaticFieldID(version, "RELEASE_OR_CODENAME", "Ljava/lang/String;");
        jfieldID fid_rel_or_preview = env->GetStaticFieldID(version, "RELEASE_OR_PREVIEW_DISPLAY", "Ljava/lang/String;");

        // Match actual device Android version to avoid SecurityException
        if (fid_sdk) env->SetStaticIntField(version, fid_sdk, 36);
        if (fid_release) {
            jstring s = env->NewStringUTF("15");
            env->SetStaticObjectField(version, fid_release, s);
            env->DeleteLocalRef(s);
        }
        if (fid_security) {
            jstring s = env->NewStringUTF("2025-05-05");
            env->SetStaticObjectField(version, fid_security, s);
            env->DeleteLocalRef(s);
        }
        if (fid_codename) {
            jstring s = env->NewStringUTF("REL");
            env->SetStaticObjectField(version, fid_codename, s);
            env->DeleteLocalRef(s);
        }
        if (fid_preview) env->SetStaticIntField(version, fid_preview, 0);
        if (fid_incremental) {
            jstring s = env->NewStringUTF("12025142");
            env->SetStaticObjectField(version, fid_incremental, s);
            env->DeleteLocalRef(s);
        }
        if (fid_base_os) {
            jstring s = env->NewStringUTF("");
            env->SetStaticObjectField(version, fid_base_os, s);
            env->DeleteLocalRef(s);
        }
        if (fid_rel_or_codename) {
            jstring s = env->NewStringUTF("15");
            env->SetStaticObjectField(version, fid_rel_or_codename, s);
            env->DeleteLocalRef(s);
        }
        if (fid_rel_or_preview) {
            jstring s = env->NewStringUTF("15");
            env->SetStaticObjectField(version, fid_rel_or_preview, s);
            env->DeleteLocalRef(s);
        }

        env->DeleteLocalRef(version);
    }

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

        LOGD("onLoad done");
    }

    void preAppSpecialize(AppSpecializeArgs *args) override {
        LOGD("====================");
        LOGD("preAppSpecialize: args=%p", (void*)args);

        if (!args || !env) {
            LOGE("args or env NULL");
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
        } else {
            api->setOption(zygisk::Option::DLCLOSE_MODULE_LIBRARY);
        }

        LOGD("preAppSpecialize done");
    }

    void postAppSpecialize(const AppSpecializeArgs *args) override {
        if (!env || !args) return;

        const char *name = nullptr;
        if (args->nice_name) {
            name = env->GetStringUTFChars(args->nice_name, nullptr);
        }
        if (name && strstr(name, "com.google.android.apps.photos")) {
            LOGI("*** POST-SPOOF for Photos ***");
            do_spoof(env);
        }
        if (name) env->ReleaseStringUTFChars(args->nice_name, name);
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
