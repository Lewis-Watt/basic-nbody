#ifndef PARTICLE_DATA_H
#define PARTICLE_DATA_H
#include <vector>


class particle_data
{
	public:
		std::vector<double> pos;
		std::vector<double> vel;
		std::vector<double> a;
		std::vector<double> dtop;
		int scount;
		double mass;
		double hr;
		double planet_radius;
		bool deleted;
		bool gravity;
		int id;
};

#endif