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
layout(set = 0, binding = 2) uniform sampler2D samplers[];

void main()
{
    vec4 color = texture(samplers[fragMaterialId], fragUV).rgba;
    if (color.a < 0.5f)
        discard;
    outColor = vec4(color.rgb, 1.0);
    //outColor = vec4(gl_FragCoord.www, 1.0);
}