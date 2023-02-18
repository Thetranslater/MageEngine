#pragma once
#include "..\asset\scene_asset.h"

namespace Mage{
    class SceneNode;
    class SceneAsset;
namespace Reflection{
namespace TypeFieldReflectionOparator{
    class TypeSceneNodeOperator{
    public:
        static const char* getClassName(){ return "SceneNode";}
        static void* constructorWithJson(const PJson& json_context){
            SceneNode* ret_instance= new SceneNode;
            PSerializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static PJson writeByName(void* instance){
            return PSerializer::write(*(SceneNode*)instance);
        }
        // base class
        static int getSceneNodeBaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = 0;
            
            return count;
        }
        // fields
        static const char* getFieldName_parent(){ return "parent";}
        static const char* getFieldTypeName_parent(){ return "int";}
        static void set_parent(void* instance, void* field_value){ static_cast<SceneNode*>(instance)->parent = *static_cast<int*>(field_value);}
        static void* get_parent(void* instance){ return static_cast<void*>(&(static_cast<SceneNode*>(instance)->parent));}
        static bool isArray_parent(){ return false; }
        static const char* getFieldName_children(){ return "children";}
        static const char* getFieldTypeName_children(){ return "std::vector<int>";}
        static void set_children(void* instance, void* field_value){ static_cast<SceneNode*>(instance)->children = *static_cast<std::vector<int>*>(field_value);}
        static void* get_children(void* instance){ return static_cast<void*>(&(static_cast<SceneNode*>(instance)->children));}
        static bool isArray_children(){ return true; }
    };
}//namespace TypeFieldReflectionOparator
namespace ArrayReflectionOperator{
#ifndef ArraystdSSvectorLintROperatorMACRO
#define ArraystdSSvectorLintROperatorMACRO
    class ArraystdSSvectorLintROperator{
        public:
            static const char* getArrayTypeName(){ return "std::vector<int>";}
            static const char* getElementTypeName(){ return "int";}
            static int getSize(void* instance){
                //todo: should check validation
                return static_cast<int>(static_cast<std::vector<int>*>(instance)->size());
            }
            static void* get(int index,void* instance){
                //todo: should check validation
                return static_cast<void*>(&((*static_cast<std::vector<int>*>(instance))[index]));
            }
            static void set(int index, void* instance, void* element_value){
                //todo: should check validation
                (*static_cast<std::vector<int>*>(instance))[index] = *static_cast<int*>(element_value);
            }
    };
#endif //ArraystdSSvectorLintROperator
}//namespace ArrayReflectionOperator

    void TypeWrapperRegister_SceneNode(){
        FieldFunctionTuple* f_field_function_tuple_parent=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeSceneNodeOperator::set_parent,
            &TypeFieldReflectionOparator::TypeSceneNodeOperator::get_parent,
            &TypeFieldReflectionOparator::TypeSceneNodeOperator::getClassName,
            &TypeFieldReflectionOparator::TypeSceneNodeOperator::getFieldName_parent,
            &TypeFieldReflectionOparator::TypeSceneNodeOperator::getFieldTypeName_parent,
            &TypeFieldReflectionOparator::TypeSceneNodeOperator::isArray_parent);
        REGISTER_FIELD_TO_MAP("SceneNode", f_field_function_tuple_parent);
        FieldFunctionTuple* f_field_function_tuple_children=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeSceneNodeOperator::set_children,
            &TypeFieldReflectionOparator::TypeSceneNodeOperator::get_children,
            &TypeFieldReflectionOparator::TypeSceneNodeOperator::getClassName,
            &TypeFieldReflectionOparator::TypeSceneNodeOperator::getFieldName_children,
            &TypeFieldReflectionOparator::TypeSceneNodeOperator::getFieldTypeName_children,
            &TypeFieldReflectionOparator::TypeSceneNodeOperator::isArray_children);
        REGISTER_FIELD_TO_MAP("SceneNode", f_field_function_tuple_children);
        
