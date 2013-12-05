#version 420

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform vec2 invResolution;

out vec4 finalColor;

float lin(vec2 c) {
    float f = 100.0;
    float n = 0.01;
    return (2 * n) / (f + n - texture2D( tex1, c ).x * (f - n));
}

void main(void) {
    vec2 vTexCoord = gl_FragCoord.xy*invResolution;
    finalColor = vec4(texture(tex1,vTexCoord).xyz + texture(tex2,vTexCoord).xyz*0.0001,1.0);
}
