
#ifndef ZIED_NASRI_3CX_TEST_JNI_AUDIO_RECORDER_H
#define ZIED_NASRI_3CX_TEST_JNI_AUDIO_RECORDER_H

extern "C" {

    void writeToFile(int16_t *audioData,int32_t numSamples);

};
#endif
