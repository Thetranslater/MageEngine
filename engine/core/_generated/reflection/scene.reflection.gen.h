#pragma once
#include "..\asset\scene.h"

namespace Mage{
    class SceneAsset;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeSceneAssetOperator{
    public:
        static const char* getClassName(){ return "SceneAsset";}
        static void* constructorWithJson(const PJson& json_context){
            SceneAsset* ret_instance= new SceneAsset;
            PSerializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static PJson writeByName(void* instance){
            return PSerializer::write(*(SceneAsset*)instance);
        }
        // base class
        static int getSceneAssetBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 0;
            
            return count;
        }
        // fields
        static const char* getFieldName_name(){ return "name";}
        static const char* getFieldTypeName_name(){ return "std::string";}
        static void set_name(void* instance, void* field_value){ static_cast<SceneAsset*>(instance)->name = *static_cast<std::string*>(field_value);}
        static void* get_name(void* instance){ return static_cast<void*>(&(static_cast<SceneAsset*>(instance)->name));}
        static bool isArray_name(){ return false; }
        static const char* getFieldName_objects(){ return "objects";}
        static const char* getFieldTypeName_objects(){ return "std::vector<ObjectAsset>";}
        static void set_objects(void* instance, void* field_value){ static_cast<SceneAsset*>(instance)->objects = *static_cast<std::vector<ObjectAsset>*>(field_value);}
        static void* get_objects(void* instance){ return static_cast<void*>(&(static_cast<SceneAsset*>(instance)->objects));}
        static bool isArray_objects(){ return true; }
    };
}//namespace TypeFieldReflectionOparator
namespace ArrayReflectionOperator{
#ifndef ArraystdSSvectorLObjectAssetROperatorMACRO
#define ArraystdSSvectorLObjectAssetROperatorMACRO
    class ArraystdSSvectorLObjectAssetROperator{
        public:
            static const char* getArrayTypeName(){ return "std::vector<ObjectAsset>";}
            static const char* getElementTypeName(){ return "ObjectAsset";}
            static int getSize(void* instance){
                //todo: should check validation
                return static_cast<int>(static_cast<std::vector<ObjectAsset>*>(instance)->size());
            }
            static void* get(int index,void* instance){
                //todo: should check validation
                return static_cast<void*>(&((*static_cast<std::vector<ObjectAsset>*>(instance))[index]));
            }
            static void set(int index, void* instance, void* element_value){
                //todo: should check validation
                (*static_cast<std::vector<ObjectAsset>*>(instance))[index] = *static_cast<ObjectAsset*>(element_value);
            }
    };
#endif //ArraystdSSvectorLObjectAssetROperator
}//namespace ArrayReflectionOperator

    void TypeWrapperRegister_SceneAsset(){
        FieldFunctionTuple* f_field_function_tuple_name=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::set_name,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::get_name,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::getClassName,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::getFieldName_name,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::getFieldTypeName_name,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::isArray_name);
        REGISTER_FIELD_TO_MAP("SceneAsset", f_field_function_tuple_name);
        FieldFunctionTuple* f_field_function_tuple_objects=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::set_objects,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::get_objects,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::getClassName,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::getFieldName_objects,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::getFieldTypeName_objects,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::isArray_objects);
        REGISTER_FIELD_TO_MAP("SceneAsset", f_field_function_tuple_objects);
        
        ArrayFunctionTuple* f_array_tuple_stdSSvectorLObjectAssetR = new  ArrayFunctionTuple(
            &ArrayReflectionOperator::ArraystdSSvectorLObjectAssetROperator::set,
            &ArrayReflectionOperator::ArraystdSSvectorLObjectAssetROperator::get,
            &ArrayReflectionOperator::ArraystdSSvectorLObjectAssetROperator::getSize,
            &ArrayReflectionOperator::ArraystdSSvectorLObjectAssetROperator::getArrayTypeName,
            &ArrayReflectionOperator::ArraystdSSvectorLObjectAssetROperator::getElementTypeName);
        REGISTER_ARRAY_TO_MAP("std::vector<ObjectAsset>", f_array_tuple_stdSSvectorLObjectAssetR);
        ClassFunctionTuple* f_class_function_tuple_SceneAsset=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::getSceneAssetBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("SceneAsset", f_class_function_tuple_SceneAsset);
    }
namespace TypeWrappersRegister{
        void SceneAsset(){ TypeWrapperRegister_SceneAsset();}
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace Piccolo

