#version 460
#extension GL_EXT_nonuniform_qualifier : require
layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragUV;
layout(location = 2) in vec3 fragPos;
layout(location = 3) in vec3 fragLightPos;
layout(location = 4) in flat uint fragMaterialId;
layout(location = 0) out vec4 outColor;

struct Material
{
    uint diffuseTexture;
    uint specularTexture;
    float shininess;
};

layout(set = 0, binding = 2) readonly buffer MaterialBuffer
{
    Material materials[];
};

layout(set = 0, binding = 3) uniform sampler2D samplers[];

const vec3 lightColor = vec3(1.0, 1.0, 1.0);

void main()
{
    vec3 dTexture = texture(samplers[materials[fragMaterialId].diffuseTexture], fragUV * 4).rgb;
    vec3 sTexture = texture(samplers[materials[fragMaterialId].specularTexture], fragUV * 4).rgb;
    vec3 ambient = lightColor * dTexture;

    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(fragLightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * diff * dTexture;

    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materials[fragMaterialId].shininess);
    vec3 specular = spec * lightColor * sTexture;

    vec3 result = (ambient + diffuse + specular);
    outColor = vec4(result, 1.0);
}