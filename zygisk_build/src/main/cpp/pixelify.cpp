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

// Target app
static const std::vector<std::string> PHOTOS_APPS = {
    "com.google.android.apps.photos"
};

// OG Pixel XL fingerprint for unlimited storage legacy check
static const char PIXEL_XL_FP[] = "google/marlin/marlin:10/QP1A.191005.007.A3/5972272:user/release-keys";
static const char PIXEL_XL_DEVICE[] = "marlin";
static const char PIXEL_XL_MODEL[] = "Pixel XL";

// Modern Pixel 8 Pro props for compatibility
static const char PIXEL_DEVICE[] = "husky";
static const char PIXEL_MODEL[] = "Pixel 8 Pro";
static const char PIXEL_PRODUCT[] = "husky";
static const char PIXEL_FP[] = "google/husky/husky:14/AP2A.240805.005/12025142:user/release-keys";

static char package_name[256] = {0};
static bool is_photos_process = false;

class MeowfyModule : public zygisk::ModuleBase
{
public:
    void onLoad(Api *api, JNIEnv *env) override {
        this->api = api;
        this->env = env;
        init_debug();
        LOGD("Module loaded, debug=%s", g_debug_enabled ? "enabled" : "disabled");
    }

    void preAppSpecialize(AppSpecializeArgs *args) override {
        if (!args || !args->nice_name) return;

        const char *process = env->GetStringUTFChars(args->nice_name, nullptr);
        if (!process) return;

        strncpy(package_name, process, sizeof(package_name) - 1);
        package_name[sizeof(package_name) - 1] = '\0';

        is_photos_process = isTargetApp(process);

        if (is_photos_process) {
            LOGD("Target app detected: %s", process);
        }

        env->ReleaseStringUTFChars(args->nice_name, process);
    }

    void postAppSpecialize(const AppSpecializeArgs *) override {
        if (!is_photos_process) return;

        LOGI("Spoofing Build props for: %s", package_name);

        // Inject Build fields
        // use Pixel XL for unlimited storage trigger
        injectBuild(PIXEL_XL_MODEL, PIXEL_XL_DEVICE, PIXEL_XL_FP);

        // Also inject modern Pixel props for compatibility
        injectProperties();

        // Spoof version for legacy unlimited storage
        injectVersion(25); // Android 7.1.2 SDK
    }

private:
    Api *api;
    JNIEnv *env;

    bool isTargetApp(const char *process) {
        std::string pkg = process;
        for (const auto &app : PHOTOS_APPS) {
            if (pkg.find(app) != std::string::npos) {
                return true;
            }
        }
        return false;
    }

