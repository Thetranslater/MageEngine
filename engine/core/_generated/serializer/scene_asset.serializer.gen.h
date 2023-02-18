#pragma once
#include "..\asset\scene_asset.h"

namespace Mage{
    template<>
    PJson PSerializer::write(const SceneNode& instance);
    template<>
    SceneNode& PSerializer::read(const PJson& json_context, SceneNode& instance);
    template<>
    PJson PSerializer::write(const SceneAsset& instance);
    template<>
    SceneAsset& PSerializer::read(const PJson& json_context, SceneAsset& instance);
}//namespace

