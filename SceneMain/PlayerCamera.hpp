#ifndef PLAYERCAMERA_HPP
#define PLAYERCAMERA_HPP
#include "commons.hpp"

class PlayerCamera : public Camera {
	public:
		PlayerCamera(const std::string &cameraName, const vec3f& pos = vec3f(0.0f));
		void update(float deltaTime);
};

#endif // PLAYERCAMERA_HPP
