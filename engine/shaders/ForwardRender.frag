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
    highp vec3 camera_position;
    float _unused_blank_1;
    PerDirectionalLightData directional_lights[2];
    PerPointLightData point_lights[8];
    int directional_light_num;
    int point_light_num;
    int _unused_blank_2;
    int _unused_blank_3;
};

layout(set = 1, binding = 0) uniform sampler2D albedo_texture;
layout(set = 1, binding = 1) uniform sampler2D normal_texture;
layout(set = 1, binding = 2) uniform sampler2D metallic_roughness_texture;

layout(set = 2, binding = 0) uniform sampler2D directional_shadow_map;

layout(location = 0) in highp vec3 in_world_position;
layout(location = 1) in highp vec3 in_world_normal;
layout(location = 2) in highp vec4 in_world_tangent;
layout(location = 3) in highp vec2 in_texcoord;
layout(location = 4) in PerMeshFragmentShaderData in_frag_factors;

layout(location = 0) out highp vec4 out_final_color;

//function
highp vec4 sRGB2Linear(highp vec4 srgb){
    highp vec4 linr = vec4(pow(srgb.xyz,vec3(2.2)), srgb.w);
    return linr;
}

highp vec3 getWorldNormal(){
    highp vec3 world_bitangent = normalize(cross(in_world_normal, in_world_tangent.xyz) * in_world_tangent.w);
    highp vec3 sample_normal = texture(normal_texture, in_texcoord).xyz * 2.f - 1.f;
    highp mat3 TBN = mat3(in_world_tangent.xyz, world_bitangent, in_world_normal);
    return normalize(TBN * sample_normal);
}

highp vec4 getBaseColor(){
    highp vec4 sample_base_color = texture(albedo_texture, in_texcoord);
    return sample_base_color;
}

highp vec3 F(highp vec3 f0, highp vec3 N, highp vec3 L){
    highp float dot_NL = clamp(dot(N,L),0.f,1.f);
    highp float dot_power_5 = pow((1.f - dot_NL),5.f);
    return f0 + (1.f - f0) * dot_power_5;
}

highp float D(highp float r, highp vec3 N, highp vec3 M){
    highp float dot_NM = dot(N,M);
    highp float a = r * r * r * r;
    highp float temp = 1.f + dot_NM * dot_NM * (a - 1.f);
    return clamp(dot_NM,0.f,1.f) * a / (3.1415926 * temp * temp);
}

highp float G(highp float r, highp vec3 N, highp vec3 L, highp vec3 V){
    highp float a = (0.5 + r / 2.f) * (0.5 + r / 2.f);
    highp float a2 = a * a;
    highp float dot_NL = dot(N,L);
    highp float dot_NV = dot(N,V);
    highp float g1 = 2.f * dot_NL / (dot_NL + sqrt(a2 + (1.f - a2) * dot_NL * dot_NL));
    highp float g2 = 2.f * dot_NV / (dot_NV + sqrt(a2 + (1.f - a2) * dot_NV * dot_NV));
    return g1 * g2;
}

highp vec3 BRDF(
    highp vec3 N, 
    highp vec3 L, 
    highp vec3 V,
    highp vec3 f0, 
    highp vec3 baseColor, 
    highp float metallic, 
    highp float roughness)
{
    highp vec3 M = normalize(L + V);
    baseColor = baseColor * in_frag_factors.base_color_factor.xyz;
    highp vec3 diffuse = baseColor / 3.1415926;
    highp vec3 Fres = F(f0, N, L);
    highp float Distri = D(roughness, N, M);
    highp float Geo = G(roughness, N, L, V);

    highp vec3 spec = Fres * Distri * Geo / (4.f * dot(N, L) * dot(N, V) + 0.001);
    highp vec3 k = (1 - Fres) * (1 - metallic);

    return k * diffuse + (1 - k) * spec;
}

highp vec3 Uncharted2_tonemapping(highp vec3 v){
    float A = 0.15;
    float B = 0.50;
    float C = 0.10;
    float D = 0.20;
    float E = 0.02;
    float F = 0.30;

    return ((v*(A*v+C*B)+D*E)/(v*(A*v+B)+D*F))-E/F;
}

highp vec3 Uncharted2_filmic(highp vec3 color){
    float exposure_bias = 2.0;
    color = Uncharted2_tonemapping(exposure_bias*color);
    return color*(vec3(1.0)/Uncharted2_tonemapping(vec3(11.2)));
}

void main(){
    highp vec4 BaseColor = getBaseColor();
    if(alpha_mode == 1 && BaseColor.a < alpha_cut_off){
        discard;
    }
    highp vec3 N = getWorldNormal();
    highp vec3 V = normalize(camera_position - in_world_position);
    highp vec4 pComined = texture(metallic_roughness_texture, in_texcoord);
    highp float pMetallic = pComined.b * in_frag_factors.metallic_factor;
    highp float pRoughness = pComined.g * in_frag_factors.roughness_factor;
    highp vec3 f0 = mix(vec3(0.04), BaseColor.xyz, pMetallic);

    highp vec3 lo = vec3(0.f);
    //for(highp int i = 0; i < directional_light_num; ++i){
        highp vec3 L = normalize(-(directional_lights[0].direction));
        highp float NdotL = dot(N, L);
        if(NdotL > 0.f){
            highp vec4 clip_coord = directional_lights[0].ortho_view_matrix * vec4(in_world_position, 1.f);
            highp vec4 ndc_coord = clip_coord / clip_coord.w;
            highp vec2 uv = (ndc_coord.xy / 2.f) + vec2(0.5f, 0.5f);
            highp float closest_depth = texture(directional_shadow_map, uv).r + 0.000075f;
            if(closest_depth >= ndc_coord.z)
                lo += BRDF(N, L, V, f0, BaseColor.xyz, pMetallic, pRoughness) * 
                    directional_lights[0].color * 
                    NdotL * 
                    directional_lights[0].intensity;
        }
    //}

    highp float ambient = 0.1f;
    highp vec3 la = BaseColor.xyz * ambient;

    highp vec3 result_color = lo + la;

    out_final_color = vec4(Uncharted2_filmic(result_color), BaseColor.a);
}