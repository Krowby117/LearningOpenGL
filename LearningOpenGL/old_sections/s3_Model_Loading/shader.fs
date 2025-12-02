#version 330 core
// LIGHTNING FRAGMENT SHADER FOR SECTION 3
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

struct DirLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// uniforms
uniform vec3 viewPos;
uniform DirLight light;
uniform float shininess;

void main()
{
    // general calculations / variables
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0);
    
    vec3 lightDir = normalize(-light.direction);
    
    // ambient calcs
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    
    // diffuse calcs
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
    
    // specular calcs
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
    
    // return the final directional light
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
