#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

void main()
{
    FragColor = vec4(0.7,0.7,0.3,1);
}
