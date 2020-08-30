#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "Ray.h"
#include "Hit.h"
#include "texture.hpp"

#include <iostream>

// additional includes:
#include "Light.h"

///@11. TODO: Implement Shade function that uses ambient, diffuse,
// ... @15. TODO specular and @16. texture
class Material
{
public:

 Material( const Vector3f& d_color,
	 				 const Vector3f& s_color=Vector3f::ZERO,
					 float s=0):
  ambientColor(d_color/2), diffuseColor( d_color),specularColor(s_color), shininess(s)
  {}

  // for artistic reasons, one might specify a different ambient color than diffuse:
  Material( const Vector3f& d_color,
            const Vector3f& a_color,
 	 				  const Vector3f& s_color=Vector3f::ZERO,
 					  float s=0):
   ambientColor(a_color), diffuseColor( d_color),specularColor(s_color), shininess(s)
   {}

  virtual ~Material()
  {

  }

  virtual Vector3f getAmbientColor() const
  {
    return  ambientColor;
  }

  virtual Vector3f getDiffuseColor() const
  {
    return  diffuseColor;
  }

  Vector3f ambientComponent()
  {

  }

  // @11
  Vector3f diffuseComponent()
  {

  }

  // @15
  Vector3f specularComponent()
  {

  }

  Vector3f Shade( const Ray& ray, const Hit& hit,
                  const Vector3f& direction_to_light,
									const Vector3f& light_color )
	{
    dbug "hey from material" eol;
    std::cin.get();

    return Vector3f(1,1,1) ;
  }

  // Vector3f Shade( const Ray& ray, const Hit& hit,
  //                 Light* scene_lights )
  // {
  //
  //
  //
  //   dbug "hey from material" eol;
  //   std::cin.get();
  //
  //   return Vector3f(1,1,1) ;
  // }

  void loadTexture(const char * filename)
	{
    t.load(filename);
  }
 protected:
  Vector3f ambientColor; // by default, ambientColor == diffuseColor
  Vector3f diffuseColor;
  Vector3f specularColor;
  float shininess;
  Texture t;
};



#endif // MATERIAL_H
