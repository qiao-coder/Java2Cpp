package com.example.java2cpp;

import static org.junit.Assert.assertEquals;

import androidx.test.ext.junit.runners.AndroidJUnit4;

import org.junit.Test;
import org.junit.runner.RunWith;

/**
 * @author wuzhanqiao
 * @date 2022/3/6.
 */
@RunWith(AndroidJUnit4.class)
public class MediaPlayerTest {
    @Test
    public void testPrepareAsync() throws InterruptedException {
        MediaPlayer mediaPlayer = new MediaPlayer();
        mediaPlayer.setOnPreparedListener(msg -> {
            assertEquals(1, msg);
        });
        mediaPlayer.prepareAsync();

        mediaPlayer.waitForTest();
    }
}
