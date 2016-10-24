
#include "LayoutMollweide.hpp"
#include <cmath>
using namespace IMT;

#define TWO_PI 2*M_PI
Layout::NormalizedFaceInfo LayoutMollweide::From2dToNormalizedFaceInfo(const CoordI& pixel) const
{

	return Layout::NormalizedFaceInfo(CoordF(double(pixel.x) / GetWidth(),double(pixel.y ) / GetHeight()), 0);
}
CoordF LayoutMollweide::FromNormalizedInfoTo2d(const NormalizedFaceInfo& ni) const
{
	return CoordF((ni.m_normalizedFaceCoordinate.x )*GetWidth(), (ni.m_normalizedFaceCoordinate.y) * GetHeight());
}
Layout::NormalizedFaceInfo LayoutMollweide::From3dToNormalizedFaceInfo(const Coord3dSpherical& sphericalCoord) const
{
	Coord3dSpherical rotCoord = Rotation(sphericalCoord, m_rotationMatrice.t());
	return NormalizedFaceInfo(CoordF(0.5 + rotCoord.y / (2.0*PI()), rotCoord.z / PI()), 0);
}
Coord3dCart LayoutMollweide::FromNormalizedInfoTo3d(const NormalizedFaceInfo& ni) const
{
	double lon = (ni.m_normalizedFaceCoordinate.x - 0.5)*2.0*PI();
	double lat = (ni.m_normalizedFaceCoordinate.y)*PI();
	double y = 2*ni.m_normalizedFaceCoordinate.y - 1;//[0,1]-->[-1,1]
	double lambda_c = 0;//lambda_c is the central meridian

    double theta = std::asin(y);
	double arg = (2 * theta + sin(2 * theta)) / M_PI;
	lat = std::asin(arg) + M_PI_2;

	if (std::fabs(theta) == M_PI || std::fabs(theta)== M_PI_2)
		lon = 0;
	else
	{
		double x = 2 * ni.m_normalizedFaceCoordinate.x - 1;
		lon = lambda_c + M_PI * x / cos(theta);
		if (std::fabs(lon) > M_PI) return(Coord3dSpherical(0, 0, 0));
	}
	return Rotation(Coord3dSpherical(1, lon, lat), m_rotationMatrice);
}