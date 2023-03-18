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
    PerDirectionalLightData directional_lights[8];
    PerPointLightData point_lights[8];
    int directional_light_num;
    int point_light_num;
    int _unused_blank_2;
    int _unused_blank_3;
};

layout(set = 0, binding = 1) readonly buffer GlobalBufferPerDrawcallData
{
    PerMeshShaderData data[60];
};

layout(location = 0) in highp vec3 in_position;
layout(location = 1) in highp vec3 in_normal;
layout(location = 2) in highp vec4 in_tangent;
layout(location = 3) in highp vec2 in_texcoord;

layout(location = 0) out highp vec3 out_world_position;
layout(location = 1) out highp vec3 out_world_normal;
layout(location = 2) out highp vec4 out_world_tangent;
layout(location = 3) out highp vec2 out_texcoord;
layout(location = 4) out PerMeshFragmentShaderData out_frag_factors;

void main(){
    highp mat4 matrix4x4 = data[gl_InstanceIndex].vertex_data.transform * data[gl_InstanceIndex].vertex_data.matrix;
    highp vec4 world = matrix4x4 * vec4(in_position,1.f);
    highp mat3 to_world = mat3(matrix4x4[0].xyz, matrix4x4[1].xyz, matrix4x4[2].xyz);
    out_world_position = world.xyz;
    out_world_normal = to_world * in_normal;
    out_world_tangent = vec4(to_world * in_tangent.xyz, in_tangent.w);
    out_texcoord = in_texcoord;
    out_frag_factors = data[gl_InstanceIndex].fragment_data;
    gl_Position = camera_perspective_matrix * camera_view_matrix * world;
}