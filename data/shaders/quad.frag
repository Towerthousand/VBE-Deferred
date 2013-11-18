#version 420

uniform sampler2D tex;

in vec2 v_texCoord;

out vec4 finalColor;

void main(void) {
	finalColor = texture(tex, v_texCoord);
}
