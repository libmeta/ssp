//
// Created by x on 2021/8/24.
//

#pragma once
extern "C" {
#include <libavutil/frame.h>
}

namespace spp::base::FFmpeg {

class MediaFrame : public AVFrame {
public:
    using base = AVFrame;
    using base::base;

    MediaFrame() {
        av_frame_unref(this);
    }

    MediaFrame(int width, int height, int format, int align = 0) : MediaFrame() {
        this->width = width;
        this->height = height;
        this->format = format;
        av_frame_get_buffer(this, align);
    }

    virtual ~MediaFrame() {
        av_frame_unref(this);
    }

};


}
