#ifndef SATURNRINGS_H
#define SATURNRINGS_H


#include "particlesystem.h"

class SaturnRings
{
	private:
		ParticleSystem m_system {};

	public:
		SaturnRings();
		void init();
		void render(Camera &c);
};

#endif // SATURNRINGS_H
