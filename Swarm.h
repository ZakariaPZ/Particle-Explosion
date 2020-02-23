#pragma once
#include "particleExp.h"

namespace zak {
	class Swarm
	{
	public:
		static const int NPARTICLES = 5000;
	private:
		Particle*  m_pParticles;
		int lastTime;
	public:
		Swarm();
		virtual ~Swarm();
		const Particle* const getParticles() {
			return m_pParticles;
		}
		void update(int elapsed);
	};
}


