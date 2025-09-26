#pragma once
class Vector3D
{
private:
	float x, y, z;
public:
	Vector3D() { x = 0; y = 0; z = 0; };
	~Vector3D() {};
	Vector3D(float x, float y, float z);
	void normalize();
	float modulo();
	float productoEscalar(const Vector3D a);
	Vector3D numeroPorVec(int n) ;

	//getters
	float getX() { return x; };
	float getY() { return y; };
	float getZ() { return z; };

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

