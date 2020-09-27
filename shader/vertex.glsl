#version 330 core

layout(location = 0) in vec3 mPosition;
layout(location = 1) in vec2 UV;
layout(location = 2) in vec3 mNormal;

out vec3 position;
out vec2 uv;
out vec3 normal;

uniform mat4 mvp;
uniform mat4 model;

void main()
{       
    gl_Position = mvp * vec4(mPosition, 1);
    
    position = vec3(model * vec4(mPosition, 1));
    uv = UV;
    normal = vec3(model * vec4(mNormal, 0));
}