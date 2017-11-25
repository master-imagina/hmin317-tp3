#version 330

// Based on https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/3.1.materials/3.1.materials.vs

struct Light
{
    vec3 pos;
    vec3 color;
};

uniform vec3 cameraPos;
uniform Light light;

uniform vec4 ka;
uniform vec4 kd;
uniform vec4 ks;
uniform float shininess;

in vec3 worldPos;
in vec3 worldNormal;

out vec4 fragColor;


void main()
{
    vec3 lightDiffuseColor = light.color * 0.5f;
    vec3 lightAmbientColor = lightDiffuseColor * 0.2f;
    vec3 lightSpecularColor = vec3(1.f);

    // ambient
    vec3 ambient = lightAmbientColor * ka.xyz;

    // diffuse
    vec3 norm = normalize(worldNormal);
    vec3 lightDir = normalize(light.pos - worldPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuse = lightDiffuseColor * (diff * kd.xyz);

    // specular
    vec3 viewDir = normalize(cameraPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    vec3 specular = lightSpecularColor * (spec * ks.xyz);

    vec3 result = ambient + diffuse + specular;

    fragColor = vec4(result, 1.0);
}
