#include "LightParticleEmitter.hpp"
#include "ParticleEmitter.hpp"
#include "glm/gtc/random.hpp"

LightParticleEmitter::LightParticleEmitter(vec3f col) : ParticleEmitter(1000), col(col)
{
}

Particle LightParticleEmitter::makeParticle(float frameTimePassed, float deltaTime, vec3f position)
{
	(void) frameTimePassed;
	(void) deltaTime;
	vec3f vel = glm::sphericalRand(3.0f);
	Particle pt;
	pt.life = 0.1;
	pt.startSize =1;
	pt.endSize = 0.5;
	pt.startCol = vec4f(col, 0.7);
	pt.endCol = vec4f(col, 0);
	pt.v = (currWorldPos - oldWorldPos )*-30.0f + vel;
	pt.p = position + pt.v/10.0f;
	pt.a = vec3f(0,1,0);
	pt.texIndex = 1;
	return pt;
}
