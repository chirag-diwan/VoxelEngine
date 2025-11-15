#version 330 core
out vec4 FragColor;

in vec3 Normal;

// Light coming FROM direction (-1,-1,-1) TOWARD object
// (must be normalized)
const vec3 lightDir = normalize(vec3(1.0, 1.0, 11.0));

void main()
{
    vec3 N = normalize(Normal);

    // Diffuse = max(dot(N, L), 0)
    float diffuse = max(dot(N, lightDir), 0.0);

    vec3 baseColor = vec3(0.3, 0.6, 0.2);
    vec3 lightColor = vec3(0.4);

    vec3 result = baseColor + diffuse * lightColor;

    FragColor = vec4(Normal, 1.0);
}
