package com.zied.nasri.zied_nasri_3cx_test;

import android.content.Context;
import android.provider.ContactsContract;
import android.util.Log;

import java.io.File;
import java.io.IOException;

public class Utils {

    private static final String TAG = "Utils";

    public static String getAudioRecordingFilePath(Context context) throws IOException {

        File imagePath = new File(context.getExternalFilesDir(null), "audio");
        if (!imagePath.exists()) {
            boolean directoryCreationStatus = imagePath.mkdirs();
            Log.i(TAG, "getAudioRecordingFilePath: directoryCreationStatus: "+directoryCreationStatus);
        }

        File newFile = new File(imagePath, "oboe_recording.pcm");
        String filePath = newFile.getAbsolutePath();

        Log.d(TAG, "getAudioRecordingFilePath: filePath: "+filePath+", fileExists: "+newFile.exists());

        if (newFile.exists()) {
            boolean deletionStatus = newFile.delete();
            Log.i(TAG, "getAudioRecordingFilePath: File already exists, delete it first, deletionStatus: "+deletionStatus);
        }

        if (!newFile.exists()) {
            boolean fileCreationStatus = newFile.createNewFile();
            Log.i(TAG, "getAudioRecordingFilePath: fileCreationStatus: "+fileCreationStatus);
        }

        return filePath;
    }
}
