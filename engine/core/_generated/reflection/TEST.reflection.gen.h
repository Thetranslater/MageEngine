#pragma once
#include "..\..\editor\TEST.h"

namespace Mage{
    class World;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeWorldOperator{
    public:
        static const char* getClassName(){ return "World";}
        static void* constructorWithJson(const PJson& json_context){
            World* ret_instance= new World;
            PSerializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static PJson writeByName(void* instance){
            return PSerializer::write(*(World*)instance);
        }
        // base class
        static int getWorldBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 0;
            
            return count;
        }
        // fields
        static const char* getFieldName_name(){ return "name";}
        static const char* getFieldTypeName_name(){ return "std::string";}
        static void set_name(void* instance, void* field_value){ static_cast<World*>(instance)->name = *static_cast<std::string*>(field_value);}
        static void* get_name(void* instance){ return static_cast<void*>(&(static_cast<World*>(instance)->name));}
        static bool isArray_name(){ return false; }
        static const char* getFieldName_urls(){ return "urls";}
        static const char* getFieldTypeName_urls(){ return "std::vector<std::string>";}
        static void set_urls(void* instance, void* field_value){ static_cast<World*>(instance)->urls = *static_cast<std::vector<std::string>*>(field_value);}
        static void* get_urls(void* instance){ return static_cast<void*>(&(static_cast<World*>(instance)->urls));}
        static bool isArray_urls(){ return true; }
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

    void TypeWrapperRegister_World(){
        FieldFunctionTuple* f_field_function_tuple_name=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeWorldOperator::set_name,
            &TypeFieldReflectionOparator::TypeWorldOperator::get_name,
            &TypeFieldReflectionOparator::TypeWorldOperator::getClassName,
            &TypeFieldReflectionOparator::TypeWorldOperator::getFieldName_name,
            &TypeFieldReflectionOparator::TypeWorldOperator::getFieldTypeName_name,
            &TypeFieldReflectionOparator::TypeWorldOperator::isArray_name);
        REGISTER_FIELD_TO_MAP("World", f_field_function_tuple_name);
        FieldFunctionTuple* f_field_function_tuple_urls=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeWorldOperator::set_urls,
            &TypeFieldReflectionOparator::TypeWorldOperator::get_urls,
            &TypeFieldReflectionOparator::TypeWorldOperator::getClassName,
            &TypeFieldReflectionOparator::TypeWorldOperator::getFieldName_urls,
            &TypeFieldReflectionOparator::TypeWorldOperator::getFieldTypeName_urls,
            &TypeFieldReflectionOparator::TypeWorldOperator::isArray_urls);
        REGISTER_FIELD_TO_MAP("World", f_field_function_tuple_urls);
        
        ArrayFunctionTuple* f_array_tuple_stdSSvectorLstdSSstringR = new  ArrayFunctionTuple(
            &ArrayReflectionOperator::ArraystdSSvectorLstdSSstringROperator::set,
            &ArrayReflectionOperator::ArraystdSSvectorLstdSSstringROperator::get,
            &ArrayReflectionOperator::ArraystdSSvectorLstdSSstringROperator::getSize,
            &ArrayReflectionOperator::ArraystdSSvectorLstdSSstringROperator::getArrayTypeName,
            &ArrayReflectionOperator::ArraystdSSvectorLstdSSstringROperator::getElementTypeName);
        REGISTER_ARRAY_TO_MAP("std::vector<std::string>", f_array_tuple_stdSSvectorLstdSSstringR);
        ClassFunctionTuple* f_class_function_tuple_World=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeWorldOperator::getWorldBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeWorldOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeWorldOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("World", f_class_function_tuple_World);
    }
namespace TypeWrappersRegister{
        void World(){ TypeWrapperRegister_World();}
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace Piccolo

