#pragma once

#include <gd.h>

template <typename T, typename R>
T as(R const v) { return reinterpret_cast<T>(v); }

namespace gdmake {
    namespace extra {
        
    }
}