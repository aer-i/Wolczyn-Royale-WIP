#version 460
#extension GL_EXT_nonuniform_qualifier : require
layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragUV;
layout(location = 2) in flat uint fragMaterialId;
layout(location = 0) out vec4 outColor;

struct Material
{
    uint diffuseTexture;
    uint specularTexture;
    float shininess;
};

// To rework
//layout(set = 0, binding = 2) readonly buffer MaterialBuffer
//{
//    Material materials[];
//};

// To rework
//layout(set = 0, binding = 3) uniform sampler2D samplers[];

void main()
{
//    vec3 r = fragNormal;
//    vec3 r2 = vec3(r.x, r.y, r.z + 1.0);
//    float m = 2.0 * length(r2);
//    vec3 vN = r.xyz / m + 0.5;
//    outColor = vec4(texture(samplers[materials[fragMaterialId].diffuseTexture], fragUV).rgb * vN, 1.0);
    //outColor = vec4(texture(samplers[materials[fragMaterialId].diffuseTexture], fragUV).rgb, 1.0);
    outColor = vec4(fragNormal.xyz, 1.0);
}