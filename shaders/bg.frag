#version 460

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec3 fragLightPos;
layout(location = 3) in flat uint fragMaterialId;
layout(location = 0) out vec4 outColor;

struct Material
{
    vec3 ambient;
    float shininess;
    vec3 diffuse;
    vec3 specular;
};

layout(std140, set = 0, binding = 2) readonly buffer MaterialBuffer
{
    Material materials[];
};

const vec3 lightColor = vec3(1.0, 1.0, 1.0);

void main()
{
    vec3 ambient = materials[fragMaterialId].ambient * lightColor;

    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(fragLightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * materials[fragMaterialId].diffuse) * lightColor;

    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materials[fragMaterialId].shininess);
    vec3 specular = (materials[fragMaterialId].specular * spec) * lightColor;

    vec3 result = (ambient + diffuse + specular);
    outColor = vec4(result, 1.0);
}