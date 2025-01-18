#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>

#include <vector>
#include <map>
#include <chrono>
#include <fstream>
#include <thread>

#include <pthread.h>
#include <dirent.h>
#include <libgen.h>

#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/uio.h>

#include <fcntl.h>

#include <jni.h>
#include <android/log.h>

#include <sys/syscall.h>
#include <linux/input-event-codes.h>
#include <sys/system_properties.h>
#include <linux/input.h>

#include "XorStr.h"

using namespace std;

#define IS_DEBUG

#if defined(IS_DEBUG)
#define LOG_TAG "PUT LOGGER"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else
#define LOGI(...)
#define LOGD(...)
#define LOGW(...)
#define LOGE(...)


#define GL
//#define KR

#if defined(GL)
#define GAME_PACKAGE "com.tencent.ig"
#elif defined(KR)
#define GAME_PACKAGE "com.tencent.ig"
#endif

#endif
