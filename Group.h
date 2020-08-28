#ifndef GROUP_H
#define GROUP_H


#include "Object3D.h"
#include "Ray.h"
#include "Hit.h"
#include <iostream>

using  namespace std;

///3. @TODO:
///Implement Group
///Add data structure to store a list of Object*
class Group:public Object3D
{
public:

  Group()
  {}

  Group( int num_objects )
  {
    m_objects.reserve(num_objects);
  }

  ~Group()
  {}

  virtual bool intersect( const Ray& r , Hit& h , float tmin )
  {
    bool hit_object = false;
    for (Object3D* object : m_objects)
    {
      // if successful hit, and previously hadn't hadn't hit anything ...
      // ... then raise the flag such that some object has been hit:
      if ( object->intersect(r, h, tmin) && !hit_object) hit_object = true;
    }
    return hit_object;
  }

  virtual float intersect_with_depth( const Ray& r , Hit& h , float tmin )
  {
    float min_depth = FLT_MAX;
    // dbug dvar(m_objects.size()) eol;
    for (Object3D* object : m_objects)
    {
      // dbug "callign " eol;
      // if successful hit, and previously hadn't hadn't hit anything ...
      // ... then raise the flag such that some object has been hit:
      float depth = object->intersect_with_depth(r, h, tmin);
      // dbug dvar(depth) eol;
      if ( depth != tmin && depth < min_depth)
      {
        min_depth = depth;
        // dbug dvar(depth) eol;
      }
    }
    // dbug "returning " << dvar(min_depth) eol;
    return min_depth;
  }

  void addObject( int index , Object3D* obj )
  {
    //dbug "adding object" eol;
    m_objects.push_back(obj);
  }

  int getGroupSize()
  {
    return m_objects.size();
  }

 private:
   std::vector<Object3D*> m_objects;
};

#endif
