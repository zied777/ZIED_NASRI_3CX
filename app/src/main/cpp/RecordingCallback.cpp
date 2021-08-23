
#include <android/log.h>
#include "RecordingCallback.h"
#include "../jni/jni_audio_recorder.h"
#include "TheMostSillyEffect.h"

oboe::DataCallbackResult
RecordingCallback::onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) {
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "RecordingCallback::onAudioReady()");
    return processRecordingFrames(audioStream, static_cast<int16_t *>(audioData), numFrames * audioStream->getChannelCount());
}

oboe::DataCallbackResult
RecordingCallback::processRecordingFrames(oboe::AudioStream *audioStream, int16_t *audioData, int32_t numFrames) {

    __android_log_print(ANDROID_LOG_DEBUG, TAG, "RecordingCallback::processRecordingFrames()");

    writeToFile(TheMostSillyEffect::sillySoundEffect(audioData,numFrames), numFrames);
    return oboe::DataCallbackResult::Continue;

}