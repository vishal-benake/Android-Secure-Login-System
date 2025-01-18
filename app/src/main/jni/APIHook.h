#include "Includes.h"
#include "Tools.h"
#include "XorStr.h"

// ============================================================================================= //
int (*Orig_open)(const char *filename, int oflag, ...);

FILE *(*Orig_fopen)(char *filename, char *modes);

struct dirent *(*Orig_readdir)(DIR *dirp);

int (*Orig_snprintf)(char *s, int size, const char *format, ...);

int (*Orig_sscanf)(char *s, char *format, ...);

char *(*Orig_fgets)(const char *s, int size, FILE *stream);

FILE *(*Orig_popen)(char *cmd, char *mode);

int (*Orig_system)(char *s);

// ============================================================================================= //

#define FAKE_APK XorStr("cpu_cache0")
#define MAIN_LIB XorStr("lib6h.so")

char szRedirectPath[512];

// ============================================================================================= //
int Proxy_open(const char *filename, int oflag, ...) {
    auto lr = __builtin_return_address(0);

    va_list list;
    va_start(list, oflag);

    long arg0 = va_arg(list, long);
    long arg1 = va_arg(list, long);
    long arg2 = va_arg(list, long);
    long arg3 = va_arg(list, long);
    long arg4 = va_arg(list, long);
    long arg5 = va_arg(list, long);

    va_end(list);

    if (strstr(filename, XorStr(".apk")) && strstr(filename, GAME_PACKAGE)) {
        return Orig_open(szRedirectPath, oflag, arg0, arg1, arg2, arg3, arg4, arg5);
    }
    if (strstr(filename, MAIN_LIB)) {
        return -1;
    }
    return Orig_open(filename, oflag, arg0, arg1, arg2, arg3, arg4, arg5);
}

// ============================================================================================= //
FILE *Proxy_fopen(char *filename, char *modes) {
    if (strstr(filename, XorStr(".apk")) && strstr(filename, GAME_PACKAGE)) {
        return Orig_fopen(szRedirectPath, modes);
    }
    if (strstr(filename, MAIN_LIB)) {
        return NULL;
    }
    return Orig_fopen(filename, modes);
}

// ============================================================================================= //

int Proxy_sscanf(char *s, char *format, ...) {
    va_list list;
    va_start(list, format);
    int result = vsscanf(s, format, list);
    va_end(list);

    return result;
}

// ============================================================================================= //
struct dirent *Proxy_readdir(DIR *dirp) {
    auto result = Orig_readdir(dirp);
    check:
    if (result) {
        if (strstr(result->d_name, FAKE_APK) || strstr(result->d_name, MAIN_LIB)) {
            result = Orig_readdir(dirp);
            goto check;
        }
    }
    return result;
}

// ============================================================================================= //
int Proxy_snprintf(char *s, int size, char *format, ...) {
    va_list list;
    va_start(list, format);
    int result = vsnprintf(s, size, format, list);
    va_end(list);

    return result;
}

// ============================================================================================= //
char *Proxy_fgets(const char *s, int size, FILE *stream) {
    auto result = Orig_fgets(s, size, stream);
    check:
    if (result) {
        if (strstr(s, MAIN_LIB)) {
            result = Orig_fgets(s, size, stream);
            goto check;
        }
    }

    return result;
}

// ============================================================================================= //
FILE *Proxy_popen(char *cmd, char *mode) {
    return Orig_popen(cmd, mode);
}

// ============================================================================================= //
int Proxy_system(char *s) {
    return Orig_system(s);
}

// ============================================================================================= //
long (*o_sub_6D090)(long a1, void *a2, void *a3, void *a4);

long sub_6D090(long a1, void *a2, void *a3, void *a4) {
    LOGI("syscall: %d | %p | %p | %p", a1, a2, a3, a4);
    if (a1 == 5) {
        LOGI("syscall - open: %s | %p | %p", a2, a3, a4);
        const char *filename = (const char *) a2;
        if (strstr(filename, FAKE_APK)) {
            return -1;
        }
        if (strstr(filename, MAIN_LIB)) {
            return -1;
        }
    }
    if (a1 == 33) {
        LOGI("syscall - access: %s | %p", a2, a3);
        const char *filename = (const char *) a2;
        if (strstr(filename, FAKE_APK)) {
            return -1;
        }
        if (strstr(filename, MAIN_LIB)) {
            return -1;
        }
    }
    return o_sub_6D090(a1, a2, a3, a4);
}

// ============================================================================================= //
void *TersafeAPI(void *) {
    uintptr_t tersafeBase = Tools::GetMapAddr("libtersafe.so");
    while (!tersafeBase) {
        tersafeBase = Tools::GetMapAddr("libtersafe.so");
        sleep(1);
    }

    LOGI("libtersafe.so: %p", tersafeBase);
    Tools::HookFunction((void *) (tersafeBase + 0x6D090), (void *) &sub_6D090, (void **) &o_sub_6D090);
    return 0;
}

// ============================================================================================= //
void APIHook(const char *redirectPath) {
    strcpy(szRedirectPath, redirectPath);

    pthread_t t;
    pthread_create(&t, 0, TersafeAPI, 0);

    Tools::HookFunction((void *) dlsym(RTLD_DEFAULT, XorStr("open")), (void *) &Proxy_open, (void **) &Orig_open);
    Tools::HookFunction((void *) dlsym(RTLD_DEFAULT, XorStr("popen")), (void *) &Proxy_popen, (void **) &Orig_popen);
    Tools::HookFunction((void *) dlsym(RTLD_DEFAULT, XorStr("fopen")), (void *) &Proxy_fopen, (void **) &Orig_fopen);
    Tools::HookFunction((void *) dlsym(RTLD_DEFAULT, XorStr("fgets")), (void *) &Proxy_fgets, (void **) &Orig_fgets);
    Tools::HookFunction((void *) dlsym(RTLD_DEFAULT, XorStr("sscanf")), (void *) &Proxy_sscanf, (void **) &Orig_sscanf);
    Tools::HookFunction((void *) dlsym(RTLD_DEFAULT, XorStr("readdir")), (void *) &Proxy_readdir, (void **) &Orig_readdir);
    Tools::HookFunction((void *) dlsym(RTLD_DEFAULT, XorStr("snprintf")), (void *) &Proxy_snprintf, (void **) &Orig_snprintf);
    Tools::HookFunction((void *) dlsym(RTLD_DEFAULT, XorStr("system")), (void *) &Proxy_system, (void **) &Orig_system);
    //Tools::HookFunction((void *) dlsym(RTLD_DEFAULT, XorStr("access")), (void *) &Proxy_access, (void **) &Orig_access);
}