#version 420

uniform sampler2D diffuse;

in vec2 vTexCoord;

out vec4 finalColor;

void main(void) {
    // Ambient light properties
    vec3 ambientLightColor = vec3(1.0f);
    float ambientLightPower = 0.2f;

    // material properties
    vec3 matDiffuseColor = texture(diffuse,vTexCoord).xyz;

    finalColor = vec4(matDiffuseColor*ambientLightColor*ambientLightPower,1.0);
}
