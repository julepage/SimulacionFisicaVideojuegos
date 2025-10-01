#include "Vector3D.h"
#include <algorithm>

Vector3D::Vector3D(float x2, float y2, float z2)
{
	x = x2; y = y2; z = z2;
}

void Vector3D::normalize()
{
	float mod = sqrt(x * x + y * y + z * z);
	x = x / mod;
	y = y / mod;
	z = z / mod;
}

float Vector3D::modulo()
{
	return sqrt(x * x + y * y + z * z);
}

float Vector3D::productoEscalar(const Vector3D a)
{
	return a.x * x + a.y * y + a.z * z;
}

Vector3D Vector3D::numeroPorVec(float n) 
{
	return { n * x,n * y,n * z };
}
