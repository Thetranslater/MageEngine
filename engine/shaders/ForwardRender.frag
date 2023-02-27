#version 450

struct PerMeshFragmentShaderData{
    float metallic_factor;
    float roughness_factor;
    float _unused_blank_1;
    float _unused_blank_2;
    vec4  base_color_factor;
};

layout(push_constant) uniform push_data{
    int alpha_mode;
    float alpha_cut_off;
};

layout(set = 1, binding = 0) uniform sampler2D albedo_texture;
layout(set = 1, binding = 1) uniform sampler2D metallic_texture;
layout(set = 1, binding = 2) uniform sampler2D roughness_texture;

layout(location = 0) in highp vec3 in_world_position;
layout(location = 1) in highp vec3 in_world_normal;
layout(location = 2) in highp vec4 in_world_tangent;
layout(location = 3) in highp vec2 in_texcoord;
layout(location = 4) in PerMeshFragmentShaderData in_frag_factors;

layout(location = 0) out highp vec4 out_final_color;

void main(){
    vec4 sample_color = texture(albedo_texture, in_texcoord);
    if(alpha_mode == 1 && sample_color.a < alpha_cut_off){
        discard;
    }
    out_final_color = sample_color * in_frag_factors.base_color_factor;
}