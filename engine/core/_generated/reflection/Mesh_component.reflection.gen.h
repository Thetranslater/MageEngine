#pragma once
#include "..\engine_core\scaffold\components\meshcomponent\Mesh_component.h"

namespace Mage{
    class MeshComponent;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeMeshComponentOperator{
    public:
        static const char* getClassName(){ return "MeshComponent";}
        static void* constructorWithJson(const PJson& json_context){
            MeshComponent* ret_instance= new MeshComponent;
            PSerializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static PJson writeByName(void* instance){
            return PSerializer::write(*(MeshComponent*)instance);
        }
        // base class
        static int getMeshComponentBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 1;
            out_list = new ReflectionInstance[count];
            for (int i=0;i<count;++i){
               out_list[i] = TypeMetaDef(Mage::Component,static_cast<MeshComponent*>(instance));
            }
            return count;
        }
        // fields
        static const char* getFieldName_mesh(){ return "mesh";}
        static const char* getFieldTypeName_mesh(){ return "Mage::MeshAsset";}
        static void set_mesh(void* instance, void* field_value){ static_cast<MeshComponent*>(instance)->mesh = *static_cast<Mage::MeshAsset*>(field_value);}
        static void* get_mesh(void* instance){ return static_cast<void*>(&(static_cast<MeshComponent*>(instance)->mesh));}
        static bool isArray_mesh(){ return false; }
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_MeshComponent(){
        FieldFunctionTuple* f_field_function_tuple_mesh=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeMeshComponentOperator::set_mesh,
            &TypeFieldReflectionOparator::TypeMeshComponentOperator::get_mesh,
            &TypeFieldReflectionOparator::TypeMeshComponentOperator::getClassName,
            &TypeFieldReflectionOparator::TypeMeshComponentOperator::getFieldName_mesh,
            &TypeFieldReflectionOparator::TypeMeshComponentOperator::getFieldTypeName_mesh,
            &TypeFieldReflectionOparator::TypeMeshComponentOperator::isArray_mesh);
        REGISTER_FIELD_TO_MAP("MeshComponent", f_field_function_tuple_mesh);
        
        
        ClassFunctionTuple* f_class_function_tuple_MeshComponent=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeMeshComponentOperator::getMeshComponentBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeMeshComponentOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeMeshComponentOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("MeshComponent", f_class_function_tuple_MeshComponent);
    }
namespace TypeWrappersRegister{
        void MeshComponent(){ TypeWrapperRegister_MeshComponent();}
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace Piccolo

