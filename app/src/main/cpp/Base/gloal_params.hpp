//
// Created by x on 2021/8/23.
//

#pragma once

#include <ratio>

namespace ssp::base {

struct GloalParams final {

    using _2160Px = std::ratio<2160, 3840>;
    using _1080Px = std::ratio<1080, 1920>;
    using _720Px = std::ratio<720, 1280>;
    using _540Px = std::ratio<540, 960>;
    using _360Px = std::ratio<360, 640>;

    using _0Degree = std::ratio<0>;
    using _90Degree = std::ratio<90>;
    using _270Degree = std::ratio<270>;

    using _15Fps = std::ratio<15>;
    using _20Fps = std::ratio<20>;
    using _23_97Fps = std::ratio<24'000, 1'001>;
    using _25Fps = std::ratio<25>;
    using _29_97Fps = std::ratio<30'000, 1'001>;
    using _30Fps = std::ratio<30>;
    using _50Fps = std::ratio<50>;
    using _60Fps = std::ratio<60>;

    using _32000Hz = std::ratio<32'000>;
    using _44100Hz = std::ratio<44'100>;
    using _48000Hz = std::ratio<48'000>;

    using _1000KBps = std::ratio<1'000'000>;
    using _2000KBps = std::ratio<2'000'000>;
    using _4000KBps = std::ratio<4'000'000>;
    using _6000KBps = std::ratio<6'000'000>;

    using _1024n = std::ratio<1024>;
    using _1152n = std::ratio<1152>;

    using _BaseTime = std::ratio<1, 1'000'000>;

    using _1GB = std::ratio<1 * 1024 * 1024 * 1024ull>;
    using _2GB = std::ratio<2 * 1024 * 1024 * 1024ull>;
    using _4GB = std::ratio<4 * 1024 * 1024 * 1024ull>;

    _1080Px pix_size;
    _48000Hz sample_rate;
    _30Fps frame_rate;
};

}
