#include "PlayerCamera.hpp"

PlayerCamera::PlayerCamera(const std::string& cameraName, const vec3f& pos) :
	Camera(cameraName, pos) {
	setName(cameraName);
}

void PlayerCamera::update(float deltaTime) {
	//move player camera
	vec3f front = getForward();
	vec3f right = (glm::abs(front) != vec3f(0,1,0)? glm::cross(front,vec3f(0,1,0)) : vec3f(0,0,front.y));
	float vel = 10.0f;
	if(Environment::getKeyboard()->isKeyHeld(Keyboard::S)) pos -= front*deltaTime*vel;
	if(Environment::getKeyboard()->isKeyHeld(Keyboard::W)) pos += front*deltaTime*vel;
	if(Environment::getKeyboard()->isKeyHeld(Keyboard::D)) pos += right*deltaTime*vel;
	if(Environment::getKeyboard()->isKeyHeld(Keyboard::A)) pos -= right*deltaTime*vel;
	vec2f displacement = vec2f(Environment::getMouse()->getMousePosRelative())*0.1f;
	if(Environment::getMouse()->getMousePosRelative() != vec2i(0, 0)) {
		rotateLocal(displacement.y, vec3f(1,0,0));
		rotateGlobal(displacement.x, vec3f(0,1,0));
	}
	Camera::update(deltaTime);
}
