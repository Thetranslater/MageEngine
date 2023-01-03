#pragma once
#include "math\quaternion.h"

namespace Mage{
    template<>
    PJson PSerializer::write(const Quaternion& instance);
    template<>
    Quaternion& PSerializer::read(const PJson& json_context, Quaternion& instance);
}//namespace

