#include <iostream>
#include "units.h"
#include <cmath>
#include <string>
double au = 1.496*std::pow(10,11);
double me = 5.972*std::pow(10,24);
double year= 60*60*24*365.25;
double grav_const =  1.992*std::pow(10,-44);
int tot_particles;
int tot_planets;
std::string particle_input;
std::string planet_input;
std::string star_out;
std::string fout;
double sma;
double eccentricity;
double true_anomaly; 
double period;
double step_size;
int nsteps;
int stepout;
double rper = (1-eccentricity)*sma;