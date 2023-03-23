#pragma once
#include "..\engine_core\scaffold\components\lightcomponent\Light_component.h"

namespace Mage{
    class LightComponent;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeLightComponentOperator{
    public:
        static const char* getClassName(){ return "LightComponent";}
        static void* constructorWithJson(const PJson& json_context){
            LightComponent* ret_instance= new LightComponent;
            PSerializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static PJson writeByName(void* instance){
            return PSerializer::write(*(LightComponent*)instance);
        }
        // base class
        static int getLightComponentBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 1;
            out_list = new ReflectionInstance[count];
            for (int i=0;i<count;++i){
               out_list[i] = TypeMetaDef(Mage::Component,static_cast<LightComponent*>(instance));
            }
            return count;
        }
        // fields
        static const char* getFieldName_type(){ return "type";}
        static const char* getFieldTypeName_type(){ return "std::string";}
        static void set_type(void* instance, void* field_value){ static_cast<LightComponent*>(instance)->type = *static_cast<std::string*>(field_value);}
        static void* get_type(void* instance){ return static_cast<void*>(&(static_cast<LightComponent*>(instance)->type));}
        static bool isArray_type(){ return false; }
        static const char* getFieldName_color(){ return "color";}
        static const char* getFieldTypeName_color(){ return "Vector3";}
        static void set_color(void* instance, void* field_value){ static_cast<LightComponent*>(instance)->color = *static_cast<Vector3*>(field_value);}
        static void* get_color(void* instance){ return static_cast<void*>(&(static_cast<LightComponent*>(instance)->color));}
        static bool isArray_color(){ return false; }
        static const char* getFieldName_intensity(){ return "intensity";}
        static const char* getFieldTypeName_intensity(){ return "float";}
        static void set_intensity(void* instance, void* field_value){ static_cast<LightComponent*>(instance)->intensity = *static_cast<float*>(field_value);}
        static void* get_intensity(void* instance){ return static_cast<void*>(&(static_cast<LightComponent*>(instance)->intensity));}
        static bool isArray_intensity(){ return false; }
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_LightComponent(){
        FieldFunctionTuple* f_field_function_tuple_type=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeLightComponentOperator::set_type,
            &TypeFieldReflectionOparator::TypeLightComponentOperator::get_type,
            &TypeFieldReflectionOparator::TypeLightComponentOperator::getClassName,
            &TypeFieldReflectionOparator::TypeLightComponentOperator::getFieldName_type,
            &TypeFieldReflectionOparator::TypeLightComponentOperator::getFieldTypeName_type,
            &TypeFieldReflectionOparator::TypeLightComponentOperator::isArray_type);
        REGISTER_FIELD_TO_MAP("LightComponent", f_field_function_tuple_type);
        FieldFunctionTuple* f_field_function_tuple_color=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeLightComponentOperator::set_color,
            &TypeFieldReflectionOparator::TypeLightComponentOperator::get_color,
            &TypeFieldReflectionOparator::TypeLightComponentOperator::getClassName,
            &TypeFieldReflectionOparator::TypeLightComponentOperator::getFieldName_color,
            &TypeFieldReflectionOparator::TypeLightComponentOperator::getFieldTypeName_color,
            &TypeFieldReflectionOparator::TypeLightComponentOperator::isArray_color);
        REGISTER_FIELD_TO_MAP("LightComponent", f_field_function_tuple_color);
        FieldFunctionTuple* f_field_function_tuple_intensity=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeLightComponentOperator::set_intensity,
            &TypeFieldReflectionOparator::TypeLightComponentOperator::get_intensity,
            &TypeFieldReflectionOparator::TypeLightComponentOperator::getClassName,
            &TypeFieldReflectionOparator::TypeLightComponentOperator::getFieldName_intensity,
            &TypeFieldReflectionOparator::TypeLightComponentOperator::getFieldTypeName_intensity,
            &TypeFieldReflectionOparator::TypeLightComponentOperator::isArray_intensity);
        REGISTER_FIELD_TO_MAP("LightComponent", f_field_function_tuple_intensity);
        
        
        ClassFunctionTuple* f_class_function_tuple_LightComponent=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeLightComponentOperator::getLightComponentBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeLightComponentOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeLightComponentOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("LightComponent", f_class_function_tuple_LightComponent);
    }
namespace TypeWrappersRegister{
        void LightComponent(){ TypeWrapperRegister_LightComponent();}
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace Piccolo

