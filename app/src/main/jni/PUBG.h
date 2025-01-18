#include <jni.h>
#include <string>
#include <algorithm>
#include "mtools/StrEnc.h"
#include "mtools/XorStr.h"
#include "mtools/Tools.h"
#include "json.hpp"
#include "zip.h"
#include "LicenseTools.h"
bool isCompleted = true;
std::map<std::string, u_long> hackConfig;
using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;
std::map<int, bool> itemConfig;
json itemData;
bool bValid = false;
std::map<uintptr_t, int> GrenadesCD;
int screenWidth = 0, screenHeight = 0;
uintptr_t g_UE4 = 0;
#define SLEEP_TIME 1000LL / 60LL
std::string g_Token, g_Auth;



void *Main_Thread(void *) {
    while (!g_UE4) {
    }
}





static size_t HeaderCallback(char *buffer, size_t size, size_t nitems, void *userdata) {
    size_t length = size * nitems, index = 0;
    while (index < length) {
        unsigned char *temp = (unsigned char *) buffer + index;
        if ((temp[0] == '\r') || (temp[0] == '\n'))
            break;
        index++;
    }

    string str((unsigned char *) buffer, (unsigned char *) buffer + index);
    map<string, string> *pmHeader = (map<string, string> *) userdata;
    size_t pos = str.find(": ");
    if (pos != string::npos)
        pmHeader->insert(pair<string, string>(str.substr(0, pos), str.substr(pos + 2)));

    return nitems;
}

// ==========================================================================//
void native_onSendConfig(JNIEnv *env, jobject thiz, jstring s, jlong v) {
    const char *config = env->GetStringUTFChars(s, 0);
    hackConfig[config] = v;
}

void native_onSendConfig2(JNIEnv *env, jobject thiz, jstring s, jstring v) {
    const char *config = env->GetStringUTFChars(s, 0);
    const char *value = env->GetStringUTFChars(v, 0);
    if (!strcmp(config, "CMD_PARSE_ITEMS")) {
        itemData = json::parse(value);
    } else if (!strcmp(config, "SETTINGS_ITEMS")) {
        u_long itemId = strtoul(value, 0, 0);
        itemConfig[itemId] = !itemConfig[itemId];
    } else {
        hackConfig[config] = (u_long) strtoul(value, 0, 0);
    }
}

