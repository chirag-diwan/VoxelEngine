#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aSkyBoxVert;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform int isSkyBox;  
uniform vec3 aSunNormal;


out vec3 FragCoord;
out vec3 TexCoords;
out vec3 Normal;
flat out int passSkyBox;
out vec3 SunNormal;

void main() {
    mat4 model = mat4(1.0);
    SunNormal = aSunNormal;
    passSkyBox = isSkyBox;

    if (isSkyBox == 1) {
        TexCoords = aSkyBoxVert;
        mat4 viewNoTrans = mat4(mat3(ViewMatrix));  
        gl_Position = ProjectionMatrix * viewNoTrans * vec4(aSkyBoxVert, 1.0);
    } else {
        Normal = aNormal;
        gl_Position = ProjectionMatrix * ViewMatrix * vec4(aPos, 1.0);
        FragCoord = vec3(model*vec4(aPos , 1.0));
    }
}
