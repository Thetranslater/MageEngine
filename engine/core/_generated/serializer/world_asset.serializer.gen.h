#pragma once
#include "..\asset\world_asset.h"

namespace Mage{
    template<>
    PJson PSerializer::write(const WorldAsset& instance);
    template<>
    WorldAsset& PSerializer::read(const PJson& json_context, WorldAsset& instance);
}//namespace

