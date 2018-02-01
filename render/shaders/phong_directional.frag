#version 330

// Based on https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/3.1.materials/3.1.materials.vs

struct Light
{
    vec3 pos;
    vec3 direction;
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
    // ambient
    vec3 ambient = vec3(0.2) * ka.rgb;

    // diffuse
    vec3 norm = normalize(worldNormal);
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(0.5) * diff * kd.rgb;

    // specular
    vec3 viewDir = normalize(cameraPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = vec3(1.0) * spec * ks.rgb;

    vec3 result = diffuse + ambient + specular;

    fragColor = vec4(result, 1.0);
}
