#version 450

struct PerMeshVertexShaderData{
    highp mat4 matrix;
    highp mat4 transform;
};

struct PerMeshFragmentShaderData{
    highp float metallic_factor;
    highp float roughness_factor;
    highp float _unused_blank_1;
    highp float _unused_blank_2;
    highp vec4  base_color_factor;
};

struct PerMeshShaderData{
    PerMeshVertexShaderData vertex_data;
    PerMeshFragmentShaderData fragment_data;
};

struct PerDirectionalLightData{
    highp vec3 direction;
    highp float intensity;
    highp vec3 color;
    highp float _unused_blank_1;
    highp mat4 ortho_view_matrix;
};

struct PerPointLightData{
    highp vec3 position;
    highp float intensity;
    highp vec3 color;
    highp float _unused_blank_1; //radius
};

layout(set = 0, binding = 0) readonly buffer per_frame_data
{
    highp mat4 camera_view_matrix;
    highp mat4 camera_perspective_matrix;
    highp vec3 camera_positon;
    highp float _unused_blank_1;
    PerDirectionalLightData directional_lights[2];
    PerPointLightData point_lights[8];
    int directional_light_num;
    int point_light_num;
    int _unused_blank_2;
    int _unused_blank_3;
};

layout(set = 0, binding = 1) readonly buffer GlobalBufferPerDrawcallData
{
    PerMeshShaderData data[32];
};


layout(location = 0) in highp vec3 in_position;
layout(location = 1) in highp vec3 in_normal;
layout(location = 2) in highp vec4 in_tangent;
layout(location = 3) in highp vec2 in_texcoord;

void main(){
    highp mat4 model_to_world = data[gl_InstanceIndex].vertex_data.transform * data[gl_InstanceIndex].vertex_data.matrix;
    highp mat4 directional_light_pers_view_mat = directional_lights[0].ortho_view_matrix;
    highp vec4 world_position = model_to_world * vec4(in_position, 1.f);
    gl_Position = directional_light_pers_view_mat * world_position;
}