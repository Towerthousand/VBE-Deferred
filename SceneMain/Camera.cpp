#include "Camera.hpp"

Camera::Camera(const std::string& cameraName, const vec3f& pos, const vec3f& rot)
	: pos(pos), rot(rot), projection(1.0f), view(1.0f) {
	this->setName(cameraName);
	this->setUpdatePriority(2);
}

void Camera::update(float deltaTime) {
	(void) deltaTime;
	for(int i = 0; i < 3; ++i) {
		if(rot[i] < 0) rot[i] = rot[i]+360;
		else if(rot[i] >= 360.0f) rot[i] = rot[i]-360;
	}

	view = mat4f(1.0f);
	view = glm::rotate(view, rot.x, vec3f(1, 0, 0));
	view = glm::rotate(view, rot.y, vec3f(0, 1, 0));
	view = glm::rotate(view, rot.z, vec3f(0, 0, 1));
	view = glm::translate(view, -pos);
}
