#version 420

uniform mat4 MVP;

in vec3 a_position;

out vec2 vTexCoord;

void main() {
    vec4 screenPos = MVP * vec4(a_position, 1.0);
    vTexCoord = (screenPos.xy/screenPos.z + 1) * 0.5f;
    gl_Position = screenPos;
}
