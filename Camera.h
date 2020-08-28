#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include <vecmath.h>
#include <float.h>
#include <cmath>

#include <iostream>

/* ========================= BASE CAMERA INTERFACE ========================== */
class Camera
{
public:
	// each derived camera object can generate rays for each 2d screen-space coordinate:
	virtual Ray generateRay( const Vector2f& point ) = 0 ;
	// ... and get the mininum t value of intersection:
	virtual float getTMin() const = 0 ;

	Vector3f getCenter()     { return m_center; }
	Vector3f getDirection()  { return m_direction; }
	Vector3f getUp() 				 { return m_up; }
	Vector3f getHorizontal() { return m_horizontal; }

	virtual ~Camera(){}
protected:
	// We're making this constructor protected because
	// we don't want people creating Camera objects directly,
	// but we still want derived classes to be able to use it:
	Camera(const Vector3f& center,
	/*+-*/ const Vector3f& direction,
	/*-+*/ const Vector3f& up)
	: m_center(center), m_direction(direction), m_up(up)
	{
		//dbug "constructed camera" eol;
		m_horizontal = Vector3f::cross(direction, up);
	}

	Vector3f m_center;
	Vector3f m_direction;
	Vector3f m_up;
	Vector3f m_horizontal;
};
/* ====================== END OF BASE CAMERA INTERFACE ====================== */

/* ======================== PERSPECTIVE CAMERA CLASS ======================== */
/// 5. @TODO: Implement Perspective camera
/// Fill in functions and add more fields if necessary
class PerspectiveCamera: public Camera
{
public:
	/* ============================== constructor ============================== */
	PerspectiveCamera(const Vector3f& center,
	/* - + - + - + */ const Vector3f& direction,
	/* + - + - + - */ const Vector3f& up,
	/* - + - + - + */ float angle)
	// C++ prevents classes from initializing inherited member variables
	// ... in the initialization list of a constructor.
	// C++ gives us the ability to explicitly choose which base class constructor
	// ... to call:
	: Camera (center, direction, up), m_fov(angle)
	{
		//dbug "made perspective camera" eol;
	}
	/* ================================ getters ================================ */
	float getFOV() { return m_fov; }

	virtual Ray generateRay( const Vector2f& point)
	{
		//dbug "generating ray for point: " << point.getprint() eol;

		float distance = 1.f / tan(m_fov/2);

		Vector3f point_on_imageplane = getCenter() +
		                               distance*getDirection() +
																	 point.x()*getHorizontal() +
																	 point.y()*getUp();
		// get the ray direction by subtracting point on imageplane from eye point ...
		// ... and normalise it to get a unit direction:
		Vector3f direction = (point_on_imageplane - getCenter()).normalized();

		return Ray( Camera::getCenter(), direction );
	}

	virtual float getTMin() const
	{ // t minimum is 0.0 to correctly clip objects behind the viewpoint
		return 0.0f;
	}

private:
	// perspective cameras have a field of view:
	float m_fov;
};
/* ==================== END OF PERSPECTIVE CAMERA CLASS ===================== */

#endif //CAMERA_H
