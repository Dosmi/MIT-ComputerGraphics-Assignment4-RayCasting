#ifndef SPHERE_H
#define SPHERE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>

// additional includes:
// #include <math.h>       /* sqrt */

#include <iostream>
using namespace std;
///2. @TODO:
///Implement functions and add more fields as necessary
class Sphere: public Object3D
{
public:
	/* ----------------------------- constructors ------------------------------ */
	// default
	Sphere()
	{
		//unit ball at the center
	}
	// parameterized
	Sphere( Vector3f center , float radius , Material* material )
		: Object3D(material), // call parameterized parent constructor
		  m_center_point(center),
			m_radius(radius)
	{}

	/* ------------------------------ destructor ------------------------------- */
	~Sphere(){}

	// Sphere implements intersect method of pure virtual parent class Object3D
	virtual bool intersect( const Ray& r , Hit& h , float tmin)
	{
		/* Parametric Ray Equation:
		 * 		P(t) = ray_origin + t * ray_direction ...
		 *    ... where ray_origin is where camera shoots it from and ...
		 *    ... ray_direction is normalized direction towards the image plane
		 * Assume a sphere at the origin (can be easily translated)
		 * ... and a distance to the surface H(P)
		 * 		H(P) = ||P||^2 - radius^2 = 0
		 * ... plugging P = ray_origin + t * ray_direction
		 * 		H(P) = (ray_direction^2)*t^2 +
		 * 					 + 2(ray_direction*ray_origin)*t +
		 *					 + (ray_origin^2 - radius) = 0
		 * ... since this takes form of a quadratic equation, rewritten in form:
		 *     a*t^2 + b*t + c = 0
		 * ... where:
		 * 		 a = (ray_direction^2) = 1, since ||ray_direction|| = 1
		 *		 b = 2(ray_direction*ray_origin)*t;
		 *		 c = (ray_origin^2 - radius^2);
		 */
		 /* ----------------------- computing discriminant ------------------------ */
		 float a =  1.f;
		 float b =  2*Vector3f::dot(r.getDirection(), r.getOrigin());
		 float c =  r.getOrigin().absSquared() - pow(2,m_radius);

		 auto discriminant = [](float a, float b, float c) -> float
		 {
			  return (b*b - 4*a*c);
		 } (/* a */ 1.f,
			  /* b */ 2*Vector3f::dot(r.getDirection(), r.getOrigin()),
		    /* c */ r.getOrigin().absSquared() - pow(2,m_radius) ); //<- invoke and return value to root1

		 /* 3 cases:
		  *  - if discriminant > 0, two intersections (ray going through)
			*  - if discriminant = 0, one intersection (ray just touches)
			*  - if discriminant < 0, ray misses the sphere
			*/
		 if (discriminant > 0)
	 	 { // when ray goes through the sphere
		 		float posT = (-b + sqrt(discriminant)) / 2*a;
		 		float negT = (-b - sqrt(discriminant)) / 2*a;
				float t = (abs(posT) > abs(negT)) * posT + (abs(posT) < abs(negT)) * negT;;
				if ( ( t > tmin ) && ( t < h.getT() ) )
				{
					Vector3f intersection = r.getOrigin() + t * r.getDirection();
					Material mat = Material( /*diffuseColor*/ Vector3f(1.));
					h.set(/* t */ t,
						    /* material */ &mat,
								/* normal */ intersection / intersection.abs() );
				}
				return true;
		 }
		 else return false;
	}

	// Sphere implements intersect method of pure virtual parent class Object3D
	virtual float intersect_with_depth( const Ray& r , Hit& h , float tmin)
	{
		/* Parametric Ray Equation:
		 * 		P(t) = ray_origin + t * ray_direction ...
		 *    ... where ray_origin is where camera shoots it from and ...
		 *    ... ray_direction is normalized direction towards the image plane
		 * Assume a sphere at the origin (can be easily translated)
		 * ... and a distance to the surface H(P)
		 * 		H(P) = ||P||^2 - radius^2 = 0
		 * ... plugging P = ray_origin + t * ray_direction
		 * 		H(P) = (ray_direction^2)*t^2 +
		 * 					 + 2(ray_direction*ray_origin)*t +
		 *					 + (ray_origin^2 - radius) = 0
		 * ... since this takes form of a quadratic equation, rewritten in form:
		 *     a*t^2 + b*t + c = 0
		 * ... where:
		 * 		 a = (ray_direction^2) = 1, since ||ray_direction|| = 1
		 *		 b = 2(ray_direction*ray_origin)*t;
		 *		 c = (ray_origin^2 - radius^2);
		 */
		 /* ----------------------- computing discriminant ------------------------ */
		 // dbug "here" eol;
		 float a =  1.f;
		 float b =  2*Vector3f::dot(r.getDirection(), r.getOrigin());
		 float c =  r.getOrigin().absSquared() - pow(2,m_radius);

		 auto discriminant = [](float a, float b, float c) -> float
		 {
			  return (b*b - 4*a*c);
		 } (/* a */ 1.f,
			  /* b */ 2*Vector3f::dot(r.getDirection(), r.getOrigin()),
		    /* c */ r.getOrigin().absSquared() - pow(2,m_radius) ); //<- invoke and return value to root1

		 // dbug dvar(discriminant) eol;

		 /* 3 cases:
		  *  - if discriminant > 0, two intersections (ray going through)
			*  - if discriminant = 0, one intersection (ray just touches)
			*  - if discriminant < 0, ray misses the sphere
			*/
		 if (discriminant > 0)
	 	 { // when ray goes through the sphere
		 		float posT = (-b + sqrt(discriminant)) / 2*a;
		 		float negT = (-b - sqrt(discriminant)) / 2*a;
				float t = (abs(posT) > abs(negT)) * posT + (abs(posT) < abs(negT)) * negT;

				// dbug dvar(t) eol;

				if ( ( t > tmin ) && ( t < h.getT() ) )
				{
					Vector3f intersection = r.getOrigin() + t * r.getDirection();
					Material mat = Material( /*diffuseColor*/ Vector3f(1.));
					h.set(/* t */ t,
						    /* material */ &mat,
								/* normal */ intersection / intersection.abs() );
				}
				return t;
		 }
		 else return tmin;
	}

protected:
	// Sphere additionally stores a center point and a radius
	// (material from parent Object3D)
	Vector3f m_center_point;
	float m_radius;

};


#endif
