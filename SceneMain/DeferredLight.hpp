#ifndef DEFERREDLIGHT_HPP
#define DEFERREDLIGHT_HPP
#include "commons.hpp"

class DeferredContainer;
class DeferredLight : public GameObject{
	public:
		DeferredLight();
		virtual ~DeferredLight();

		virtual void update(float deltaTime);
		void draw() const;

	protected:
		virtual void drawDeferredLight() const;

	private:
		DeferredContainer* renderer;
};

#endif // DEFERREDLIGHT_HPP
