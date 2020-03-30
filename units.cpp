#include <iostream>
#include "units.h"
#include <cmath>
#define _USE_MATH_DEFINES
#include <string>
double au_SI = 1.496e11;
double me_SI = 5.972e24;
double year_SI= 60*60*24*365.25;
double time_SI = year_SI/(2*M_PI);
double mass_SI = 1.99e30;
double grav_const =  1;//1.992e-44);
double time_yr = 1/(2*M_PI);
int tot_particles;
int tot_planets;
std::string particle_input;
std::string planet_input;
double star_mass;
std::string fout;
double sma;
double eccentricity;
double true_anomaly; 
double period;
double part_rot;
double starttime_remove;
int nsteps;
int stepout;

