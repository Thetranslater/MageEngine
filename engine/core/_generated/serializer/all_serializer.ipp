#pragma once
#include "_generated\serializer\vector3.serializer.gen.h"
#include "_generated\serializer\Light_component.serializer.gen.h"
#include "_generated\serializer\Transform_component.serializer.gen.h"
#include "_generated\serializer\object_asset.serializer.gen.h"
#include "_generated\serializer\scene_asset.serializer.gen.h"
#include "_generated\serializer\mesh_asset.serializer.gen.h"
#include "_generated\serializer\component.serializer.gen.h"
#include "_generated\serializer\world_asset.serializer.gen.h"
#include "_generated\serializer\quaternion.serializer.gen.h"
#include "_generated\serializer\vector2.serializer.gen.h"
#include "_generated\serializer\Mesh_component.serializer.gen.h"
#include "_generated\serializer\Camera_component.serializer.gen.h"
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
    PJson PSerializer::write(const LightComponent& instance){
        PJson::object  ret_context;
        auto&&  json_context_0 = PSerializer::write(*(Mage::Component*)&instance);
        assert(json_context_0.is_object());
        auto&& json_context_map_0 = json_context_0.object_items();
        ret_context.insert(json_context_map_0.begin() , json_context_map_0.end());
        ret_context.insert_or_assign("type", PSerializer::write(instance.type));
        ret_context.insert_or_assign("color", PSerializer::write(instance.color));
        ret_context.insert_or_assign("intensity", PSerializer::write(instance.intensity));
        return  PJson(ret_context);
    }
    template<>
    LightComponent& PSerializer::read(const PJson& json_context, LightComponent& instance){
        assert(json_context.is_object());
        PSerializer::read(json_context,*(Mage::Component*)&instance);
        if(!json_context["type"].is_null()){
            PSerializer::read(json_context["type"], instance.type);
        }
        if(!json_context["color"].is_null()){
            PSerializer::read(json_context["color"], instance.color);
        }
        if(!json_context["intensity"].is_null()){
            PSerializer::read(json_context["intensity"], instance.intensity);
        }
        return instance;
    }
    template<>
    PJson PSerializer::write(const TransformComponent& instance){
        PJson::object  ret_context;
        auto&&  json_context_0 = PSerializer::write(*(Mage::Component*)&instance);
        assert(json_context_0.is_object());
        auto&& json_context_map_0 = json_context_0.object_items();
        ret_context.insert(json_context_map_0.begin() , json_context_map_0.end());
        ret_context.insert_or_assign("position", PSerializer::write(instance.position));
        ret_context.insert_or_assign("rotation", PSerializer::write(instance.rotation));
        ret_context.insert_or_assign("scale", PSerializer::write(instance.scale));
        return  PJson(ret_context);
    }
    template<>
    TransformComponent& PSerializer::read(const PJson& json_context, TransformComponent& instance){
        assert(json_context.is_object());
        PSerializer::read(json_context,*(Mage::Component*)&instance);
        if(!json_context["position"].is_null()){
            PSerializer::read(json_context["position"], instance.position);
        }
        if(!json_context["rotation"].is_null()){
            PSerializer::read(json_context["rotation"], instance.rotation);
        }
        if(!json_context["scale"].is_null()){
            PSerializer::read(json_context["scale"], instance.scale);
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
    PJson PSerializer::write(const SceneNode& instance){
        PJson::object  ret_context;
        
        ret_context.insert_or_assign("parent", PSerializer::write(instance.parent));
        PJson::array children_json;
        for (auto& item : instance.children){
            children_json.emplace_back(PSerializer::write(item));
        }
        ret_context.insert_or_assign("children",children_json);
        
        return  PJson(ret_context);
    }
    template<>
    SceneNode& PSerializer::read(const PJson& json_context, SceneNode& instance){
        assert(json_context.is_object());
        
        if(!json_context["parent"].is_null()){
            PSerializer::read(json_context["parent"], instance.parent);
        }
        if(!json_context["children"].is_null()){
            assert(json_context["children"].is_array());
            PJson::array array_children = json_context["children"].array_items();
            instance.children.resize(array_children.size());
            for (size_t index=0; index < array_children.size();++index){
                PSerializer::read(array_children[index], instance.children[index]);
            }
        }
        return instance;
    }
    template<>
    PJson PSerializer::write(const SceneAsset& instance){
        PJson::object  ret_context;
        
        ret_context.insert_or_assign("name", PSerializer::write(instance.name));
        PJson::array roots_json;
        for (auto& item : instance.roots){
            roots_json.emplace_back(PSerializer::write(item));
        }
        ret_context.insert_or_assign("roots",roots_json);
        
        PJson::array nodes_json;
        for (auto& item : instance.nodes){
            nodes_json.emplace_back(PSerializer::write(item));
        }
        ret_context.insert_or_assign("nodes",nodes_json);
        
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
        if(!json_context["roots"].is_null()){
            assert(json_context["roots"].is_array());
            PJson::array array_roots = json_context["roots"].array_items();
            instance.roots.resize(array_roots.size());
            for (size_t index=0; index < array_roots.size();++index){
                PSerializer::read(array_roots[index], instance.roots[index]);
            }
        }
        if(!json_context["nodes"].is_null()){
            assert(json_context["nodes"].is_array());
            PJson::array array_nodes = json_context["nodes"].array_items();
            instance.nodes.resize(array_nodes.size());
            for (size_t index=0; index < array_nodes.size();++index){
                PSerializer::read(array_nodes[index], instance.nodes[index]);
            }
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
    PJson PSerializer::write(const MeshAsset& instance){
        PJson::object  ret_context;
        
        ret_context.insert_or_assign("gltf_model_url", PSerializer::write(instance.gltf_model_url));
        ret_context.insert_or_assign("albedo", PSerializer::write(instance.albedo));
        ret_context.insert_or_assign("normal_map", PSerializer::write(instance.normal_map));
        ret_context.insert_or_assign("metallic_roughness", PSerializer::write(instance.metallic_roughness));
        return  PJson(ret_context);
    }
    template<>
    MeshAsset& PSerializer::read(const PJson& json_context, MeshAsset& instance){
        assert(json_context.is_object());
        
        if(!json_context["gltf_model_url"].is_null()){
            PSerializer::read(json_context["gltf_model_url"], instance.gltf_model_url);
        }
        if(!json_context["albedo"].is_null()){
            PSerializer::read(json_context["albedo"], instance.albedo);
        }
        if(!json_context["normal_map"].is_null()){
            PSerializer::read(json_context["normal_map"], instance.normal_map);
        }
        if(!json_context["metallic_roughness"].is_null()){
            PSerializer::read(json_context["metallic_roughness"], instance.metallic_roughness);
        }
        return instance;
    }
    template<>
    PJson PSerializer::write(const Component& instance){
        PJson::object  ret_context;
        
        
        return  PJson(ret_context);
    }
    template<>
    Component& PSerializer::read(const PJson& json_context, Component& instance){
        assert(json_context.is_object());
        
        
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
    template<>
    PJson PSerializer::write(const MeshComponent& instance){
        PJson::object  ret_context;
        auto&&  json_context_0 = PSerializer::write(*(Mage::Component*)&instance);
        assert(json_context_0.is_object());
        auto&& json_context_map_0 = json_context_0.object_items();
        ret_context.insert(json_context_map_0.begin() , json_context_map_0.end());
        ret_context.insert_or_assign("mesh", PSerializer::write(instance.mesh));
        return  PJson(ret_context);
    }
    template<>
    MeshComponent& PSerializer::read(const PJson& json_context, MeshComponent& instance){
        assert(json_context.is_object());
        PSerializer::read(json_context,*(Mage::Component*)&instance);
        if(!json_context["mesh"].is_null()){
            PSerializer::read(json_context["mesh"], instance.mesh);
        }
        return instance;
    }
    template<>
    PJson PSerializer::write(const CameraComponent& instance){
        PJson::object  ret_context;
        auto&&  json_context_0 = PSerializer::write(*(Mage::Component*)&instance);
        assert(json_context_0.is_object());
        auto&& json_context_map_0 = json_context_0.object_items();
        ret_context.insert(json_context_map_0.begin() , json_context_map_0.end());
        ret_context.insert_or_assign("field_of_view", PSerializer::write(instance.field_of_view));
        ret_context.insert_or_assign("near_clip_plane", PSerializer::write(instance.near_clip_plane));
        ret_context.insert_or_assign("far_clip_plane", PSerializer::write(instance.far_clip_plane));
        return  PJson(ret_context);
    }
    template<>
    CameraComponent& PSerializer::read(const PJson& json_context, CameraComponent& instance){
        assert(json_context.is_object());
        PSerializer::read(json_context,*(Mage::Component*)&instance);
        if(!json_context["field_of_view"].is_null()){
            PSerializer::read(json_context["field_of_view"], instance.field_of_view);
        }
        if(!json_context["near_clip_plane"].is_null()){
            PSerializer::read(json_context["near_clip_plane"], instance.near_clip_plane);
        }
        if(!json_context["far_clip_plane"].is_null()){
            PSerializer::read(json_context["far_clip_plane"], instance.far_clip_plane);
        }
        return instance;
    }

}

