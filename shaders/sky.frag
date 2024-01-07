#version 460

layout(push_constant) uniform PushConstant
{
    vec2 mousePos;
    vec2 resolution;
} pc;

layout(location = 0) out vec4 outColor;

const float verticalFieldOfView = 50.0 * 3.1415927 / 180.0;
const float sens = 0.7;

void main()
{
    float yaw = pc.mousePos.y * sens;
    float pitch = pc.mousePos.x * sens;

    vec3 rd = normalize(vec3(gl_FragCoord.xy - pc.resolution.xy / 2.0, pc.resolution.y * 0.5 / -tan(verticalFieldOfView * 0.5)));

    mat3 yawMatrix = mat3(cos(yaw), 0.0, -sin(yaw), 0.0, 1.0, 0.0,  sin(yaw), 0.0, cos(yaw));
    mat3 pitchMatrix = mat3(1.0, 0.0, 0.0, 0.0, cos(pitch), sin(pitch), 0.0, -sin(pitch), cos(pitch));

    rd = yawMatrix * pitchMatrix * rd;

    vec3 col;

    if (rd.y > 0.0)
    {
        col = vec3(0.10, 0.25, 0.5) * (1.0 - 0.8 * rd.y) * 0.9;
    }
    else
    {
        col = vec3(0.3, 0.55, 0.8) * (1.0 - 0.8 * rd.y) * 0.9;
    }

    col = mix(col, vec3(0.7, 0.75, 0.8), pow(1.0 - max(abs(rd.y), 0.0), 8.0));
    col = pow(col, vec3(0.4545));
    outColor = vec4(col, 1.0);
}