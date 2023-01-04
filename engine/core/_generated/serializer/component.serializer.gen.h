#pragma once
#include "..\engine_core\scaffold\components\component.h"

namespace Mage{
    template<>
    PJson PSerializer::write(const Component& instance);
    template<>
    Component& PSerializer::read(const PJson& json_context, Component& instance);
}//namespace

