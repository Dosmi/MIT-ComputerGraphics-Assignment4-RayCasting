#ifndef PLANE_H
#define PLANE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>
using namespace std;
///TODO: Implement Plane representing an infinite plane
///choose your representation , add more fields and fill in the functions
class Plane: public Object3D
{
public:
	Plane(){}
	Plane( const Vector3f& normal , float d , Material* m):Object3D(m){
	}
	~Plane(){}
	virtual bool intersect( const Ray& r , Hit& h , float tmin){

	}
	virtual float intersect_with_depth( const Ray& r , Hit& h, float tmin)
	{
		std::cout << "in plane iwd\n";
	}

protected:
};
#endif //PLANE_H
