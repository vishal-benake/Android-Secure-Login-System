#include "Includes/Includes.h"
#include "MemManager.h"
#include "APIHook.h"
#include "Includes/XorStr.h"

JNIEnv *env;

class JField {
public:
    jobject field;

    void setAccessible(bool b) {
        auto fieldClass = env->FindClass(XorStr("java/lang/reflect/Field"));
        auto setAccessibleMethod = env->GetMethodID(fieldClass, XorStr("setAccessible"), XorStr("(Z)V"));

        env->CallVoidMethod(field, setAccessibleMethod, b);
    }

    jobject get(jobject obj) {
        auto fieldClass = env->FindClass(XorStr("java/lang/reflect/Field"));
        auto getMethod = env->GetMethodID(fieldClass, XorStr("get"), XorStr("(Ljava/lang/Object;)Ljava/lang/Object;"));

        return env->CallObjectMethod(field, getMethod, obj);
    }

    void set(jobject obj, jobject val) {
        auto fieldClass = env->FindClass(XorStr("java/lang/reflect/Field"));
        auto setMethod = env->GetMethodID(fieldClass, XorStr("set"), XorStr("(Ljava/lang/Object;Ljava/lang/Object;)V"));

        env->CallVoidMethod(field, setMethod, obj, val);
    }
};

class JClass {
public:
    jobject clazz;

    static JClass *forName(const char *s) {
        auto str = env->NewStringUTF(s);

        auto classClass = env->FindClass(XorStr("java/lang/Class"));
        auto forNameMethod = env->GetStaticMethodID(classClass, XorStr("forName"), XorStr("(Ljava/lang/String;)Ljava/lang/Class;"));

        auto clazz = new JClass();
        clazz->clazz = env->CallStaticObjectMethod(classClass, forNameMethod, str);

        return clazz;
    }

    JField *getDeclaredField(const char *s) {
        auto str = env->NewStringUTF(s);

        auto classClass = env->FindClass(XorStr("java/lang/Class"));
        auto getDeclaredFieldMethod = env->GetMethodID(classClass, XorStr("getDeclaredField"), XorStr("(Ljava/lang/String;)Ljava/lang/reflect/Field;"));

        auto field = new JField();
        field->field = env->CallObjectMethod(clazz, getDeclaredFieldMethod, str);

        return field;
    }
};

string apk_path = "";
unsigned long apk_crc = 0;

void native_StartKill(JNIEnv *env, jclass clazz, jobject contextObj) {
    ::env = env;

    auto contextClass = env->FindClass(XorStr("android/content/Context"));
    auto getAssetsMethod = env->GetMethodID(contextClass, XorStr("getAssets"), XorStr("()Landroid/content/res/AssetManager;"));

    auto assetMgr = AAssetManager_fromJava(env, env->CallObjectMethod(contextObj, getAssetsMethod));
    auto apkFile = AAssetManager_open(assetMgr, XorStr("6h/yotsuba.dat"), AASSET_MODE_UNKNOWN);


    char apkPath[512];
    sprintf(apkPath, XorStr("/data/data/%s/files/cpu_cache0"), GAME_PACKAGE);

    char apkDir[512];
    sprintf(apkDir, XorStr("mkdir /data/data/%s/files"), GAME_PACKAGE);
    system(apkDir);

    apk_path = apkPath;

    FILE *f = fopen(apkPath, XorStr("wb"));
    if (f) {
        uint8_t buf[512];
        int n;
        while ((n = AAsset_read(apkFile, buf, sizeof(buf))) > 0) {
            fwrite(buf, sizeof(uint8_t), n, f);
        }
        fclose(f);
    }

    AAsset_close(apkFile);

    auto contextImplClass = JClass::forName(XorStr("android.app.ContextImpl"));
    auto mPackageInfoField = contextImplClass->getDeclaredField(XorStr("mPackageInfo"));
    mPackageInfoField->setAccessible(true);

    jobject mPackageInfo = mPackageInfoField->get(contextObj);

    auto loadedApkClass = JClass::forName(XorStr("android.app.LoadedApk"));
    auto mAppDirField = loadedApkClass->getDeclaredField(XorStr("mAppDir"));
    mAppDirField->setAccessible(true);

    auto mResDirField = loadedApkClass->getDeclaredField(XorStr("mResDir"));
    mResDirField->setAccessible(true);

    auto mApplicationInfoField = loadedApkClass->getDeclaredField(XorStr("mApplicationInfo"));
    mApplicationInfoField->setAccessible(true);
    jobject mApplicationInfo = mApplicationInfoField->get(mPackageInfo);

    auto applicationInfoClass = JClass::forName(XorStr("android.content.pm.ApplicationInfo"));

    auto sourceDirField = applicationInfoClass->getDeclaredField(XorStr("sourceDir"));
    sourceDirField->setAccessible(true);

    auto publicSourceDirField = applicationInfoClass->getDeclaredField(XorStr("publicSourceDir"));
    publicSourceDirField->setAccessible(true);

    string apkData;
    f = fopen(env->GetStringUTFChars((jstring)publicSourceDirField->get(mApplicationInfo), 0), "rb");
    if (f) {
        uint8_t buf[512];
        int n;
        while ((n = fread(buf, sizeof(uint8_t), sizeof buf, f)) > 0) {
            apkData.append(buf, buf + n);
        }
        fclose(f);

        apk_crc = crc32(0L, Z_NULL, 0);
        apk_crc = crc32(apk_crc, (const unsigned char*)apkData.data(), apkData.length());
    }
    auto newApk = env->NewStringUTF(apkPath);

    mAppDirField->set(mPackageInfo, newApk);
    mResDirField->set(mPackageInfo, newApk);
    sourceDirField->set(mApplicationInfo, newApk);
    publicSourceDirField->set(mApplicationInfo, newApk);

    APIHook(apkPath);
}

jstring native_GetAPK(JNIEnv *env, jclass clazz) {
    return env->NewStringUTF(apk_path.c_str());
}