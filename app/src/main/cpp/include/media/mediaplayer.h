//
// Created by wuzhanqiao on 2022/3/5.
//

#ifndef JAVA2CPP_MEDIAPLAYER_H
#define JAVA2CPP_MEDIAPLAYER_H

enum media_event_type {
    MEDIA_PREPARED = 1,
};

class MediaPlayerListener {
public:
    virtual void notify(int msg) = 0;
};


class MediaPlayer {

public:
    ~MediaPlayer();

    void setListener(MediaPlayerListener *listener);

    void prepareAsync();

    void notify(int msg);

private:
    MediaPlayerListener *mListener;

    void doPrepare();
};


#endif //JAVA2CPP_MEDIAPLAYER_H
