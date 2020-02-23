#pragma once

namespace zak {
	class Particle
	{
	public: //efficient to not encapsulate - just allow access without getters
		double m_x;
		double m_y; //we can move it bit by bit vs. moving it by an entire pixel if we used int
				//we will round of the double to the nearest pixel
				//not const because we want to change them
	private:
		double m_speed;
		double m_direction;
	public:
		Particle();
		virtual ~Particle();
		void update(int interval);
	private:
		void init();
	};

} 