package com.zied.nasri.zied_nasri_3cx_test;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import androidx.core.content.FileProvider;

import com.google.android.material.button.MaterialButton;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import Audio.WavEncoder;
import io.reactivex.rxjava3.annotations.NonNull;
import io.reactivex.rxjava3.core.Observable;
import io.reactivex.rxjava3.core.Observer;
import io.reactivex.rxjava3.disposables.Disposable;
import io.reactivex.rxjava3.schedulers.Schedulers;
import io.reactivex.rxjava3.subjects.PublishSubject;


public class MainActivity extends BaseActivity {

    private static final String TAG = "MainActivity::";
    private String mFilePath = "";
    FileOutputStream stream = null;

    PublishSubject cSubject = PublishSubject.create();

    private final Observable cClickObservable = cSubject.hide();
    private final Observable cStartRecodingObservable = cClickObservable.filter(title -> title.equals(getResources().getString(R.string.start_record)));
    private final Observable cStopRecording = cClickObservable.filter(title -> title.equals(getResources().getString(R.string.stop_record)));
    private final Observable mPlayObservable = cClickObservable.filter(title -> title.equals(getResources().getString(R.string.play)));

    PublishSubject cWriteToFileSubject = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        create();
        this.checkRecordAudioPermission();
        Button recordButton = findViewById(R.id.record_bt);
        Button playButton = findViewById(R.id.play_bt);

        recordButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                cSubject.onNext(((Button) view).getText());
            }
        });
        playButton.setEnabled(false);
        playButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                cSubject.onNext(((Button) view).getText());
            }
        });

        cStartRecodingObservable
                .map(event -> {
                    if (!checkRecordAudioPermission()) {
                        return false;
                    }
                    return true;
                })
                .observeOn(Schedulers.newThread())
                .subscribe(t -> {

                    if(!((boolean)t))return;
                    mFilePath = Utils.getAudioRecordingFilePath(this);
                    stream = new FileOutputStream(mFilePath);
                    startRecording(mFilePath);

                    this.cWriteToFileSubject = PublishSubject.create();
                    this.cWriteToFileSubject.hide().observeOn(Schedulers.newThread()).subscribe(new Observer() {
                        @Override
                        public void onSubscribe(@NonNull Disposable d) {

                        }

                        @Override
                        public void onNext(@NonNull Object data) {
                            Log.d(TAG, "#### Writing to File::" + ((byte[]) data).length);

                            try {
                                stream.write((byte[]) data);
                            } catch (IOException e) {
                                e.printStackTrace();
                            }
                        }

                        @Override
                        public void onError(@NonNull Throwable e) {

                        }

                        @Override
                        public void onComplete() {
                            try {
                                stream.getFD().sync();
                                stream.close();

                                File f1 = new File(mFilePath);
                                WavEncoder.rawToWave(f1, new File(mFilePath.replace("pcm", "wav")));
                                f1.delete();
                            } catch (IOException e) {
                                e.printStackTrace();
                            }
                        }
                    });

                    MainActivity.this.runOnUiThread(new Runnable() {
                        @Override
                        public void run() {

                            playButton.setEnabled(false);
                            recordButton.setText(R.string.stop_record);
                            ((MaterialButton) recordButton).setIconResource(R.drawable.ic_baseline_stop_24);
                        }
                    });
                });

        cStopRecording.observeOn(Schedulers.newThread()).subscribe(t -> {

            stopRecording();
            this.cWriteToFileSubject.onComplete();
            MainActivity.this.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    playButton.setEnabled(true);
                    recordButton.setText(R.string.start_record);
                    ((MaterialButton) recordButton).setIconResource(R.drawable.ic_baseline_record_voice_over_24);
                }
            });
        });

        mPlayObservable.subscribe(t -> {

            File file = new File(mFilePath.replace("pcm","wav"));
            Uri recordURI = FileProvider.getUriForFile(this, getApplicationContext().getPackageName() + ".provider", file);
            Intent i = new Intent();
            i.setAction(android.content.Intent.ACTION_VIEW);
            i.setDataAndType(recordURI, "audio/*");
            startActivity(Intent.createChooser(i, "Play Record"));
        });


    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        delete();
    }

    @Override
    void disableControls() {

        findViewById(R.id.record_bt).setEnabled(false);
        findViewById(R.id.play_bt).setEnabled(false);

    }

    @Override
    void enableControls() {

        findViewById(R.id.record_bt).setEnabled(true);
        findViewById(R.id.play_bt).setEnabled(true);
    }

    //JNI -> Java
    void writeToFile(byte[] data) {

        this.cWriteToFileSubject.onNext(data);

    }


    //Java -> JNI
    static {
        System.loadLibrary("audioRecorder");
    }

    public native boolean create();

    public native void delete();

    public native void startRecording(String mFilePath);

    public native void stopRecording();
}