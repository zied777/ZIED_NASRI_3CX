#include <jni.h>
#include <android/log.h>
#include "../cpp/AudioRecorder.h"


const char *TAG = "jni_audio_recorder::";
static AudioRecorder *audioRecorder = nullptr;

JavaVM *javaVM = NULL;
jclass activityClass;
jobject activityObj;

extern "C" {

JNIEXPORT jboolean  JNICALL
Java_com_zied_nasri_zied_1nasri_13cx_1test_MainActivity_create(JNIEnv *env, jobject) {

    __android_log_print(ANDROID_LOG_DEBUG, TAG, "CREATE()");

    if (audioRecorder == nullptr) {
        audioRecorder = new AudioRecorder();
    }

    return (audioRecorder != nullptr);
}

JNIEXPORT void JNICALL
Java_com_zied_nasri_zied_1nasri_13cx_1test_MainActivity_delete(JNIEnv *env, jobject) {

    __android_log_print(ANDROID_LOG_DEBUG, TAG, "CREATE()");

    delete audioRecorder;
    audioRecorder = nullptr;

}

JNIEXPORT void JNICALL
Java_com_zied_nasri_zied_1nasri_13cx_1test_MainActivity_startRecording(JNIEnv *env, jobject obj,
                                                                       jstring filePath) {

    env->GetJavaVM(&javaVM);
    jclass cls = env->GetObjectClass(obj);
    activityClass = (jclass) env->NewGlobalRef(cls);
    activityObj = env->NewGlobalRef(obj);

    __android_log_print(ANDROID_LOG_DEBUG, TAG, "startRecording()");

    if (audioRecorder == nullptr) {
        __android_log_print(ANDROID_LOG_ERROR, TAG,
                            "audioEngine is null, you must call create() method before calling this method");
        return;
    }

    audioRecorder->startRecording(filePath);

}

JNIEXPORT void JNICALL
Java_com_zied_nasri_zied_1nasri_13cx_1test_MainActivity_stopRecording(JNIEnv *env, jobject) {

    __android_log_print(ANDROID_LOG_DEBUG, TAG, "stopRecording()");

    if (audioRecorder == nullptr) {
        __android_log_print(ANDROID_LOG_ERROR, TAG,
                            "audioEngine is null, you must call create() method before calling this method");

        return;
    }

    audioRecorder->stopRecording();

}


void writeToFile( int16_t *audioData, int32_t numSamples) {
    JNIEnv *env;
    javaVM->AttachCurrentThread(&env, NULL);
    jmethodID method = env->GetMethodID(activityClass, "writeToFile", "([B)V");

    jbyteArray result;
    result = env->NewByteArray(numSamples);
    if (result == NULL) {
        return;
    }
    env->SetByteArrayRegion(result, 0, numSamples, reinterpret_cast<const jbyte *>(audioData));
    env->CallVoidMethod(activityObj, method, result);
}


};