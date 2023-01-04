#pragma once
#include "..\asset\object_asset.h"

namespace Mage{
    class ObjectAsset;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeObjectAssetOperator{
    public:
        static const char* getClassName(){ return "ObjectAsset";}
        static void* constructorWithJson(const PJson& json_context){
            ObjectAsset* ret_instance= new ObjectAsset;
            PSerializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static PJson writeByName(void* instance){
            return PSerializer::write(*(ObjectAsset*)instance);
        }
        // base class
        static int getObjectAssetBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 0;
            
            return count;
        }
        // fields
        static const char* getFieldName_name(){ return "name";}
        static const char* getFieldTypeName_name(){ return "std::string";}
        static void set_name(void* instance, void* field_value){ static_cast<ObjectAsset*>(instance)->name = *static_cast<std::string*>(field_value);}
        static void* get_name(void* instance){ return static_cast<void*>(&(static_cast<ObjectAsset*>(instance)->name));}
        static bool isArray_name(){ return false; }
        static const char* getFieldName_components(){ return "components";}
        static const char* getFieldTypeName_components(){ return "std::vector<Reflection::ReflectionPtr<Component>>";}
        static void set_components(void* instance, void* field_value){ static_cast<ObjectAsset*>(instance)->components = *static_cast<std::vector<Reflection::ReflectionPtr<Component>>*>(field_value);}
        static void* get_components(void* instance){ return static_cast<void*>(&(static_cast<ObjectAsset*>(instance)->components));}
        static bool isArray_components(){ return true; }
    };
}//namespace TypeFieldReflectionOparator
namespace ArrayReflectionOperator{
#ifndef ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperatorMACRO
#define ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperatorMACRO
    class ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperator{
        public:
            static const char* getArrayTypeName(){ return "std::vector<Reflection::ReflectionPtr<Component>>";}
            static const char* getElementTypeName(){ return "Reflection::ReflectionPtr<Component>";}
            static int getSize(void* instance){
                //todo: should check validation
                return static_cast<int>(static_cast<std::vector<Reflection::ReflectionPtr<Component>>*>(instance)->size());
            }
            static void* get(int index,void* instance){
                //todo: should check validation
                return static_cast<void*>(&((*static_cast<std::vector<Reflection::ReflectionPtr<Component>>*>(instance))[index]));
            }
            static void set(int index, void* instance, void* element_value){
                //todo: should check validation
                (*static_cast<std::vector<Reflection::ReflectionPtr<Component>>*>(instance))[index] = *static_cast<Reflection::ReflectionPtr<Component>*>(element_value);
            }
    };
#endif //ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperator
}//namespace ArrayReflectionOperator

    void TypeWrapperRegister_ObjectAsset(){
        FieldFunctionTuple* f_field_function_tuple_name=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeObjectAssetOperator::set_name,
            &TypeFieldReflectionOparator::TypeObjectAssetOperator::get_name,
            &TypeFieldReflectionOparator::TypeObjectAssetOperator::getClassName,
            &TypeFieldReflectionOparator::TypeObjectAssetOperator::getFieldName_name,
            &TypeFieldReflectionOparator::TypeObjectAssetOperator::getFieldTypeName_name,
            &TypeFieldReflectionOparator::TypeObjectAssetOperator::isArray_name);
        REGISTER_FIELD_TO_MAP("ObjectAsset", f_field_function_tuple_name);
        FieldFunctionTuple* f_field_function_tuple_components=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeObjectAssetOperator::set_components,
            &TypeFieldReflectionOparator::TypeObjectAssetOperator::get_components,
            &TypeFieldReflectionOparator::TypeObjectAssetOperator::getClassName,
            &TypeFieldReflectionOparator::TypeObjectAssetOperator::getFieldName_components,
            &TypeFieldReflectionOparator::TypeObjectAssetOperator::getFieldTypeName_components,
            &TypeFieldReflectionOparator::TypeObjectAssetOperator::isArray_components);
        REGISTER_FIELD_TO_MAP("ObjectAsset", f_field_function_tuple_components);
        
        ArrayFunctionTuple* f_array_tuple_stdSSvectorLReflectionSSReflectionPtrLComponentRR = new  ArrayFunctionTuple(
            &ArrayReflectionOperator::ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperator::set,
            &ArrayReflectionOperator::ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperator::get,
            &ArrayReflectionOperator::ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperator::getSize,
            &ArrayReflectionOperator::ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperator::getArrayTypeName,
            &ArrayReflectionOperator::ArraystdSSvectorLReflectionSSReflectionPtrLComponentRROperator::getElementTypeName);
        REGISTER_ARRAY_TO_MAP("std::vector<Reflection::ReflectionPtr<Component>>", f_array_tuple_stdSSvectorLReflectionSSReflectionPtrLComponentRR);
        ClassFunctionTuple* f_class_function_tuple_ObjectAsset=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeObjectAssetOperator::getObjectAssetBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeObjectAssetOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeObjectAssetOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("ObjectAsset", f_class_function_tuple_ObjectAsset);
    }
namespace TypeWrappersRegister{
        void ObjectAsset(){ TypeWrapperRegister_ObjectAsset();}
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace Piccolo

