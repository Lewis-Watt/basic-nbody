#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
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
			r[j]->pos[0] += n[0];
			r[j]->pos[1] += n[1];
			r[j]->pos[2] += n[2];
			r[j]->vel[0] += n[3];
			r[j]->vel[1] += n[4];
			r[j]->vel[2] += n[5];
			r[j]->mass = n[6];
			j++;
		}
		myfile.close();
	}
	else std::cout << "Unable to open file \n";

}
