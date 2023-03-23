#pragma once
#include "..\engine_core\scaffold\components\lightcomponent\Light_component.h"
#include "_generated\serializer\component.serializer.gen.h"

namespace Mage{
    template<>
    PJson PSerializer::write(const LightComponent& instance);
    template<>
    LightComponent& PSerializer::read(const PJson& json_context, LightComponent& instance);
}//namespace

