#include "standardairmodel.h"

double calculate_air_density(double altitude)
{
    return pow(10, 0.088136 - 6.05317e-5 * altitude + 1.37264e-11 * pow(altitude, 2));
}
