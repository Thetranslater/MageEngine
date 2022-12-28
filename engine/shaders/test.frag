#version 450

layout(set=1,binding=0) uniform sampler2D albedo;
layout(set=1,binding=1) uniform sampler2D normal;
layout(set=1,binding=2) uniform sampler2D metallic_roughness;

layout(location=0) in vec2 texcoord_out;

layout(location=0) out vec4 outColor;

void main(){
    outColor = texture(albedo,texcoord_out);
}