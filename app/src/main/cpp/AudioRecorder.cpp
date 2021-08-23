#include <oboe/Oboe.h>
#include "AudioRecorder.h"

AudioRecorder::~AudioRecorder() {
    stopStream(mRecordingStream);
    closeStream(mRecordingStream);
}

void AudioRecorder::startRecording(_jstring *filePath) {

    __android_log_print(ANDROID_LOG_DEBUG, TAG, "startRecording()");

    openRecordingStream();

    if (mRecordingStream) {
        startStream(mRecordingStream);
    } else {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "startRecording(): Failed to create recording (%p) stream", mRecordingStream);
        closeStream(mRecordingStream);
    }

}

void AudioRecorder::stopRecording() {

    __android_log_print(ANDROID_LOG_DEBUG, TAG, "stopRecording()");

    stopStream(mRecordingStream);
    closeStream(mRecordingStream);

}


void AudioRecorder::openRecordingStream() {

    __android_log_print(ANDROID_LOG_DEBUG, TAG, "openRecordingStream()");

    oboe::AudioStreamBuilder builder;

    setUpRecordingStreamParameters(&builder);

    oboe::Result result = builder.openStream(&mRecordingStream);
    if (result == oboe::Result::OK && mRecordingStream) {

        mFormat = mRecordingStream->getFormat();

    } else {
        __android_log_print(ANDROID_LOG_ERROR, TAG,  "Failed to create recording stream. Error: %s",oboe::convertToText(result));
    }

}

void AudioRecorder::startStream(oboe::AudioStream *stream) {

    __android_log_print(ANDROID_LOG_DEBUG, TAG, "startStream()");

    if (stream) {
        oboe::Result result = stream->requestStart();
        if (result != oboe::Result::OK) {
            __android_log_print(ANDROID_LOG_ERROR, TAG, "Error starting stream. %s", oboe::convertToText(result));
        }
    }

}

void AudioRecorder::stopStream(oboe::AudioStream *stream) {

    __android_log_print(ANDROID_LOG_DEBUG, TAG, "stopStream()");

    if (stream) {
        oboe::Result result = stream->stop(0L);
        if (result != oboe::Result::OK) {
            __android_log_print(ANDROID_LOG_ERROR, TAG,  "Error stopping stream. %s", oboe::convertToText(result));
        }
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "stopStream(): mTotalSamples = ");
    }

}

void AudioRecorder::closeStream(oboe::AudioStream *stream) {

    __android_log_print(ANDROID_LOG_DEBUG, TAG, "closeStream()");

    if (stream) {
        oboe::Result result = stream->close();
        if (result != oboe::Result::OK) {
            __android_log_print(ANDROID_LOG_DEBUG, TAG, "Error closing stream. %s", oboe::convertToText(result));
        } else {
            stream = nullptr;
        }

    }

}

oboe::AudioStreamBuilder *AudioRecorder::setUpRecordingStreamParameters(oboe::AudioStreamBuilder *builder) {

    __android_log_print(ANDROID_LOG_DEBUG, TAG, "setUpRecordingStreamParameters()");

    builder->setAudioApi(mAudioApi)
            ->setFormat(mFormat)
            ->setSharingMode(oboe::SharingMode::Exclusive)
            ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
            ->setCallback(&recordingCallback)
            ->setSampleRate(44100.0f)
            ->setDeviceId(mRecordingDeviceId)
            ->setDirection(oboe::Direction::Input)
            ->setChannelCount(mInputChannelCount);
    return builder;
}


