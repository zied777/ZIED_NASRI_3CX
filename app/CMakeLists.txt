cmake_minimum_required(VERSION 3.10.2)

add_library (audioRecorder
            SHARED src/main/cpp/AudioRecorder.cpp
            src/main/jni/jni_audio_recorder.cpp
            src/main/cpp/RecordingCallback.cpp
            src/main/cpp/TheMostSillyEffect.cpp
        )
find_package (oboe REQUIRED CONFIG)
find_library(log-lib log)
target_link_libraries(audioRecorder ${log-lib} oboe::oboe)