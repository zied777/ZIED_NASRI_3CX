#ifndef OBOE_RECORDER_AUDIORECORDER_H
#define OBOE_RECORDER_AUDIORECORDER_H

#ifndef MODULE_NAME
#define MODULE_NAME  "AudioRecorder"
#endif

#include <oboe/Definitions.h>
#include <oboe/AudioStream.h>
#include "RecordingCallback.h"
#include <android/log.h>
#include <jni.h>

class AudioRecorder {

public:
    ~AudioRecorder();

    RecordingCallback recordingCallback = RecordingCallback();

    void startRecording(_jstring *filePath);
    void stopRecording();
private:

    const char* TAG = "AudioRecorder::";

    int32_t mRecordingDeviceId = oboe::VoiceRecognition;
    oboe::AudioFormat mFormat = oboe::AudioFormat::I16;
    int32_t mInputChannelCount = oboe::ChannelCount::Stereo;

    oboe::AudioApi mAudioApi = oboe::AudioApi::AAudio;
    oboe::AudioStream *mRecordingStream = nullptr;

    void openRecordingStream();

    void startStream(oboe::AudioStream *stream);
    void stopStream(oboe::AudioStream *stream);
    void closeStream(oboe::AudioStream *stream);

    oboe::AudioStreamBuilder* setUpRecordingStreamParameters(oboe::AudioStreamBuilder* builder);
};

#endif