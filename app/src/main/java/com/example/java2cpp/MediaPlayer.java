package com.example.java2cpp;

import androidx.annotation.VisibleForTesting;

import com.example.java2cpp.annotation.CallByNative;
import com.example.java2cpp.listener.OnPreparedListener;

import java.lang.ref.WeakReference;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

/**
 * @author wuzhanqiao
 * @date 2022/3/5.
 */
public class MediaPlayer {
    static {
        System.loadLibrary("mediaplayer");
    }

    @CallByNative
    private long mNativeContext;
    private OnPreparedListener mOnPreparedListener;
    private ReentrantLock mTestLock = new ReentrantLock();
    private Condition mTestCondition = mTestLock.newCondition();
    private boolean isTesting;


    public MediaPlayer() {
        native_setup(new WeakReference<>(this));
    }

    private native void native_setup(Object weak_this);

    private native void native_finalize();

    public native void prepareAsync();

    @CallByNative
    private static void postEventFromNative(Object mediaplayer_weak_ref, int msg) {
        MediaPlayer mp = (MediaPlayer) ((WeakReference) mediaplayer_weak_ref).get();
        if (mp == null) return;
        if (mp.mOnPreparedListener != null) {
            mp.mOnPreparedListener.onPrepared(msg);
        }
        if (mp.isTesting) mp.finishTest();
    }

    private void finishTest() {
        isTesting = false;
        mTestLock.lock();
        try {
            mTestCondition.signal();
        } finally {
            mTestLock.unlock();
        }
    }


    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        native_finalize();
    }

    public void setOnPreparedListener(OnPreparedListener listener) {
        mOnPreparedListener = listener;
    }

    @VisibleForTesting
    public void waitForTest() throws InterruptedException {
        isTesting = true;
        mTestLock.lock();
        try {
            mTestCondition.await();
        } finally {
            mTestLock.unlock();
        }
    }
}
