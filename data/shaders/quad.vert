#version 420

in vec3 a_position;

out vec2 vTexCoord;

void main() {
	vTexCoord = (a_position.xy+vec2(1,1))/2.0f;
	gl_Position = vec4(a_position,1.0);
}
