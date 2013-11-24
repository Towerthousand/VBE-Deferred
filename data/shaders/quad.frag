#version 420

uniform sampler2D diffuse;
uniform vec2 invResolution;

out vec4 finalColor;

void main(void) {
    // Ambient light properties
    vec3 ambientLightColor = vec3(1.0f);
    float ambientLightPower = 0.05f;

    // material properties
    vec2 vTexCoord = gl_FragCoord.xy*invResolution;
    vec3 matDiffuseColor = texture(diffuse,vTexCoord).xyz;

    finalColor = vec4(matDiffuseColor*ambientLightColor*ambientLightPower,1.0);
}
