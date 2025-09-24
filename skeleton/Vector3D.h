#pragma once
class Vector3D
{
private:
	int x, y, z;
public:
	Vector3D() {};
	~Vector3D() {};
	Vector3D(int x, int y, int z);
	void normalize(Vector3D v);
	int modulo(const Vector3D v);
	int productoEscalar(const Vector3D a, const Vector3D b);
	Vector3D numeroPorVec(int n, Vector3D a);

	//operadores
	//suma
	Vector3D operator+(const Vector3D a) {
		return { x + a.x, y + a.y, z + a.z };
	}
	//resta
	Vector3D operator-(const Vector3D a) {
		return { x - a.x, y - a.y, z - a.z };
	}
	//mult
	Vector3D operator*(const Vector3D a) {
		return { x * a.x, y * a.y, z * a.z };
	}
	//asignacion
	void operator=(const Vector3D& a) {
		x = a.x;
		y = a.y;
		z = a.z;
	}
};

