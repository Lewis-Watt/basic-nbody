#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <map>
#include "units.h"
#include "open_data.h"
#include "step.h"


void opendata(particle_data **r,std::string file)
{
	std::cout << file << std::endl;
	std::ifstream myfile (file);
	std::string line;

	if (myfile.is_open())
	{
		int j = 0;
		while ( std::getline (myfile,line))
		{
			std::stringstream tokenizer(line);
			double h;
			std::vector<double> n;
			while(tokenizer >> h)
			{

				n.push_back(h);

			}
			double sa = std::acos((r[j]->pos[0]+(sma*eccentricity))/sma);
			double l1 = std::sin(sa)/(sma*std::sqrt(1-(eccentricity*eccentricity)));
			double l2 = std::cos(sa)/(sma);
			double phi = std::acos(-l1*(1/std::sqrt((l1*l1) + (l2*l2))));
			if ((std::fmod(sa,M_PI)) > (M_PI/2))
			{
				phi = -phi;
			}
			if (r[j]->pos[1] < 0)
			{
				phi = M_PI - phi;
			}
			
			r[j]->pos[0] += (n[0]*std::cos(phi))-(n[1]*std::sin(phi));

			r[j]->pos[1] += (n[0]*std::sin(phi))+(n[1]*std::cos(phi));

			r[j]->pos[2] += n[2];
			r[j]->vel[0] +=  (n[3]*std::cos(phi)) - (n[4]*std::sin(phi)); //n[3];
			r[j]->vel[1] += (n[3]*std::sin(phi)) + (n[4]*std::cos(phi));//n[4];
			r[j]->vel[2] += n[5];
			r[j]->mass = n[6]*me;
			j++;
		}
		myfile.close();
	}
	else std::cout << "Unable to open file \n";

}
