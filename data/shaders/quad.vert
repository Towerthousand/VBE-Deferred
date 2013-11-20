#version 420

in vec3 a_position;

out vec2 vTexCoord;

void main() {
		vTexCoord = (a_position.xy+1)*0.5f;
		gl_Position = vec4(a_position,1.0);
}
