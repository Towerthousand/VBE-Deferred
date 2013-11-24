#ifndef DEFERREDMODEL_H
#define DEFERREDMODEL_H
#include "commons.hpp"

class DeferredContainer;
class DeferredModel : public GameObject{
	public:
		DeferredModel(const std::string& meshID, const std::string& texID, float ambient = 0.1, float specular = 1.0);
		virtual ~DeferredModel();

		virtual void update(float deltaTime);
		void draw() const;

		vec3f pos;
		vec3f rot;
		vec3f scale;

		float ambient, specular;

	protected:
		virtual void drawDeferredModel() const;

	private:
		DeferredContainer* renderer;
		Model model;
		std::string tex;

};

#endif // DEFERREDMODEL_H
