#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include "load.h"
#include "units.h"

void parameter_readin(std::string input)
{
	std::ifstream ifs;
	ifs.open(input);
	std::string line;
	std::size_t size1;
	int count = 0;
	while (std::getline(ifs,line))
	{
		if (count == 1)
		{
			particle_input = line;
		}
		if (count == 3)
		{
			planet_input = line;
		}
		if (count == 5)
		{
			star_mass = std::stod(line,&size1);
		}
		if (count == 7)
		{
			fout = line;
		}
		if (count == 9)
		{
			nsteps = std::stod(line,&size1);
		}
		if (count==11)
		{
			part_rot = std::stod(line,&size1)*M_PI;
		}
		if (count==13)
		{
			stepout = std::stod(line,&size1);
		}
		if (count == 15)
		{
			sma = std::stod(line,&size1);
		}
		if (count == 17)
		{
			eccentricity = std::stod(line,&size1);
		}
		if (count == 19)
		{
			true_anomaly = std::stod(line,&size1);
		}
		if (count == 21)
		{
			starttime_remove = std::stod(line,&size1);
		}
		count++;

	}
	ifs.close();
	period = std::sqrt((4*M_PI*M_PI*sma*sma*sma)/(grav_const*star_mass));
	tot_particles = lines_in_txtfile(particle_input);
	tot_planets = lines_in_txtfile(planet_input);
	std::cout<< "Starting Position Properties" << std::endl;
	std::cout << "Semi Major Axis: " << sma << "au" <<std::endl;
	std::cout << "Eccentricity: " << eccentricity << std::endl;
	std::cout << "True Anomaly: " << true_anomaly << std::endl;
	std::cout << "Period: " << period*time_yr << std::endl;

}

int lines_in_txtfile(std::string file)
{
	/*
	Counts the number of lines in a text file. Used for getting the number of
	unbound particles.
	*/ 	
	int count = 0;
	std::ifstream myfile (file);
	std::string line;
	while ( std::getline (myfile,line))
	{
		count++;
	}
	myfile.close();
	return count;
}