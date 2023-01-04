#pragma once
#include "..\engine_core\scaffold\components\meshcomponent\Mesh_component.h"
#include "_generated\serializer\component.serializer.gen.h"

namespace Mage{
    template<>
    PJson PSerializer::write(const MeshComponent& instance);
    template<>
    MeshComponent& PSerializer::read(const PJson& json_context, MeshComponent& instance);
}//namespace

