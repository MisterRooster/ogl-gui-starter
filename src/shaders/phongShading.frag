out vec4 FragColor;

in vec3 FragPos;
in vec3 Color;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 lightColor;

void main()
{
    vec3 viewPos = vec3(0.0f, 0.0f, 3.0f); 

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = specularStrength * spec * lightColor; 

    vec3 col = (ambient + diffuse + specular) * Color;
    FragColor = vec4(col, 1.0f);
} 