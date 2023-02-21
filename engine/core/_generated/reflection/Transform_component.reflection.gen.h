#pragma once
#include "..\engine_core\scaffold\components\transformcomponent\Transform_component.h"

namespace Mage{
    class TransformComponent;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeTransformComponentOperator{
    public:
        static const char* getClassName(){ return "TransformComponent";}
        static void* constructorWithJson(const PJson& json_context){
            TransformComponent* ret_instance= new TransformComponent;
            PSerializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static PJson writeByName(void* instance){
            return PSerializer::write(*(TransformComponent*)instance);
        }
        // base class
        static int getTransformComponentBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 1;
            out_list = new ReflectionInstance[count];
            for (int i=0;i<count;++i){
               out_list[i] = TypeMetaDef(Mage::Component,static_cast<TransformComponent*>(instance));
            }
            return count;
        }
        // fields
        static const char* getFieldName_position(){ return "position";}
        static const char* getFieldTypeName_position(){ return "Vector3";}
        static void set_position(void* instance, void* field_value){ static_cast<TransformComponent*>(instance)->position = *static_cast<Vector3*>(field_value);}
        static void* get_position(void* instance){ return static_cast<void*>(&(static_cast<TransformComponent*>(instance)->position));}
        static bool isArray_position(){ return false; }
        static const char* getFieldName_rotation(){ return "rotation";}
        static const char* getFieldTypeName_rotation(){ return "Quaternion";}
        static void set_rotation(void* instance, void* field_value){ static_cast<TransformComponent*>(instance)->rotation = *static_cast<Quaternion*>(field_value);}
        static void* get_rotation(void* instance){ return static_cast<void*>(&(static_cast<TransformComponent*>(instance)->rotation));}
        static bool isArray_rotation(){ return false; }
        static const char* getFieldName_scale(){ return "scale";}
        static const char* getFieldTypeName_scale(){ return "Vector3";}
        static void set_scale(void* instance, void* field_value){ static_cast<TransformComponent*>(instance)->scale = *static_cast<Vector3*>(field_value);}
        static void* get_scale(void* instance){ return static_cast<void*>(&(static_cast<TransformComponent*>(instance)->scale));}
        static bool isArray_scale(){ return false; }
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_TransformComponent(){
        FieldFunctionTuple* f_field_function_tuple_position=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::set_position,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::get_position,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::getClassName,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::getFieldName_position,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::getFieldTypeName_position,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::isArray_position);
        REGISTER_FIELD_TO_MAP("TransformComponent", f_field_function_tuple_position);
        FieldFunctionTuple* f_field_function_tuple_rotation=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::set_rotation,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::get_rotation,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::getClassName,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::getFieldName_rotation,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::getFieldTypeName_rotation,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::isArray_rotation);
        REGISTER_FIELD_TO_MAP("TransformComponent", f_field_function_tuple_rotation);
        FieldFunctionTuple* f_field_function_tuple_scale=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::set_scale,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::get_scale,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::getClassName,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::getFieldName_scale,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::getFieldTypeName_scale,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::isArray_scale);
        REGISTER_FIELD_TO_MAP("TransformComponent", f_field_function_tuple_scale);
        
        
        ClassFunctionTuple* f_class_function_tuple_TransformComponent=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::getTransformComponentBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeTransformComponentOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("TransformComponent", f_class_function_tuple_TransformComponent);
    }
namespace TypeWrappersRegister{
        void TransformComponent(){ TypeWrapperRegister_TransformComponent();}
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace Piccolo

