#pragma once
#include "meta/reflection/reflection.h"
#include "_generated/serializer/all_serializer.h"
#include "_generated\reflection\vector3.reflection.gen.h"
#include "_generated\reflection\Transform_component.reflection.gen.h"
#include "_generated\reflection\object_asset.reflection.gen.h"
#include "_generated\reflection\scene_asset.reflection.gen.h"
#include "_generated\reflection\mesh_asset.reflection.gen.h"
#include "_generated\reflection\component.reflection.gen.h"
#include "_generated\reflection\world_asset.reflection.gen.h"
#include "_generated\reflection\quaternion.reflection.gen.h"
#include "_generated\reflection\vector2.reflection.gen.h"
#include "_generated\reflection\Mesh_component.reflection.gen.h"

namespace Mage{
namespace Reflection{
    void TypeMetaRegister::Register(){
    TypeWrappersRegister::Vector3();
    TypeWrappersRegister::TransformComponent();
    TypeWrappersRegister::ObjectAsset();
    TypeWrappersRegister::SceneAsset();
    TypeWrappersRegister::MeshAsset();
    TypeWrappersRegister::Component();
    TypeWrappersRegister::WorldAsset();
    TypeWrappersRegister::Quaternion();
    TypeWrappersRegister::Vector2();
    TypeWrappersRegister::MeshComponent();
    }
}
}

