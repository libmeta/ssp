

#ifndef XLIVE_ANCHOR_UID_HPP
#define XLIVE_ANCHOR_UID_HPP

#include "common.hpp"

namespace ssp {

struct AnchorUID final {
    friend class std::hash<AnchorUID>;

private:
    int64_t time = 0;
    void *ptr = nullptr;
public:
    AnchorUID(void *ptr = nullptr) noexcept: time(Common::GetNanoSeconds().count()), ptr(ptr) {}

public:
    bool operator==(const AnchorUID &other) const {
        return time == other.time && ptr == other.ptr;
    }

    bool operator!=(const AnchorUID &other) const {
        return !this->operator==(other);
    }
};

class Anchored {
public:
    virtual const AnchorUID &GetAnchorUID() const = 0;
};

}

namespace std {

using namespace ssp;

template<>
struct hash<AnchorUID> {
    size_t operator()(const AnchorUID &uid) const {
        return std::hash<int64_t>()(reinterpret_cast<int64_t>(uid.ptr)) ^ std::hash<int64_t>()(uid.time);
    }
};

}

#endif //XLIVE_ANCHOR_UID_HPP