        ArrayFunctionTuple* f_array_tuple_stdSSvectorLintR = new  ArrayFunctionTuple(
            &ArrayReflectionOperator::ArraystdSSvectorLintROperator::set,
            &ArrayReflectionOperator::ArraystdSSvectorLintROperator::get,
            &ArrayReflectionOperator::ArraystdSSvectorLintROperator::getSize,
            &ArrayReflectionOperator::ArraystdSSvectorLintROperator::getArrayTypeName,
            &ArrayReflectionOperator::ArraystdSSvectorLintROperator::getElementTypeName);
        REGISTER_ARRAY_TO_MAP("std::vector<int>", f_array_tuple_stdSSvectorLintR);
        ClassFunctionTuple* f_class_function_tuple_SceneNode=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeSceneNodeOperator::getSceneNodeBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeSceneNodeOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeSceneNodeOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("SceneNode", f_class_function_tuple_SceneNode);
    }namespace TypeFieldReflectionOparator{
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
        static const char* getFieldName_roots(){ return "roots";}
        static const char* getFieldTypeName_roots(){ return "std::vector<int>";}
        static void set_roots(void* instance, void* field_value){ static_cast<SceneAsset*>(instance)->roots = *static_cast<std::vector<int>*>(field_value);}
        static void* get_roots(void* instance){ return static_cast<void*>(&(static_cast<SceneAsset*>(instance)->roots));}
        static bool isArray_roots(){ return true; }
        static const char* getFieldName_nodes(){ return "nodes";}
        static const char* getFieldTypeName_nodes(){ return "std::vector<SceneNode>";}
        static void set_nodes(void* instance, void* field_value){ static_cast<SceneAsset*>(instance)->nodes = *static_cast<std::vector<SceneNode>*>(field_value);}
        static void* get_nodes(void* instance){ return static_cast<void*>(&(static_cast<SceneAsset*>(instance)->nodes));}
        static bool isArray_nodes(){ return true; }
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
#ifndef ArraystdSSvectorLSceneNodeROperatorMACRO
#define ArraystdSSvectorLSceneNodeROperatorMACRO
    class ArraystdSSvectorLSceneNodeROperator{
        public:
            static const char* getArrayTypeName(){ return "std::vector<SceneNode>";}
            static const char* getElementTypeName(){ return "SceneNode";}
            static int getSize(void* instance){
                //todo: should check validation
                return static_cast<int>(static_cast<std::vector<SceneNode>*>(instance)->size());
            }
            static void* get(int index,void* instance){
                //todo: should check validation
                return static_cast<void*>(&((*static_cast<std::vector<SceneNode>*>(instance))[index]));
            }
            static void set(int index, void* instance, void* element_value){
                //todo: should check validation
                (*static_cast<std::vector<SceneNode>*>(instance))[index] = *static_cast<SceneNode*>(element_value);
            }
    };
#endif //ArraystdSSvectorLSceneNodeROperator
#ifndef ArraystdSSvectorLintROperatorMACRO
#define ArraystdSSvectorLintROperatorMACRO
    class ArraystdSSvectorLintROperator{
        public:
            static const char* getArrayTypeName(){ return "std::vector<int>";}
            static const char* getElementTypeName(){ return "int";}
            static int getSize(void* instance){
                //todo: should check validation
                return static_cast<int>(static_cast<std::vector<int>*>(instance)->size());
            }
            static void* get(int index,void* instance){
                //todo: should check validation
                return static_cast<void*>(&((*static_cast<std::vector<int>*>(instance))[index]));
            }
            static void set(int index, void* instance, void* element_value){
                //todo: should check validation
                (*static_cast<std::vector<int>*>(instance))[index] = *static_cast<int*>(element_value);
            }
    };
#endif //ArraystdSSvectorLintROperator
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
        FieldFunctionTuple* f_field_function_tuple_roots=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::set_roots,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::get_roots,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::getClassName,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::getFieldName_roots,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::getFieldTypeName_roots,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::isArray_roots);
        REGISTER_FIELD_TO_MAP("SceneAsset", f_field_function_tuple_roots);
        FieldFunctionTuple* f_field_function_tuple_nodes=new FieldFunctionTuple(
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::set_nodes,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::get_nodes,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::getClassName,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::getFieldName_nodes,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::getFieldTypeName_nodes,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::isArray_nodes);
        REGISTER_FIELD_TO_MAP("SceneAsset", f_field_function_tuple_nodes);
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
        ArrayFunctionTuple* f_array_tuple_stdSSvectorLSceneNodeR = new  ArrayFunctionTuple(
            &ArrayReflectionOperator::ArraystdSSvectorLSceneNodeROperator::set,
            &ArrayReflectionOperator::ArraystdSSvectorLSceneNodeROperator::get,
            &ArrayReflectionOperator::ArraystdSSvectorLSceneNodeROperator::getSize,
            &ArrayReflectionOperator::ArraystdSSvectorLSceneNodeROperator::getArrayTypeName,
            &ArrayReflectionOperator::ArraystdSSvectorLSceneNodeROperator::getElementTypeName);
        REGISTER_ARRAY_TO_MAP("std::vector<SceneNode>", f_array_tuple_stdSSvectorLSceneNodeR);
        ArrayFunctionTuple* f_array_tuple_stdSSvectorLintR = new  ArrayFunctionTuple(
            &ArrayReflectionOperator::ArraystdSSvectorLintROperator::set,
            &ArrayReflectionOperator::ArraystdSSvectorLintROperator::get,
            &ArrayReflectionOperator::ArraystdSSvectorLintROperator::getSize,
            &ArrayReflectionOperator::ArraystdSSvectorLintROperator::getArrayTypeName,
            &ArrayReflectionOperator::ArraystdSSvectorLintROperator::getElementTypeName);
        REGISTER_ARRAY_TO_MAP("std::vector<int>", f_array_tuple_stdSSvectorLintR);
        ClassFunctionTuple* f_class_function_tuple_SceneAsset=new ClassFunctionTuple(
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::getSceneAssetBaseClassReflectionInstanceList,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::constructorWithJson,
            &TypeFieldReflectionOparator::TypeSceneAssetOperator::writeByName);
        REGISTER_BASE_CLASS_TO_MAP("SceneAsset", f_class_function_tuple_SceneAsset);
    }
namespace TypeWrappersRegister{
        void SceneNode(){ TypeWrapperRegister_SceneNode();}
        void SceneAsset(){ TypeWrapperRegister_SceneAsset();}
}//namespace TypeWrappersRegister

}//namespace Reflection
}//namespace Piccolo

