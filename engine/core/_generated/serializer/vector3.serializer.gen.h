#pragma once
#include "math\vector3.h"

namespace Mage{
    template<>
    PJson PSerializer::write(const Vector3& instance);
    template<>
    Vector3& PSerializer::read(const PJson& json_context, Vector3& instance);
}//namespace

