#pragma once
#include "_generated\serializer\vector3.serializer.gen.h"
#include "_generated\serializer\object_asset.serializer.gen.h"
#include "_generated\serializer\scene_asset.serializer.gen.h"
#include "_generated\serializer\world_asset.serializer.gen.h"
#include "_generated\serializer\quaternion.serializer.gen.h"
#include "_generated\serializer\vector2.serializer.gen.h"
namespace Mage{
    template<>
    PJson PSerializer::write(const Vector3& instance){
        PJson::object  ret_context;
        
        ret_context.insert_or_assign("x", PSerializer::write(instance.x));
        ret_context.insert_or_assign("y", PSerializer::write(instance.y));
        ret_context.insert_or_assign("z", PSerializer::write(instance.z));
        return  PJson(ret_context);
    }
    template<>
    Vector3& PSerializer::read(const PJson& json_context, Vector3& instance){
        assert(json_context.is_object());
        
        if(!json_context["x"].is_null()){
            PSerializer::read(json_context["x"], instance.x);
        }
        if(!json_context["y"].is_null()){
            PSerializer::read(json_context["y"], instance.y);
        }
        if(!json_context["z"].is_null()){
            PSerializer::read(json_context["z"], instance.z);
        }
        return instance;
    }
    template<>
    PJson PSerializer::write(const ObjectAsset& instance){
        PJson::object  ret_context;
        
        ret_context.insert_or_assign("name", PSerializer::write(instance.name));
        PJson::array components_json;
        for (auto& item : instance.components){
            components_json.emplace_back(PSerializer::write(item));
        }
        ret_context.insert_or_assign("components",components_json);
        
        return  PJson(ret_context);
    }
    template<>
    ObjectAsset& PSerializer::read(const PJson& json_context, ObjectAsset& instance){
        assert(json_context.is_object());
        
        if(!json_context["name"].is_null()){
            PSerializer::read(json_context["name"], instance.name);
        }
        if(!json_context["components"].is_null()){
            assert(json_context["components"].is_array());
            PJson::array array_components = json_context["components"].array_items();
            instance.components.resize(array_components.size());
            for (size_t index=0; index < array_components.size();++index){
                PSerializer::read(array_components[index], instance.components[index]);
            }
        }
        return instance;
    }
    template<>
    PJson PSerializer::write(const SceneAsset& instance){
        PJson::object  ret_context;
        
        ret_context.insert_or_assign("name", PSerializer::write(instance.name));
        PJson::array objects_json;
        for (auto& item : instance.objects){
            objects_json.emplace_back(PSerializer::write(item));
        }
        ret_context.insert_or_assign("objects",objects_json);
        
        return  PJson(ret_context);
    }
    template<>
    SceneAsset& PSerializer::read(const PJson& json_context, SceneAsset& instance){
        assert(json_context.is_object());
        
        if(!json_context["name"].is_null()){
            PSerializer::read(json_context["name"], instance.name);
        }
        if(!json_context["objects"].is_null()){
            assert(json_context["objects"].is_array());
            PJson::array array_objects = json_context["objects"].array_items();
            instance.objects.resize(array_objects.size());
            for (size_t index=0; index < array_objects.size();++index){
                PSerializer::read(array_objects[index], instance.objects[index]);
            }
        }
        return instance;
    }
    template<>
    PJson PSerializer::write(const WorldAsset& instance){
        PJson::object  ret_context;
        
        ret_context.insert_or_assign("name", PSerializer::write(instance.name));
        ret_context.insert_or_assign("default_scene", PSerializer::write(instance.default_scene));
        PJson::array scene_urls_json;
        for (auto& item : instance.scene_urls){
            scene_urls_json.emplace_back(PSerializer::write(item));
        }
        ret_context.insert_or_assign("scene_urls",scene_urls_json);
        
        return  PJson(ret_context);
    }
    template<>
    WorldAsset& PSerializer::read(const PJson& json_context, WorldAsset& instance){
        assert(json_context.is_object());
        
        if(!json_context["name"].is_null()){
            PSerializer::read(json_context["name"], instance.name);
        }
        if(!json_context["default_scene"].is_null()){
            PSerializer::read(json_context["default_scene"], instance.default_scene);
        }
        if(!json_context["scene_urls"].is_null()){
            assert(json_context["scene_urls"].is_array());
            PJson::array array_scene_urls = json_context["scene_urls"].array_items();
            instance.scene_urls.resize(array_scene_urls.size());
            for (size_t index=0; index < array_scene_urls.size();++index){
                PSerializer::read(array_scene_urls[index], instance.scene_urls[index]);
            }
        }
        return instance;
    }
    template<>
    PJson PSerializer::write(const Quaternion& instance){
        PJson::object  ret_context;
        
        ret_context.insert_or_assign("w", PSerializer::write(instance.w));
        ret_context.insert_or_assign("x", PSerializer::write(instance.x));
        ret_context.insert_or_assign("y", PSerializer::write(instance.y));
        ret_context.insert_or_assign("z", PSerializer::write(instance.z));
        return  PJson(ret_context);
    }
    template<>
    Quaternion& PSerializer::read(const PJson& json_context, Quaternion& instance){
        assert(json_context.is_object());
        
        if(!json_context["w"].is_null()){
            PSerializer::read(json_context["w"], instance.w);
        }
        if(!json_context["x"].is_null()){
            PSerializer::read(json_context["x"], instance.x);
        }
        if(!json_context["y"].is_null()){
            PSerializer::read(json_context["y"], instance.y);
        }
        if(!json_context["z"].is_null()){
            PSerializer::read(json_context["z"], instance.z);
        }
        return instance;
    }
    template<>
    PJson PSerializer::write(const Vector2& instance){
        PJson::object  ret_context;
        
        ret_context.insert_or_assign("x", PSerializer::write(instance.x));
        ret_context.insert_or_assign("y", PSerializer::write(instance.y));
        return  PJson(ret_context);
    }
    template<>
    Vector2& PSerializer::read(const PJson& json_context, Vector2& instance){
        assert(json_context.is_object());
        
        if(!json_context["x"].is_null()){
            PSerializer::read(json_context["x"], instance.x);
        }
        if(!json_context["y"].is_null()){
            PSerializer::read(json_context["y"], instance.y);
        }
        return instance;
    }

}

