#pragma once
#include "..\engine_core\scaffold\components\transformcomponent\Transform_component.h"
#include "_generated\serializer\component.serializer.gen.h"

namespace Mage{
    template<>
    PJson PSerializer::write(const TransformComponent& instance);
    template<>
    TransformComponent& PSerializer::read(const PJson& json_context, TransformComponent& instance);
}//namespace

