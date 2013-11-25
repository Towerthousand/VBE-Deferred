#version 420

uniform sampler2D tex;
uniform vec2 invResolution;

out vec4 finalColor;

void main(void) {
    vec2 vTexCoord = gl_FragCoord.xy*invResolution;
    finalColor = texture(tex,vTexCoord);
}
