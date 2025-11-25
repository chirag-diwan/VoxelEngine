#version 330 core

out vec4 FragColor;

in vec3 TexCoords;
in vec3 Normal;
in vec3 SunNormal;
flat in int passSkyBox;  
uniform samplerCube skybox;

void main() {
    if (passSkyBox == 1) {
        FragColor = texture(skybox, TexCoords);
    } else {
        vec3 baseColor = vec3(0.21, 1.0, 0.25);
        vec3 N = normalize(Normal);
        float diff = 0.4 + max(dot(N, SunNormal), 0.0);
        vec3 finalColor = baseColor * diff;
        FragColor = vec4(finalColor, 1.0);
    }
}
