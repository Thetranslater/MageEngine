#version 450

struct PerMeshVertexShaderData{
    highp mat4 matrix;
};

layout(set = 0, binding = 0) readonly uniform per_frame_data
{
    highp mat4 camera_view_matrix;
    highp mat4 camera_perspective_matrix;
};

layout(set = 0, binding = 1) readonly uniform GlobalBufferPerDrawcallVertexShaderData
{
    PerMeshVertexShaderData mesh_datas[60];
};

layout(location = 0) in highp vec3 in_position;
layout(location = 1) in highp vec3 in_normal;
layout(location = 2) in highp vec4 in_tangent;
layout(location = 3) in highp vec2 in_texcoord;

layout(location = 0) out highp vec3 out_world_position;
layout(location = 1) out highp vec3 out_world_normal;
layout(location = 2) out highp vec4 out_world_tangent;
layout(location = 3) out highp vec2 out_texcoord;
layout(location = 4) out int out_instanceID;

void main(){
    out_instanceID = gl_InstanceIndex;

    highp mat4 model_matrix = mesh_datas[gl_InstanceIndex].matrix;
    highp mat3 model_matrix_3x3 = mat3(model_matrix[0].xyz, model_matrix[1].xyz, model_matrix[2].xyz);
    //NO BLENDING FOR NOW
    out_world_position = (model_matrix * vec4(in_position,1.f)).xyz;
    out_world_normal = normalize(model_matrix_3x3 * in_normal);
    out_world_tangent = vec4(normalize(model_matrix_3x3 * in_tangent.xyz), in_tangent.w);
    
    gl_Position = (camera_perspective_matrix * camera_view_matrix * vec4(out_world_position, 1.f));
}