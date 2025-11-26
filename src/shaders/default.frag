#version 330 core

out vec4 FragColor;

in vec3 TexCoords;
in vec3 Normal;
in vec3 SunNormal;
in vec3 FragCoord;
flat in int passSkyBox;  
uniform samplerCube skybox;

void main() {
    if (passSkyBox == 1) {
        FragColor = texture(skybox, TexCoords);
    } else {
        
        vec3 N = normalize(Normal);
        vec3 L = normalize(SunNormal);   
        vec3 V = normalize(-FragCoord);  
        vec3 H = normalize(L + V);       

        
        float ambientStrength  = 0.15;
        float diffuseStrength  = 1.0;
        float specularStrength = 0.6;
        float shininess        = 12.0;

        
        vec3 baseColor = vec3(0.21, 1.0, 0.25);

        
        vec3 ambient = ambientStrength * baseColor;

        
        float diff = max(dot(N, L), 0.0);
        vec3 diffuse = diffuseStrength * diff * baseColor;

        
        float spec = pow(max(dot(N, H), 0.0), shininess);
        vec3 specular = specularStrength * spec * vec3(1.0); 

        
        vec3 finalColor = ambient + diffuse + specular;

        FragColor = vec4(finalColor, 1.0);
    }
}