void native_Init(JNIEnv *env, jclass clazz, jobject mContext) {
    char szToast[0] = {};
    for (unsigned int oywdm = 0, SmnWs = 0; oywdm < 0; oywdm++) {
        SmnWs = szToast[oywdm];
        SmnWs++;
        SmnWs -= oywdm;
        SmnWs++;
        SmnWs -= oywdm;
        SmnWs ^= 0x67;
        szToast[oywdm] = SmnWs;
    }

        

    jclass toastClass = env->FindClass(/*android/widget/Toast*/ StrEnc("eCS7dp(}Z:2f/$i/S'CO", "\x04\x2D\x37\x45\x0B\x19\x4C\x52\x2D\x53\x56\x01\x4A\x50\x46\x7B\x3C\x46\x30\x3B", 20).c_str());
    jmethodID makeTextMethod = env->GetStaticMethodID(toastClass, /*makeText*/ StrEnc("jI/4d64U", "\x07\x28\x44\x51\x30\x53\x4C\x21", 8).c_str(), /*(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;*/ StrEnc("H#L{!#l1/A\"2Z.X>f@XyZ3A*Za^XIT6&u0^zeYt%go5U[V!{X6(RXl>-!sFl9cE(l^[%f\\a2XM", "\x60\x6F\x2D\x15\x45\x51\x03\x58\x4B\x6E\x41\x5D\x34\x5A\x3D\x50\x12\x6F\x1B\x16\x34\x47\x24\x52\x2E\x5A\x12\x32\x28\x22\x57\x09\x19\x51\x30\x1D\x4A\x1A\x1C\x44\x15\x3C\x50\x24\x2E\x33\x4F\x18\x3D\x0D\x61\x7B\x14\x0D\x50\x49\x53\x1C\x2F\x08\x16\x14\x2C\x4C\x0B\x3B\x2F\x0A\x32\x33\x00\x41\x2C\x76", 74).c_str());

    
}
jstring native_Check(JNIEnv *env, jclass clazz, jobject mContext, jstring mUserKey) {
    auto userKey = env->GetStringUTFChars(mUserKey, 0);

    std::string hwid = userKey;
    hwid += GetAndroidID(env, mContext);/*https://xyz.com/connect*/ StrEnc("lakyWbF&oaI*n86C`nK<mwLp*0EeCIO=Zvaw", "\x04\x15\x1F\x09\x24\x58\x69\x09\x02\x0E\x2D\x04\x1E\x59\x58\x26\x0C\x1D\x3F\x53\x1F\x12\x62\x03\x5A\x51\x26\x00\x6C\x2A\x20\x53\x34\x13\x02\x03", 36).c_str();/*https://mod.panelstore.space/connect*/ StrEnc("lakyWbF&oaI*n86C`nK<mwLp*0EeCIO=Zvaw", "\x04\x15\x1F\x09\x24\x58\x69\x09\x02\x0E\x2D\x04\x1E\x59\x58\x26\x0C\x1D\x3F\x53\x1F\x12\x62\x03\x5A\x51\x26\x00\x6C\x2A\x20\x53\x34\x13\x02\x03", 36).c_str();/*https://mod.panelstore.space/connect*/ StrEnc("lakyWbF&oaI*n86C`nK<mwLp*0EeCIO=Zvaw", "\x04\x15\x1F\x09\x24\x58\x69\x09\x02\x0E\x2D\x04\x1E\x59\x58\x26\x0C\x1D\x3F\x53\x1F\x12\x62\x03\x5A\x51\x26\x00\x6C\x2A\x20\x53\x34\x13\x02\x03", 36).c_str();/*https://mod.panelstore.space/connect*/ StrEnc("lakyWbF&oaI*n86C`nK<mwLp*0EeCIO=Zvaw", "\x04\x15\x1F\x09\x24\x58\x69\x09\x02\x0E\x2D\x04\x1E\x59\x58\x26\x0C\x1D\x3F\x53\x1F\x12\x62\x03\x5A\x51\x26\x00\x6C\x2A\x20\x53\x34\x13\x02\x03", 36).c_str();
    hwid += GetDeviceModel(env);
    hwid += GetDeviceBrand(env);

    std::string UUID = GetDeviceUniqueIdentifier(env, hwid.c_str());

    std::string errMsg;

    struct MemoryStruct chunk{};
    chunk.memory = (char *) malloc(1);
    chunk.size = 0;

    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, /*POST*/ StrEnc(",IL=", "\x7C\x06\x1F\x69", 4).c_str());
        curl_easy_setopt(curl, CURLOPT_URL, /*https://xyz.com/public/connect*/ StrEnc("\\(jR%=, oT#\"C^li`<F]Sh,u~&-Yoy(kEIplQp6>9w_JJIvz$rnB", "\x34\x5C\x1E\x22\x56\x07\x03\x0F\x02\x3B\x4D\x51\x37\x3B\x1E\x04\x0F\x58\x35\x73\x63\x58\x1C\x02\x1B\x44\x45\x36\x1C\x0D\x49\x1B\x35\x67\x13\x03\x3C\x5F\x46\x4B\x5B\x1B\x36\x29\x65\x2A\x19\x14\x4A\x17\x0D\x36", 52).c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, /*https*/ StrEnc("!mLBO", "\x49\x19\x38\x32\x3C", 5).c_str());
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, /*Content-Type: application/x-www-form-urlencoded*/ StrEnc("@;Ls\\(KP4Qrop`b#d3094/r1cf<c<=H)AiiBG6i|Ta66s2[", "\x03\x54\x22\x07\x39\x46\x3F\x7D\x60\x28\x02\x0A\x4A\x40\x03\x53\x14\x5F\x59\x5A\x55\x5B\x1B\x5E\x0D\x49\x44\x4E\x4B\x4A\x3F\x04\x27\x06\x1B\x2F\x6A\x43\x1B\x10\x31\x0F\x55\x59\x17\x57\x3F", 47).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        char data[4096];
        sprintf(data, /*game=PUBG&user_key=%s&serial=%s*/ StrEnc("qu2yXK,YkJyGD@ut0.u~Nb'5(:.:chK", "\x16\x14\x5F\x1C\x65\x1B\x79\x1B\x2C\x6C\x0C\x34\x21\x32\x2A\x1F\x55\x57\x48\x5B\x3D\x44\x54\x50\x5A\x53\x4F\x56\x5E\x4D\x38", 31).c_str(), userKey, UUID.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            try {
                json result = json::parse(chunk.memory);
                if (result[/*status*/ StrEnc("(>_LBm", "\x5B\x4A\x3E\x38\x37\x1E", 6).c_str()] == true) {
                    std::string token = result[/*data*/ StrEnc("fAVA", "\x02\x20\x22\x20", 4).c_str()][/*token*/ StrEnc("{>3Lr", "\x0F\x51\x58\x29\x1C", 5).c_str()].get<std::string>();
                    time_t rng = result[/*data*/ StrEnc("fAVA", "\x02\x20\x22\x20", 4).c_str()][/*rng*/ StrEnc("+n,", "\x59\x00\x4B", 3).c_str()].get<time_t>();
                    if (rng + 30 > time(0)) {
                        std::string auth = /*PUBG*/ StrEnc("Q*) ", "\x01\x7F\x6B\x67", 4).c_str();;
                        auth += "-";
                        auth += userKey;
                        auth += "-";
                        auth += UUID;
                        auth += "-";
                        auth += /*Vm8Lk7Uj2JmsjCPVPVjrLa7zgfx3uz9E*/ StrEnc("ZD$_K NtaM8Fu=n0fFyO;!Ae<H)*Gy4%", "\x0C\x29\x1C\x13\x20\x17\x1B\x1E\x53\x07\x55\x35\x1F\x7E\x3E\x66\x36\x10\x13\x3D\x77\x40\x76\x1F\x5B\x2E\x51\x19\x32\x03\x0D\x60", 32).c_str();;
                        std::string outputAuth = Tools::CalcMD5(auth);

                        g_Token = token;
                        g_Auth = outputAuth;

                        bValid = g_Token == g_Auth;

                        if (bValid) {
                            pthread_t t;
                          pthread_create(&t, 0, Main_Thread, 0);
                        }
                    }
                } else {
                    errMsg = result[/*reason*/ StrEnc("LW(3(c", "\x3E\x32\x49\x40\x47\x0D", 6).c_str()].get<std::string>();
                }
            } catch (json::exception &e) {
                errMsg = "{";
                errMsg += e.what();
                errMsg += "}\n{";
                errMsg += chunk.memory;
                errMsg += "}";
            }
        } else {
            errMsg = curl_easy_strerror(res);
        }
    }
    curl_easy_cleanup(curl);
    return bValid ? env->NewStringUTF(/*OK*/ StrEnc("8q", "\x77\x3A", 2).c_str()) : env->NewStringUTF(errMsg.c_str());
}

