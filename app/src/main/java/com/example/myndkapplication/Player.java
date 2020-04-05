package com.example.myndkapplication;

import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import org.jetbrains.annotations.Nullable;

public class Player implements SurfaceHolder.Callback {
    static {
        System.loadLibrary("native-lib");
    }

    private String dataSource;
    private SurfaceHolder surfaceHolder;


    public void setDataSource(String dataSource) {
        this.dataSource = dataSource;
    }

    public void prepare() {
        prepareNative(dataSource);
    }

    public void start() {
        startNative();
    }

    public void onPrepared() {
        if (mOnPrepare != null) {
            mOnPrepare.onPrepared();
        }
    }

    private OnPrepare mOnPrepare;

    public void setOnPrepared(OnPrepare onPrepare) {
        mOnPrepare = onPrepare;
    }

    public void setSurfaceView(@Nullable SurfaceView surfaceView) {
        if (surfaceHolder != null) {
            surfaceHolder.removeCallback(this);
        }
        surfaceHolder = surfaceView.getHolder();
        surfaceHolder.addCallback(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        setSurfaceNative(holder.getSurface());
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    interface OnPrepare{
        void onPrepared();
    }

    private native void prepareNative(String dataSource);

    private native void startNative();
    private native void setSurfaceNative(Surface surface);
}
