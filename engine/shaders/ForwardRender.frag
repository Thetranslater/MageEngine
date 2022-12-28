#version 450

struct PerMeshFragmentShaderData
{
    float metallic_factor;
    float roughness_factor;
    float _unused_blank_1;
    float _unused_blank_2;
    highp vec4 base_color_factor;
};

layout(set = 0, binding = 2) readonly uniform GlobalBufferPerDrawcallFragmentShaderData
{
    PerMeshFragmentShaderData frag_datas[60];
};

layout(set = 1, binding = 0) uniform sampler2D albedo_texture;
layout(set = 1, binding = 1) uniform sampler2D metallic_texture;
layout(set = 1, binding = 2) uniform sampler2D roughness_texture;

layout(location = 0) in highp vec3 in_world_position;
layout(location = 1) in highp vec3 in_world_normal;
layout(location = 2) in highp vec4 in_world_tangent;
layout(location = 3) in highp vec2 in_texcoord;
layout(location = 4) flat in int in_instanceID;

layout(location = 0) out highp vec4 out_final_color;

void main(){
    out_final_color = vec4(in_world_position,1.f);
}