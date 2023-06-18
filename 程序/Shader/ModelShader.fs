#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material {
    vec3 ambient;
    sampler2D diffuseTexture;
    vec3 diffuse;
    sampler2D specularTexture;
    vec3 specular;
    float shininess;
}; 

struct Light {
    vec3 position;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
    // ��ȡ������ɫ
    vec3 diffuseColor = texture(material.diffuseTexture, TexCoords).rgb;

    // ���㷨�ߵĵ�λ����
    vec3 norm = normalize(Normal);

    // �������߷���
    vec3 viewDir = normalize(viewPos - FragPos);

    // ���㻷������
    vec3 ambient = light.ambient * material.ambient;

    // �������������
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = mix(light.diffuse * diff, material.diffuse,0.5);

    // ���㾵�����
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specularColor = texture(material.specularTexture, TexCoords).rgb;
    vec3 specular = light.specular * spec * specularColor;

    // �������˥��
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // �������յ�Ƭ����ɫ
    vec3 result = (ambient + diffuse * attenuation) * diffuseColor + specular * attenuation;
    FragColor = vec4(result, 1.0);
}


