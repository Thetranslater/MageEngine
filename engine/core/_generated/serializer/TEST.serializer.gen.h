#pragma once
#include "..\..\editor\TEST.h"

namespace Mage{
    template<>
    PJson PSerializer::write(const World& instance);
    template<>
    World& PSerializer::read(const PJson& json_context, World& instance);
}//namespace

