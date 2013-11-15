#ifndef DEFERREDMODEL_H
#define DEFERREDMODEL_H
#include "commons.hpp"

class DeferredContainer;
class DeferredModel : public GameObject{
	public:
		DeferredModel();
		virtual ~DeferredModel();

		virtual void update(float deltaTime);
		void draw() const;

	protected:
		virtual void drawDeferred() const;
};

#endif // DEFERREDMODEL_H
