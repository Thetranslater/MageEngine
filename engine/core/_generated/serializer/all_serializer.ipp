#pragma once
#include "_generated\serializer\reflection_test.serializer.gen.h"
#include "_generated\serializer\vector3.serializer.gen.h"
namespace Mage{
    template<>
    PJson PSerializer::write(const BaseTest& instance){
        PJson::object  ret_context;
        
        ret_context.insert_or_assign("int", PSerializer::write(instance.m_int));
        PJson::array m_int_vector_json;
        for (auto& item : instance.m_int_vector){
            m_int_vector_json.emplace_back(PSerializer::write(item));
        }
        ret_context.insert_or_assign("int_vector",m_int_vector_json);
        
        return  PJson(ret_context);
    }
    template<>
    BaseTest& PSerializer::read(const PJson& json_context, BaseTest& instance){
        assert(json_context.is_object());
        
        if(!json_context["int"].is_null()){
            PSerializer::read(json_context["int"], instance.m_int);
        }
        if(!json_context["int_vector"].is_null()){
            assert(json_context["int_vector"].is_array());
            PJson::array array_m_int_vector = json_context["int_vector"].array_items();
            instance.m_int_vector.resize(array_m_int_vector.size());
            for (size_t index=0; index < array_m_int_vector.size();++index){
                PSerializer::read(array_m_int_vector[index], instance.m_int_vector[index]);
            }
        }
        return instance;
    }
    template<>
    PJson PSerializer::write(const Test1& instance){
        PJson::object  ret_context;
        auto&&  json_context_0 = PSerializer::write(*(Mage::BaseTest*)&instance);
        assert(json_context_0.is_object());
        auto&& json_context_map_0 = json_context_0.object_items();
        ret_context.insert(json_context_map_0.begin() , json_context_map_0.end());
        ret_context.insert_or_assign("char", PSerializer::write(instance.m_char));
        return  PJson(ret_context);
    }
    template<>
    Test1& PSerializer::read(const PJson& json_context, Test1& instance){
        assert(json_context.is_object());
        PSerializer::read(json_context,*(Mage::BaseTest*)&instance);
        if(!json_context["char"].is_null()){
            PSerializer::read(json_context["char"], instance.m_char);
        }
        return instance;
    }
    template<>
    PJson PSerializer::write(const Test2& instance){
        PJson::object  ret_context;
        auto&&  json_context_0 = PSerializer::write(*(Mage::BaseTest*)&instance);
        assert(json_context_0.is_object());
        auto&& json_context_map_0 = json_context_0.object_items();
        ret_context.insert(json_context_map_0.begin() , json_context_map_0.end());
        PJson::array m_test_base_array_json;
        for (auto& item : instance.m_test_base_array){
            m_test_base_array_json.emplace_back(PSerializer::write(item));
        }
        ret_context.insert_or_assign("test_base_array",m_test_base_array_json);
        
        return  PJson(ret_context);
    }
    template<>
    Test2& PSerializer::read(const PJson& json_context, Test2& instance){
        assert(json_context.is_object());
        PSerializer::read(json_context,*(Mage::BaseTest*)&instance);
        if(!json_context["test_base_array"].is_null()){
            assert(json_context["test_base_array"].is_array());
            PJson::array array_m_test_base_array = json_context["test_base_array"].array_items();
            instance.m_test_base_array.resize(array_m_test_base_array.size());
            for (size_t index=0; index < array_m_test_base_array.size();++index){
                PSerializer::read(array_m_test_base_array[index], instance.m_test_base_array[index]);
            }
        }
        return instance;
    }
    template<>
    PJson PSerializer::write(const Vector3& instance){
        PJson::object  ret_context;
        
        ret_context.insert_or_assign("x", PSerializer::write(instance.x));
        ret_context.insert_or_assign("y", PSerializer::write(instance.y));
        ret_context.insert_or_assign("z", PSerializer::write(instance.z));
        return  PJson(ret_context);
    }
    template<>
    Vector3& PSerializer::read(const PJson& json_context, Vector3& instance){
        assert(json_context.is_object());
        
        if(!json_context["x"].is_null()){
            PSerializer::read(json_context["x"], instance.x);
        }
        if(!json_context["y"].is_null()){
            PSerializer::read(json_context["y"], instance.y);
        }
        if(!json_context["z"].is_null()){
            PSerializer::read(json_context["z"], instance.z);
        }
        return instance;
    }

}

