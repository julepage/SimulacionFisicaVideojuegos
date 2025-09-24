#include "Vector3D.h"
#include <algorithm>

Vector3D::Vector3D(int x2, int y2, int z2)
{
	x = x2; y = y2; z = z2;
}

void Vector3D::normalize(Vector3D v)
{
	int mod = modulo(v);
	v.x = v.x / mod;
	v.y = v.y / mod;
	v.z = v.z / mod;
}

int Vector3D::modulo(const Vector3D v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

int Vector3D::productoEscalar(const Vector3D a, const Vector3D b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3D Vector3D::numeroPorVec(int n, Vector3D a)
{
	return { n * a.x,n * a.y,n * a.z };
}
