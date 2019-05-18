#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <sstream>
#include <cstring>
#include <iomanip>
#include <algorithm>
#include "units.h"
#include "step.h"
#include "open_data.h"
#include "init_particle.h"
#include "load.h"

/* TO DO:
1) Sort out units - Partially done
2) Add a second particle - done
3) Add gravity to second particle from first - done but will need to redo for general
   form.
4) Add parameter file inserts for grav and non-grav particle initial conditions - done
5) Figure out how to parallelise - Is not needed, but may be desired in the future
6) Add radiative force to particles
7) Added multiple gravitating bodies to the code
   -need to add gravity betweem them. DONE
8) Change initial variables to r,rp,e and true anomaly then resolve for x,y,z
   vx,vy,vz. Kinda Done - need to add them to the parameter file input
9)Need to orientate the inputted data

g++ -std=c++14 main.cpp step.cpp init_particle.cpp load.cpp units.cpp particle_data.cpp open_data.cpp -o nufc

*/



int main(int argc, char* argv[])
{
	std::string param_file_name = argv[1];
	parameter_readin(param_file_name);
	/*
	This is where the particles are set up initial conditions. Will be replaced by
	3 files - a parameter file, particle data file, planet data file
	*/

	std::vector<particle_data*> test2 (tot_planets);
	particle_data **planets = test2.data();
	init_particle(planets,tot_planets);
	std::vector<particle_data*> test (tot_particles);
	particle_data **particles = test.data();
	init_particle(particles,tot_particles);

	for (int u=0;u<tot_planets;u++)
	{
		if (u==0)
		{
			initial_orbit_velocity(planets[u]);
		}
		else
			planets[u]->vel[0] = planets[0]->vel[0];
			planets[u]->vel[1] = planets[0]->vel[1];
			planets[u]->vel[2] = planets[0]->vel[2];
	}
	std::cout << "Passes vel set up" << std::endl;



	for (int u=0; u<tot_particles;u++)
	{
		particles[u]->pos[0] = planets[0]->pos[0];
		particles[u]->pos[1] = planets[0]->pos[1];
		particles[u]->pos[2] = planets[0]->pos[2];
		particles[u]->vel[0] = planets[0]->vel[0];
		particles[u]->vel[1] = planets[0]->vel[1];
		particles[u]->vel[2] = planets[0]->vel[2];
		particles[u]->mass = 0;
		particles[u]->gravity = true; //change back to true also change step.cpp back to normal
	}
	opendata(planets,planet_input);
	opendata(particles,particle_input);
	double min_val;
	std::cout << "Planet Mass: " << planets[0]->mass << std::endl;

	// Compares distance from planet to each particle and picks out the min
	for (int u=0; u<tot_particles;u++)
	{

		if (u==0)
		{
			min_val = (std::sqrt( ((particles[u]->pos[0]-planets[0]->pos[0])*(particles[u]->pos[0]-planets[0]->pos[0]))
				               +((particles[u]->pos[1]-planets[0]->pos[1])*(particles[u]->pos[1]-planets[0]->pos[1]))
				               +((particles[u]->pos[2]-planets[0]->pos[2])*(particles[u]->pos[2]-planets[0]->pos[2]))));
			continue;
		}
		double rad_ptop = (std::sqrt( ((particles[u]->pos[0]-planets[0]->pos[0])*(particles[u]->pos[0]-planets[0]->pos[0]))
				               +((particles[u]->pos[1]-planets[0]->pos[1])*(particles[u]->pos[1]-planets[0]->pos[1]))
				               +((particles[u]->pos[2]-planets[0]->pos[2])*(particles[u]->pos[2]-planets[0]->pos[2]))));
		min_val = std::min(rad_ptop,min_val);
	}

	double step_time = period/200000;//2*M_PI*std::sqrt(min_val*min_val*min_val/(grav_const*planets[0]->mass))/100;
	//std::cout << (step_time/(step_size*period)) << std::endl;
	std::ofstream output_file;
	nsteps = 5*(period/step_time) + 1;
	std::cout << "Number of steps:" <<nsteps << std::endl;
	stepout = 0.01*period/step_time;
	std::cout << "Output every " << stepout << "steps" <<std::endl;
	int file_count = 0;
	for (int j=0; j<nsteps; j++)
	{

		if (j!=0)
		{
			step(particles,j,step_time,planets);
			step(planets,j,step_time,planets);
		}
		if ((j%stepout==0) || (j==nsteps-1))
		{
			output_file.open (fout+"_"+std::to_string(file_count)+".txt");
			for (int g=0; g<tot_particles; g++)
			{
				output_file << std::scientific << std::setw(11) << std::setprecision(6) << particles[g]->pos[0] << "\t";
				output_file << std::scientific << std::setw(11) << std::setprecision(6) << particles[g]->pos[1] << "\t";
				output_file << std::scientific << std::setw(11) << std::setprecision(6) << particles[g]->pos[2] << "\t";
				output_file << std::scientific << std::setw(11) << std::setprecision(6) << particles[g]->vel[0] << "\t";
				output_file << std::scientific << std::setw(11) << std::setprecision(6) << particles[g]->vel[1] << "\t";
				output_file << std::scientific << std::setw(11) << std::setprecision(6) << particles[g]->vel[2] << "\t";
				
				output_file << std::endl;
			}
			for (int g=0;g<tot_planets; g++)
			{
				output_file << std::scientific << std::setw(11) << std::setprecision(6) << planets[g]->pos[0] << "\t";
				output_file << std::scientific << std::setw(11) << std::setprecision(6) << planets[g]->pos[1] << "\t";
				output_file << std::scientific << std::setw(11) << std::setprecision(6) << planets[g]->pos[2] << "\t";
				output_file << std::scientific << std::setw(11) << std::setprecision(6) << planets[g]->vel[0] << "\t";
				output_file << std::scientific << std::setw(11) << std::setprecision(6) << planets[g]->vel[1] << "\t";
				output_file << std::scientific << std::setw(11) << std::setprecision(6) << planets[g]->vel[2] << "\t";
				output_file << std::endl;
			}
			output_file.close();
			file_count++;


		}
		
	}

	return 0;
}

