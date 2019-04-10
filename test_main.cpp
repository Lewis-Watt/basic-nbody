#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <sstream>
#include <cstring>
#include "units.h"
#include "step.h"
#include "open_data.h"
#include "init_particle.h"


int main(int argc, char** argv)
{

	std::string arg1 = argv[1];
	std::string arg2 = argv[2];
	std::string arg3 = argv[3];
	std::size_t size1;
	sma = std::stod(arg1,&size1);
	eccentricity = std::stod(arg2,&size1);
	true_anomaly = std::stod(arg3,&size1);
	std::vector<particle_data*> test (1);
	tot_particles= 1;
	particle_data **particles = test.data();
	init_particle(particles,tot_particles);
	//double radius2 = (sma+0.05*(1-(eccentricity*eccentricity)))/(1+(eccentricity*std::cos(true_anomaly)));
	//particles[1]->pos[0] = radius2*std::cos(true_anomaly);
	//particles[1]->pos[1] = radius2*std::sin(true_anomaly);
	period = std::sqrt((4*M_PI*M_PI*sma*sma*sma)/(grav_const*1.989*std::pow(10,30)));
	initial_orbit_velocity(particles[0]);
	//initial_orbit_velocity(particles[1]);
	particles[0]->mass = 1*me;
	particles[0]->gravity=true;
	//particles[1]->mass = 1*me;
	std::ofstream output_file;
	output_file.open ("test_particle.txt");
	for (int j=0; j<300000; j++)
	{
		if (j!=0)
		{
			step(particles,j,0.00005*period,particles);
		}

		output_file << particles[0]->pos[0] << "\t";
		output_file << particles[0]->pos[1] << "\t";
		output_file << particles[0]->pos[2] << "\t";
		output_file << particles[0]->vel[0] << "\t";
		output_file << particles[0]->vel[1] << "\t";
		output_file << particles[0]->vel[2] << "\t";
		/*
		output_file << particles[1]->pos[0] << "\t";
		output_file << particles[1]->pos[1] << "\t";
		output_file << particles[1]->pos[2] << "\t";
		output_file << particles[1]->vel[0] << "\t";
		output_file << particles[1]->vel[1] << "\t";
		output_file << particles[1]->vel[2] << "\t";
		*/
		output_file << std::endl;

	}
	output_file.close();
}