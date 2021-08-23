package com.zied.nasri.zied_nasri_3cx_test;

import android.Manifest;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.provider.Settings;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

public abstract class BaseActivity extends AppCompatActivity {


    private int AUDIO_RECORD_REQUEST = 17779;
    private String[] PERMISSIONS = new String[]{
            Manifest.permission.RECORD_AUDIO,
            Manifest.permission.READ_EXTERNAL_STORAGE,
            Manifest.permission.WRITE_EXTERNAL_STORAGE
    };

    boolean checkRecordAudioPermission(){

        boolean isRecordingAllowed = isRecordPermissionGranted();
        if (!isRecordingAllowed) {

            requestRecordPermission();
            disableControls();

        } else {

            enableControls();

        }

        return isRecordingAllowed;
    }

    private boolean isRecordPermissionGranted(){

        boolean permissionStatus = (ActivityCompat
                .checkSelfPermission(
                        this,
                        Manifest.permission.RECORD_AUDIO
                ) == PackageManager.PERMISSION_GRANTED) &&
                (ActivityCompat
                        .checkSelfPermission(
                                this,
                                Manifest.permission.READ_EXTERNAL_STORAGE
                        ) == PackageManager.PERMISSION_GRANTED) &&
                (ActivityCompat
                        .checkSelfPermission(
                                this,
                                Manifest.permission.WRITE_EXTERNAL_STORAGE
                        ) == PackageManager.PERMISSION_GRANTED);


        return permissionStatus;
    }

    private void requestRecordPermission() {

        ActivityCompat.requestPermissions(this, PERMISSIONS, AUDIO_RECORD_REQUEST);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);

        if (AUDIO_RECORD_REQUEST != requestCode) {
            return;
        }

        if (!isRecordPermissionGranted()) {
            disableControls();
        } else {
            enableControls();
        }
    }

    abstract void disableControls();
    abstract void enableControls();
}
