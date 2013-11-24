#version 420

uniform sampler2D diffuseTex;
uniform float ambient;
uniform float specular;

in vec2 v_texCoord;
in vec3 v_normal;

layout(location = 0) out vec3 color0;
layout(location = 1) out vec4 color1;

void main(void) {
        color0 = texture(diffuseTex,v_texCoord).xyz;
        color1 = vec4(v_normal.xy, ambient, specular);
}
