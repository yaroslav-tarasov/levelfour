#include "phyllo_layout3D.h"
#include <cmath>

//For the generic phyllotree layout code, see phyllo_layout3D.h
namespace graphdefs {
	float convertPolarToX(float phi, float radius)
	{
		return radius * std::cos(phi);
	}

	float convertPolarToY(float phi, float radius)
	{
		return radius * std::sin(phi);
	}
} // end namespace graphdefs