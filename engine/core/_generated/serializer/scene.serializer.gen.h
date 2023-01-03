#pragma once
#include "..\asset\scene.h"

namespace Mage{
    template<>
    PJson PSerializer::write(const SceneAsset& instance);
    template<>
    SceneAsset& PSerializer::read(const PJson& json_context, SceneAsset& instance);
}//namespace

