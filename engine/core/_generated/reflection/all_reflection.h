#pragma once
#include "meta/reflection/reflection.h"
#include "_generated/serializer/all_serializer.h"
#include "_generated\reflection\vector3.reflection.gen.h"
#include "_generated\reflection\object.reflection.gen.h"
#include "_generated\reflection\scene.reflection.gen.h"
#include "_generated\reflection\world.reflection.gen.h"
#include "_generated\reflection\quaternion.reflection.gen.h"
#include "_generated\reflection\vector2.reflection.gen.h"

namespace Mage{
namespace Reflection{
    void TypeMetaRegister::Register(){
    TypeWrappersRegister::Vector3();
    TypeWrappersRegister::ObjectAsset();
    TypeWrappersRegister::SceneAsset();
    TypeWrappersRegister::WorldAsset();
    TypeWrappersRegister::Quaternion();
    TypeWrappersRegister::Vector2();
    }
}
}

