#ifndef DEFERREDMODEL_H
#define DEFERREDMODEL_H
#include "commons.hpp"

class DeferredContainer;
class DeferredModel : public GameObject{
	public:
		DeferredModel(const std::string& meshID, const std::string& texID);
		virtual ~DeferredModel();

		virtual void update(float deltaTime);
		void draw() const;

	protected:
		virtual void drawDeferredModel() const;

	private:
		DeferredContainer* renderer;
		Model model;
		std::string tex;
};

#endif // DEFERREDMODEL_H