#pragma once
class Vector3D
{
private:
	float x, y, z;
public:
	Vector3D() { x = 0; y = 0; z = 0; };
	virtual ~Vector3D() = default;
	Vector3D(float x, float y, float z);
	Vector3D normalize();
	float modulo();
	float productoEscalar(const Vector3D& a);
	Vector3D numeroPorVec(float n);

	//getters
	float getX() const { return x; };
	float getY() const { return y; };
	float getZ() const { return z; };

	//setters
	void setX(float newX) { x = newX; }
	void setY(float newY) { y = newY; }
	void setZ(float newZ) { z = newZ; }

	//operadores
	//suma
	Vector3D operator+(const Vector3D a) const{
		return { x + a.x, y + a.y, z + a.z };
	}
	Vector3D operator+(float a) const{
		return { x + a, y + a, z + a };
	}
	//resta
	Vector3D operator-(const Vector3D a) const{
		return { x - a.x, y - a.y, z - a.z };
	}
	//mult
	Vector3D operator*(float n) const {
		return { x * n, y * n, z * n };
	}
	float operator* (const Vector3D& a) const {
		return a.x * x + a.y * y + a.z * z;
	}
	//Div
	Vector3D operator/(float s) const {
		return Vector3D(x / s, y / s, z / s);
	}
	//asignacion
	void operator=(const Vector3D& a) {
		x = a.x;
		y = a.y;
		z = a.z;
	}

	Vector3D cross(const Vector3D& a) const {
		return Vector3D(
			y * a.z - z * a.y,
			z * a.x - x * a.z,
			x * a.y - y * a.x
		);
	}


};

