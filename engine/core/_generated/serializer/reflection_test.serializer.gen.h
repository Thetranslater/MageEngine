#pragma once
#include "meta\reflection_test.h"

namespace Mage{
    template<>
    PJson PSerializer::write(const BaseTest& instance);
    template<>
    BaseTest& PSerializer::read(const PJson& json_context, BaseTest& instance);
    template<>
    PJson PSerializer::write(const Test1& instance);
    template<>
    Test1& PSerializer::read(const PJson& json_context, Test1& instance);
    template<>
    PJson PSerializer::write(const Test2& instance);
    template<>
    Test2& PSerializer::read(const PJson& json_context, Test2& instance);
}//namespace

