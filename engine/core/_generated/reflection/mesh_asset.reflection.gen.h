#pragma once
#include "..\engine_core\scaffold\components\meshcomponent\mesh_asset.h"

namespace Mage{
    class MeshAsset;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeMeshAssetOperator{
    public:
        static const char* getClassName(){ return "MeshAsset";}
        static void* constructorWithJson(const PJson& json_context){
            MeshAsset* ret_instance= new MeshAsset;
            PSerializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static PJson writeByName(void* instance){
            return PSerializer::write(*(MeshAsset*)instance);
        }
        // base class
        static int getMeshAssetBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 0;
            
            return count;
        }
        // fields
        static const char* getFieldName_gltf_model_url(){ return "gltf_model_url";}
        static const char* getFieldTypeName_gltf_model_url(){ return "std::string";}
        static void set_gltf_model_url(void* instance, void* field_value){ static_cast<MeshAsset*>(instance)->gltf_model_url = *static_cast<std::string*>(field_value);}
        static void* get_gltf_model_url(void* instance){ return static_cast<void*>(&(static_cast<MeshAsset*>(instance)->gltf_model_url));}
        static bool isArray_gltf_model_url(){ return false; }
        static const char* getFieldName_albedo(){ return "albedo";}
        static const char* getFieldTypeName_albedo(){ return "std::string";}
        static void set_albedo(void* instance, void* field_value){ static_cast<MeshAsset*>(instance)->albedo = *static_cast<std::string*>(field_value);}
        static void* get_albedo(void* instance){ return static_cast<void*>(&(static_cast<MeshAsset*>(instance)->albedo));}
        static bool isArray_albedo(){ return false; }
        static const char* getFieldName_normal_map(){ return "normal_map";}
        static const char* getFieldTypeName_normal_map(){ return "std::string";}
        static void set_normal_map(void* instance, void* field_value){ static_cast<MeshAsset*>(instance)->normal_map = *static_cast<std::string*>(field_value);}
        static void* get_normal_map(void* instance){ return static_cast<void*>(&(static_cast<MeshAsset*>(instance)->normal_map));}
        static bool isArray_normal_map(){ return false; }
        static const char* getFieldName_metallic_roughness(){ return "metallic_roughness";}
        static const char* getFieldTypeName_metallic_roughness(){ return "std::string";}
        static void set_metallic_roughness(void* instance, void* field_value){ static_cast<MeshAsset*>(instance)->metallic_roughness = *static_cast<std::string*>(field_value);}
        static void* get_metallic_roughness(void* instance){ return static_cast<void*>(&(static_cast<MeshAsset*>(instance)->metallic_roughness));}
        static bool isArray_metallic_roughness(){ return false; }
    };
}//namespace TypeFieldReflectionOparator


    void TypeWrapperRegister_MeshAsset(){
        FieldFunctionTuple* f_field_function_tuple_gltf_model_url=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::set_gltf_model_url,
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::get_gltf_model_url,
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::getClassName,
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::getFieldName_gltf_model_url,
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::getFieldTypeName_gltf_model_url,
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::isArray_gltf_model_url);
        REGISTER_FIELD_TO_MAP("MeshAsset", f_field_function_tuple_gltf_model_url);
        FieldFunctionTuple* f_field_function_tuple_albedo=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::set_albedo,
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::get_albedo,
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::getClassName,
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::getFieldName_albedo,
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::getFieldTypeName_albedo,
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::isArray_albedo);
        REGISTER_FIELD_TO_MAP("MeshAsset", f_field_function_tuple_albedo);
        FieldFunctionTuple* f_field_function_tuple_normal_map=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::set_normal_map,
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::get_normal_map,
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::getClassName,
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::getFieldName_normal_map,
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::getFieldTypeName_normal_map,
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::isArray_normal_map);
        REGISTER_FIELD_TO_MAP("MeshAsset", f_field_function_tuple_normal_map);
        FieldFunctionTuple* f_field_function_tuple_metallic_roughness=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::set_metallic_roughness,
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::get_metallic_roughness,
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::getClassName,
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::getFieldName_metallic_roughness,
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::getFieldTypeName_metallic_roughness,
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::isArray_metallic_roughness);
        REGISTER_FIELD_TO_MAP("MeshAsset", f_field_function_tuple_metallic_roughness);
        
        
        ClassFunctionTuple* f_class_function_tuple_MeshAsset=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::getMeshAssetBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeMeshAssetOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("MeshAsset", f_class_function_tuple_MeshAsset);
    }
namespace TypeWrappersRegister{
        void MeshAsset(){ TypeWrapperRegister_MeshAsset();}
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace Piccolo

