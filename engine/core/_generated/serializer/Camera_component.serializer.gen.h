#pragma once
#include "..\engine_core\scaffold\components\cameracomponent\Camera_component.h"
#include "_generated\serializer\component.serializer.gen.h"

namespace Mage{
    template<>
    PJson PSerializer::write(const CameraComponent& instance);
    template<>
    CameraComponent& PSerializer::read(const PJson& json_context, CameraComponent& instance);
}//namespace

