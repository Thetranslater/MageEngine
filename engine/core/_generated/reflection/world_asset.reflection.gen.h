#pragma once
#include "..\asset\world_asset.h"

namespace Mage{
    class WorldAsset;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeWorldAssetOperator{
    public:
        static const char* getClassName(){ return "WorldAsset";}
        static void* constructorWithJson(const PJson& json_context){
            WorldAsset* ret_instance= new WorldAsset;
            PSerializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static PJson writeByName(void* instance){
            return PSerializer::write(*(WorldAsset*)instance);
        }
        // base class
        static int getWorldAssetBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 0;
            
            return count;
        }
        // fields
        static const char* getFieldName_name(){ return "name";}
        static const char* getFieldTypeName_name(){ return "std::string";}
        static void set_name(void* instance, void* field_value){ static_cast<WorldAsset*>(instance)->name = *static_cast<std::string*>(field_value);}
        static void* get_name(void* instance){ return static_cast<void*>(&(static_cast<WorldAsset*>(instance)->name));}
        static bool isArray_name(){ return false; }
        static const char* getFieldName_default_scene(){ return "default_scene";}
        static const char* getFieldTypeName_default_scene(){ return "int";}
        static void set_default_scene(void* instance, void* field_value){ static_cast<WorldAsset*>(instance)->default_scene = *static_cast<int*>(field_value);}
        static void* get_default_scene(void* instance){ return static_cast<void*>(&(static_cast<WorldAsset*>(instance)->default_scene));}
        static bool isArray_default_scene(){ return false; }
        static const char* getFieldName_scene_urls(){ return "scene_urls";}
        static const char* getFieldTypeName_scene_urls(){ return "std::vector<std::string>";}
        static void set_scene_urls(void* instance, void* field_value){ static_cast<WorldAsset*>(instance)->scene_urls = *static_cast<std::vector<std::string>*>(field_value);}
        static void* get_scene_urls(void* instance){ return static_cast<void*>(&(static_cast<WorldAsset*>(instance)->scene_urls));}
        static bool isArray_scene_urls(){ return true; }
    };
}//namespace TypeFieldReflectionOparator
namespace ArrayReflectionOperator{
#ifndef ArraystdSSvectorLstdSSstringROperatorMACRO
#define ArraystdSSvectorLstdSSstringROperatorMACRO
    class ArraystdSSvectorLstdSSstringROperator{
        public:
            static const char* getArrayTypeName(){ return "std::vector<std::string>";}
            static const char* getElementTypeName(){ return "std::string";}
            static int getSize(void* instance){
                //todo: should check validation
                return static_cast<int>(static_cast<std::vector<std::string>*>(instance)->size());
            }
            static void* get(int index,void* instance){
                //todo: should check validation
                return static_cast<void*>(&((*static_cast<std::vector<std::string>*>(instance))[index]));
            }
            static void set(int index, void* instance, void* element_value){
                //todo: should check validation
                (*static_cast<std::vector<std::string>*>(instance))[index] = *static_cast<std::string*>(element_value);
            }
    };
#endif //ArraystdSSvectorLstdSSstringROperator
}//namespace ArrayReflectionOperator

    void TypeWrapperRegister_WorldAsset(){
        FieldFunctionTuple* f_field_function_tuple_name=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeWorldAssetOperator::set_name,
            &TypeFieldReflectionOparator::TypeWorldAssetOperator::get_name,
            &TypeFieldReflectionOparator::TypeWorldAssetOperator::getClassName,
            &TypeFieldReflectionOparator::TypeWorldAssetOperator::getFieldName_name,
            &TypeFieldReflectionOparator::TypeWorldAssetOperator::getFieldTypeName_name,
            &TypeFieldReflectionOparator::TypeWorldAssetOperator::isArray_name);
        REGISTER_FIELD_TO_MAP("WorldAsset", f_field_function_tuple_name);
        FieldFunctionTuple* f_field_function_tuple_default_scene=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeWorldAssetOperator::set_default_scene,
            &TypeFieldReflectionOparator::TypeWorldAssetOperator::get_default_scene,
            &TypeFieldReflectionOparator::TypeWorldAssetOperator::getClassName,
            &TypeFieldReflectionOparator::TypeWorldAssetOperator::getFieldName_default_scene,
            &TypeFieldReflectionOparator::TypeWorldAssetOperator::getFieldTypeName_default_scene,
            &TypeFieldReflectionOparator::TypeWorldAssetOperator::isArray_default_scene);
        REGISTER_FIELD_TO_MAP("WorldAsset", f_field_function_tuple_default_scene);
        FieldFunctionTuple* f_field_function_tuple_scene_urls=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeWorldAssetOperator::set_scene_urls,
            &TypeFieldReflectionOparator::TypeWorldAssetOperator::get_scene_urls,
            &TypeFieldReflectionOparator::TypeWorldAssetOperator::getClassName,
            &TypeFieldReflectionOparator::TypeWorldAssetOperator::getFieldName_scene_urls,
            &TypeFieldReflectionOparator::TypeWorldAssetOperator::getFieldTypeName_scene_urls,
            &TypeFieldReflectionOparator::TypeWorldAssetOperator::isArray_scene_urls);
        REGISTER_FIELD_TO_MAP("WorldAsset", f_field_function_tuple_scene_urls);
        
        ArrayFunctionTuple* f_array_tuple_stdSSvectorLstdSSstringR = new  ArrayFunctionTuple(
            &ArrayReflectionOperator::ArraystdSSvectorLstdSSstringROperator::set,
            &ArrayReflectionOperator::ArraystdSSvectorLstdSSstringROperator::get,
            &ArrayReflectionOperator::ArraystdSSvectorLstdSSstringROperator::getSize,
            &ArrayReflectionOperator::ArraystdSSvectorLstdSSstringROperator::getArrayTypeName,
            &ArrayReflectionOperator::ArraystdSSvectorLstdSSstringROperator::getElementTypeName);
        REGISTER_ARRAY_TO_MAP("std::vector<std::string>", f_array_tuple_stdSSvectorLstdSSstringR);
        ClassFunctionTuple* f_class_function_tuple_WorldAsset=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeWorldAssetOperator::getWorldAssetBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeWorldAssetOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeWorldAssetOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("WorldAsset", f_class_function_tuple_WorldAsset);
    }
namespace TypeWrappersRegister{
        void WorldAsset(){ TypeWrapperRegister_WorldAsset();}
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace Piccolo

