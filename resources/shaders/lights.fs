#version 330 core
#define POINT_LIGHTS_NUM 6

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;

    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[POINT_LIGHTS_NUM];
uniform SpotLight spotLight;
uniform Material material;
uniform bool blinn;
uniform bool flashLight;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // phase 1: directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: point light
    for (int i = 0; i < POINT_LIGHTS_NUM; i++) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }
    // phase 3: spot light
    if (flashLight) {
        result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    }

    // check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) {
        BrightColor = vec4(result, 1.0);
    } else {
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }

    FragColor = vec4(result, 1.0);
}

// calculates the 8color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = 0.0f;
    if (blinn) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    } else {
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }

    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0f;
    if (blinn) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    } else {
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float spec = 0.0f;
    if (blinn) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    } else {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}