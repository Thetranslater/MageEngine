#pragma once
#include "..\engine_core\scaffold\components\meshcomponent\mesh_asset.h"

namespace Mage{
    template<>
    PJson PSerializer::write(const MeshAsset& instance);
    template<>
    MeshAsset& PSerializer::read(const PJson& json_context, MeshAsset& instance);
}//namespace

