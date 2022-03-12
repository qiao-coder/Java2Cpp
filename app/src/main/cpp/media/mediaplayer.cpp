//
// Created by wuzhanqiao on 2022/3/5.
//

#include "media/mediaplayer.h"
#include <thread>
#include <chrono>

void MediaPlayer::setListener(MediaPlayerListener *listener) {
    mListener = listener;
}

MediaPlayer::~MediaPlayer() {
    if (mListener) {
        delete mListener;
        mListener = 0;
    }
}

void MediaPlayer::prepareAsync() {
    new std::thread (&MediaPlayer::doPrepare, this);
}

void MediaPlayer::doPrepare() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    notify(MEDIA_PREPARED);
}

void MediaPlayer::notify(int msg) {
    if (mListener != nullptr) {
        mListener->notify(msg);
    }
}