#version 450

layout(set=0,binding=0) readonly buffer perframe{
    mat4 view;
    mat4 perv;
};

layout(set=0,binding=1) readonly buffer ubo{
    mat4 model[60];
};

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec4 tangent;
layout(location=3) in vec2 texcoord;

layout(location=0) out vec2 texcoord_out;

void main(){
    vec4 homo_position = vec4(position, 1.f);
    texcoord_out = texcoord;
    gl_Position = perv * view * model[gl_InstanceIndex] * homo_position;
}