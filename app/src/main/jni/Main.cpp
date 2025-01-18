#include <list>
#include <vector>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include "ESP/ESP.h"
#include "ESP/Hacks.h"
#include "Includes/base64.hpp"
#include "KittyMemory/MemoryPatch.h"
#include "Includes/Logger.h"
#include "Includes/Utils.h"
#include "Includes/obfuscate.h"
#include "PUBG.h"
using namespace std;


// fancy struct for patches for kittyMemory
struct My_Patches {
 MemoryPatch           
Bypass;
} MonsterHack; bool 

featureHookToggle = false;
void *instanceBtn;
ESP espOverlay;


void *antiLeech(void *) {
    sleep(15);
    }
   

void native_onSwitch(JNIEnv*, jclass clazz, int code, jboolean jboolean1)
{
    switch ((int) code) {

    }
}



jclass (*orig_FindClass)(JNIEnv *env, const char *name);
int Register1(JNIEnv *env) {
    JNINativeMethod methods[] = {{"onSendConfig", "(Ljava/lang/String;J)V",(void *) native_onSendConfig},{"onSendConfig2", "(Ljava/lang/String;Ljava/lang/String;)V", (void *)native_onSendConfig2},{"Switch", "(IZ)V", (void *) native_onSwitch}};
    jclass clazz = env->FindClass("team/pmods/MenuEngine");
    if (!clazz)
        return -1;

    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) != 0)
        return -1;

    return 0;
}
int Register2(JNIEnv *env) {
    JNINativeMethod methods[] = {{"Init",  "(Landroid/content/Context;)V",                   (void *) native_Init},
	                             {"Check", "(Landroid/content/Context;Ljava/lang/String;)Ljava/lang/String;", (void *) native_Check}};

    jclass clazz = env->FindClass("team/pmods/Launcher");
    if (!clazz)
        return -1;

    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) != 0)
        return -1;

    return 0;
}




void (*old_Update)(void *instance);
void Update(void *instance) { instanceBtn = instance; old_Update(instance); }
void *Super_thread(void *) { ProcMap il2cppMap; do { il2cppMap = KittyMemory::getLibraryMap("libUE4.so");  sleep(1); } while(!il2cppMap.isValid());
                                                    

return NULL;

}
void *hack_thread(void *) { ProcMap il2cppMap; do { il2cppMap = KittyMemory::getLibraryMap("libtersafe.so");  sleep(1); } while(!il2cppMap.isValid());   
                                         
    return NULL;
}
__attribute__((constructor))
void initializer() {
    // Create a new thread so it does not block the main thread, means the game would not freeze
    
    pthread_t ptid;
    pthread_create(&ptid, NULL, Super_thread, NULL);
}



JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *globalEnv;
    vm->GetEnv((void **) &globalEnv, JNI_VERSION_1_6);
    
     if (Register1(globalEnv) != 0)
        return -1;
    if (Register2(globalEnv) != 0)
        return -1;


    pthread_t ptid;   
    pthread_create(&ptid, NULL, hack_thread, NULL);
    return JNI_VERSION_1_6;
}


JNIEXPORT void JNICALL
JNI_OnUnload(JavaVM *vm, void *reserved) {
	}
	
   
