#version 420

uniform sampler2D diffuseTex;

in vec2 v_texCoord;
in vec3 v_normal;

layout(location = 0) out vec4 colorOut;
layout(location = 1) out vec4 normalOut;

void main(void) {
	normalOut = vec4(v_normal,1.0f);
        colorOut = texture(diffuseTex,v_texCoord);
}
