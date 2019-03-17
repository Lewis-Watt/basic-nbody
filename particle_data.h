#ifndef PARTICLE_DATA_H
#define PARTICLE_DATA_H
#include <vector>


class particle_data
{
	public:
		std::vector<double> pos;
		std::vector<double> vel;
		std::vector<double> a;
		double mass;
		bool gravity;
		int id;
};

#endif