//
// Created by x on 2021/8/24.
//

#pragma once

extern "C" {
#include <libavcodec/packet.h>
}

namespace ssp::base::FFmpeg {

class MediaPacket : public AVPacket {
public :
    using base = AVPacket;
    using base::base;

    MediaPacket() {
        av_init_packet(this);
    }

    MediaPacket(size_t size) {
        auto ret = av_new_packet(this, size);
    }

    virtual ~MediaPacket() {
        av_packet_unref(this);
    }

};


}
