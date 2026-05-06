#include <jni.h>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <android/log.h>
#include <sys/ptrace.h>
#include <unistd.h>

#define LOG_TAG "ANTI_DEBUG_LAB23"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// --------------------------------------------------
// Contrôle 1 : Détection de débogueur
// --------------------------------------------------
static bool detectTracer() {
    long ptraceResult = ptrace(PTRACE_TRACEME, 0, 0, 0);
    if (ptraceResult == -1) {
        LOGE(">>> ALERTE: Processus trace ou debogue <<<");
        return true;
    }
    LOGI("Check ptrace: aucun traceur detecte");
    return false;
}

// --------------------------------------------------
// Contrôle 2 : Inspection mémoire /proc/self/maps
// --------------------------------------------------
static bool findHookingLibraries() {
    FILE* mapsFile = fopen("/proc/self/maps", "r");
    if (!mapsFile) {
        LOGW("Impossible de lire /proc/self/maps");
        return false;
    }

    char buffer[1024];
    const char* blacklist[] = {
        "frida",
        "xposed", 
        "libfrida",
        "gdbserver",
        "libgdb",
        "magisk",
        "substrate",
        "cydia"
    };
    int blacklistSize = 8;

    while (fgets(buffer, sizeof(buffer), mapsFile)) {
        for (int i = 0; i < blacklistSize; i++) {
            if (strstr(buffer, blacklist[i])) {
                LOGE(">>> BIBLIOTHEQUE SUSPECTE TROUVEE: %s", blacklist[i]);
                LOGE(">>> Ligne complete: %s", buffer);
                fclose(mapsFile);
                return true;
            }
        }
    }

    fclose(mapsFile);
    LOGI("Check maps: aucune bibliotheque suspecte");
    return false;
}

// --------------------------------------------------
// Méthode JNI principale de détection
// --------------------------------------------------
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_example_lab22_1dev_MainActivity_isUnderInspection(
        JNIEnv* env,
        jobject /* this */) {

    bool tracerFound = detectTracer();
    bool hookLibFound = findHookingLibraries();

    if (tracerFound || hookLibFound) {
        LOGE("=== ENVIRONNEMENT NON SECURISE DETECTE ===");
        return JNI_TRUE;
    }

    LOGI("=== ENVIRONNEMENT SECURISE ===");
    return JNI_FALSE;
}

// --------------------------------------------------
// Méthode JNI existante (à conserver)
// --------------------------------------------------
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_lab22_1dev_MainActivity_getNativeData(
        JNIEnv* env,
        jobject /* this */) {
    LOGI("getNativeData() appelee");
    return env->NewStringUTF("Message depuis le code natif C++!");
}

// --------------------------------------------------
// Méthode JNI existante (à conserver)
// --------------------------------------------------
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_lab22_1dev_MainActivity_calculateFactorial(
        JNIEnv* env,
        jobject /* this */,
        jint value) {

    LOGI("calculateFactorial(%d) appelee", value);

    if (value < 0) {
        LOGE("Erreur: valeur negative interdite");
        return -1;
    }

    long long factResult = 1;
    for (int i = 1; i <= value; i++) {
        factResult *= i;
    }

    LOGI("Resultat factoriel: %d! = %lld", value, factResult);
    return static_cast<jint>(factResult);
}