    void injectBuild(const char *model1, const char *device1, const char *finger1) {
        if (!env) {
            LOGW("env is null for %s", package_name);
            return;
        }

        jclass build_class = env->FindClass("android/os/Build");
        if (!build_class) {
            LOGW("Build class not found for %s", package_name);
            return;
        }

        LOGI("Injecting Build for %s: MODEL=%s DEVICE=%s", package_name, model1, device1);

        // Set BRAND
        jstring brand = env->NewStringUTF("google");
        jfieldID brand_id = env->GetStaticFieldID(build_class, "BRAND", "Ljava/lang/String;");
        if (brand_id) env->SetStaticObjectField(build_class, brand_id, brand);

        // Set MANUFACTURER
        jstring manufacturer = env->NewStringUTF("Google");
        jfieldID manufacturer_id = env->GetStaticFieldID(build_class, "MANUFACTURER", "Ljava/lang/String;");
        if (manufacturer_id) env->SetStaticObjectField(build_class, manufacturer_id, manufacturer);

        // Set DEVICE
        jstring device = env->NewStringUTF(device1);
        jfieldID device_id = env->GetStaticFieldID(build_class, "DEVICE", "Ljava/lang/String;");
        if (device_id) env->SetStaticObjectField(build_class, device_id, device);

        // Set PRODUCT
        jstring product = env->NewStringUTF(device1);
        jfieldID product_id = env->GetStaticFieldID(build_class, "PRODUCT", "Ljava/lang/String;");
        if (product_id) env->SetStaticObjectField(build_class, product_id, product);

        // Set MODEL
        jstring model = env->NewStringUTF(model1);
        jfieldID model_id = env->GetStaticFieldID(build_class, "MODEL", "Ljava/lang/String;");
        if (model_id) env->SetStaticObjectField(build_class, model_id, model);

        // Set FINGERPRINT
        if (finger1 && strlen(finger1) > 0) {
            jstring finger = env->NewStringUTF(finger1);
            jfieldID finger_id = env->GetStaticFieldID(build_class, "FINGERPRINT", "Ljava/lang/String;");
            if (finger_id) env->SetStaticObjectField(build_class, finger_id, finger);
            env->DeleteLocalRef(finger);
        }

        // Set BOARD
        jstring board = env->NewStringUTF("husky");
        jfieldID board_id = env->GetStaticFieldID(build_class, "BOARD", "Ljava/lang/String;");
        if (board_id) env->SetStaticObjectField(build_class, board_id, board);

        // Set HARDWARE
        jstring hardware = env->NewStringUTF("husky");
        jfieldID hardware_id = env->GetStaticFieldID(build_class, "HARDWARE", "Ljava/lang/String;");
        if (hardware_id) env->SetStaticObjectField(build_class, hardware_id, hardware);

        if (env->ExceptionCheck()) {
            LOGW("Exception during Build injection, clearing");
            env->ExceptionClear();
        }

        env->DeleteLocalRef(brand);
        env->DeleteLocalRef(manufacturer);
        env->DeleteLocalRef(device);
        env->DeleteLocalRef(product);
        env->DeleteLocalRef(model);
        env->DeleteLocalRef(board);
        env->DeleteLocalRef(hardware);
    }

    void injectVersion(int sdk_int) {
        if (!env) return;

        jclass version_class = env->FindClass("android/os/Build\$VERSION");
        if (!version_class) {
            LOGW("Build.VERSION class not found");
            return;
        }

        LOGD("Injecting SDK_INT=%d", sdk_int);

        // Set SDK_INT
        jfieldID sdk_id = env->GetStaticFieldID(version_class, "SDK_INT", "I");
        if (sdk_id) env->SetStaticIntField(version_class, sdk_id, sdk_int);

        // Set DEVICE_INITIAL_SDK_INT for newer Android
        jfieldID init_sdk_id = env->GetStaticFieldID(version_class, "DEVICE_INITIAL_SDK_INT", "I");
        if (init_sdk_id) env->SetStaticIntField(version_class, init_sdk_id, sdk_int);

        // Set PREVIEW_SDK_INT to 0
        jfieldID preview_id = env->GetStaticFieldID(version_class, "PREVIEW_SDK_INT", "I");
        if (preview_id) env->SetStaticIntField(version_class, preview_id, 0);

        if (env->ExceptionCheck()) {
            env->ExceptionClear();
        }
    }

    void injectProperties() {
        // Use __system_property_set to spoof system properties
        const char* props[][2] = {
            {"ro.product.brand", "google"},
            {"ro.product.manufacturer", "Google"},
            {"ro.product.device", PIXEL_DEVICE},
            {"ro.product.product.device", PIXEL_DEVICE},
            {"ro.product.model", PIXEL_MODEL},
            {"ro.product.product.model", PIXEL_MODEL},
            {"ro.product.name", PIXEL_PRODUCT},
            {"ro.product.product.name", PIXEL_PRODUCT},
            {"ro.build.fingerprint", PIXEL_FP},
            {"ro.build.product", PIXEL_DEVICE},
            {"ro.product.board", "husky"},
            {nullptr, nullptr}
        };

        for (int i = 0; props[i][0] != nullptr; i++) {
            int ret = __system_property_set(props[i][0], props[i][1]);
            LOGD("Set prop %s=%s (ret=%d)", props[i][0], props[i][1], ret);
        }
    }
};

REGISTER_ZYGISK_MODULE(MeowfyModule)
