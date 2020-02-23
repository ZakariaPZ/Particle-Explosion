#include "Swarm.h"

namespace zak {

	Swarm::Swarm() {
		lastTime = 0;
		m_pParticles = new Particle[NPARTICLES];
	}

	Swarm::~Swarm() {
		delete[]m_pParticles;

	}

	void Swarm::update(int elapsed) {

		int interval = elapsed - lastTime;

		for (int i = 0; i < Swarm::NPARTICLES; i++) {
			m_pParticles[i].Particle::update(interval); //or just update - since m_particles is a Particle object, it will use that update method from Particle

		}

		lastTime = elapsed;
	}

}