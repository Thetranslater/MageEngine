#pragma once
#include "meta/reflection/reflection.h"
#include "_generated/serializer/all_serializer.h"
#include "_generated\reflection\reflection_test.reflection.gen.h"

namespace Mage{
namespace Reflection{
    void TypeMetaRegister::Register(){
    TypeWrappersRegister::BaseTest();
    TypeWrappersRegister::Test1();
    TypeWrappersRegister::Test2();
    }
}
}

