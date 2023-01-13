#pragma once
#include "..\engine_core\scaffold\components\cameracomponent\Camera_component.h"

namespace Mage{
    class CameraComponent;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeCameraComponentOperator{
    public:
        static const char* getClassName(){ return "CameraComponent";}
        static void* constructorWithJson(const PJson& json_context){
            CameraComponent* ret_instance= new CameraComponent;
            PSerializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static PJson writeByName(void* instance){
            return PSerializer::write(*(CameraComponent*)instance);
        }
        // base class
        static int getCameraComponentBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 1;
            out_list = new ReflectionInstance[count];
            for (int i=0;i<count;++i){
               out_list[i] = TypeMetaDef(Mage::Component,static_cast<CameraComponent*>(instance));
            }
            return count;
        }
        // fields
        static const char* getFieldName_field_of_view(){ return "field_of_view";}
        static const char* getFieldTypeName_field_of_view(){ return "float";}
        static void set_field_of_view(void* instance, void* field_value){ static_cast<CameraComponent*>(instance)->field_of_view = *static_cast<float*>(field_value);}
        static void* get_field_of_view(void* instance){ return static_cast<void*>(&(static_cast<CameraComponent*>(instance)->field_of_view));}
        static bool isArray_field_of_view(){ return false; }
        static const char* getFieldName_near_clip_plane(){ return "near_clip_plane";}
        static const char* getFieldTypeName_near_clip_plane(){ return "float";}
        static void set_near_clip_plane(void* instance, void* field_value){ static_cast<CameraComponent*>(instance)->near_clip_plane = *static_cast<float*>(field_value);}
        static void* get_near_clip_plane(void* instance){ return static_cast<void*>(&(static_cast<CameraComponent*>(instance)->near_clip_plane));}
        static bool isArray_near_clip_plane(){ return false; }
        static const char* getFieldName_far_clip_plane(){ return "far_clip_plane";}
        static const char* getFieldTypeName_far_clip_plane(){ return "float";}
        static void set_far_clip_plane(void* instance, void* field_value){ static_cast<CameraComponent*>(instance)->far_clip_plane = *static_cast<float*>(field_value);}
        static void* get_far_clip_plane(void* instance){ return static_cast<void*>(&(static_cast<CameraComponent*>(instance)->far_clip_plane));}
        static bool isArray_far_clip_plane(){ return false; }
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_CameraComponent(){
        FieldFunctionTuple* f_field_function_tuple_field_of_view=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::set_field_of_view,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::get_field_of_view,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::getClassName,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::getFieldName_field_of_view,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::getFieldTypeName_field_of_view,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::isArray_field_of_view);
        REGISTER_FIELD_TO_MAP("CameraComponent", f_field_function_tuple_field_of_view);
        FieldFunctionTuple* f_field_function_tuple_near_clip_plane=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::set_near_clip_plane,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::get_near_clip_plane,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::getClassName,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::getFieldName_near_clip_plane,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::getFieldTypeName_near_clip_plane,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::isArray_near_clip_plane);
        REGISTER_FIELD_TO_MAP("CameraComponent", f_field_function_tuple_near_clip_plane);
        FieldFunctionTuple* f_field_function_tuple_far_clip_plane=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::set_far_clip_plane,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::get_far_clip_plane,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::getClassName,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::getFieldName_far_clip_plane,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::getFieldTypeName_far_clip_plane,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::isArray_far_clip_plane);
        REGISTER_FIELD_TO_MAP("CameraComponent", f_field_function_tuple_far_clip_plane);
        
        
        ClassFunctionTuple* f_class_function_tuple_CameraComponent=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::getCameraComponentBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeCameraComponentOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("CameraComponent", f_class_function_tuple_CameraComponent);
    }
namespace TypeWrappersRegister{
        void CameraComponent(){ TypeWrapperRegister_CameraComponent();}
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace Piccolo

