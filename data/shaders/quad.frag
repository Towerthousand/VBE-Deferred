#version 420

uniform sampler2D depth;
uniform sampler2D normal;
uniform sampler2D diffuse;

in vec2 v_texCoord;

out vec4 finalColor;

void main(void) {
	if(v_texCoord.y > 2.0f/3.0f) {
		float f=100.0;
		float n = 0.01;
		float z = (2 * n) / (f + n - texture( depth, v_texCoord ).x * (f - n));
		finalColor = vec4(z,z,z,1);
	}
	else if(v_texCoord.y > 1.0f/3.0f) finalColor = texture( normal, v_texCoord);
	else finalColor = texture( diffuse, v_texCoord );
}
