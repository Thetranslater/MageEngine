#pragma once
#include "..\asset\object_asset.h"

namespace Mage{
    template<>
    PJson PSerializer::write(const ObjectAsset& instance);
    template<>
    ObjectAsset& PSerializer::read(const PJson& json_context, ObjectAsset& instance);
}//namespace

