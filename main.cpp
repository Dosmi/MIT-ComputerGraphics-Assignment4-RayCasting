#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "SceneParser.h"
#include "Image.h"
#include "Camera.h"
#include <string.h>

// using the unordered map for getting command line arguments:
#include <unordered_map>

#include <typeinfo>

using namespace std;

// 8. @TODO debugging function to clamp the depth outside the min/max range:
// ... that is, if depth is between min and max, then get interpolate %
// ... else, if min returns 0.f (pure black),
// ...   and if max - returns 1.f (pure white)
float clampedDepth ( float depthInput, float depthMin , float depthMax)
{
  if (depthInput < depthMin ) return 0.0f;
  if (depthInput > depthMax ) return 1.f;
  else return (depthInput-depthMin) / (depthMax-depthMin);
}

void saveToImage (int width, int height, const char* name,
                  std::vector<Vector3f> &colors)
{
  Image image( width+1, height+1 );
  Vector3f ambient_color = (1.0f,0,0);
  for (int x = 0; x <= width; x++)
  {
    for (int y = 0; y <= height; y++)
    {
      image.SetPixel( y, x, ambient_color );
      image.SetPixel( y, x, colors[(width+1)*x+y] );
    }
  }
  // image.SaveImage(argument_lookup.find("output")->second[0]);
  image.SaveImage(name);
}


#include "bitmap_image.hpp"

int main( int argc, char* argv[] )
{
  // Fill in your implementation here.

  // This loop loops over each of the input arguments.
  // argNum is initialized to 1 because the first (0th)
  // "argument" provided to the program is actually the
  // name of the executable (in our case, "a4").
  std::unordered_map<std::string, std::vector<const char*>> argument_lookup;
  for( int argNum = 1; argNum < argc; ++argNum )
  {
    std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    // if the argument begins with '-', then it is an option identifier
    if (argv[argNum][0] == '-')
    {
      int num_subarguments = ( !(strcmp(argv[argNum],"-input")) \
                               || \
                               !(strcmp(argv[argNum],"-output")) \
                             ) * 1 + \
                             (!strcmp(argv[argNum],"-size")) * 2 + \
                             (!strcmp(argv[argNum],"-depth")) * 3;
      //dbug dvar(num_subarguments) eol;

      std::vector<const char*> subarguments;
      subarguments.reserve(num_subarguments);
      // for each argument, populate its subarguments:
      for( int subargNum = 1; subargNum <= num_subarguments; ++subargNum)
      {
        subarguments.push_back(argv[argNum+subargNum]);
      }
      argument_lookup[std::string(++argv[argNum])] = subarguments;
      argNum += num_subarguments;
    }
  }

  // using ArgMap = std::unordered_map<char*, std::vector<char*> >
  /*
  //dbug "testing the argmap:" eol;
  for (auto  kv : argument_lookup)
  {
    //dbug kv.first eol;
    // for (auto it : kv.second ) const_cast<char*>
    for (auto it : argument_lookup.find(kv.first)->second )
      //dbug " - > " << it eol;
  }
  */

  // 6. @Todo First, parse the scene using SceneParser.
  // argument lookup for "input" contains a vector of size 1 ...
  // ... with the 'scene placne' input file in position [0]:
  //dbug "parsing the scene:" eol;
  SceneParser sp(argument_lookup.find("input")->second[0]);
/*
  //dbug dvar(sp.getGroup()->getGroupSize()) eol;
  //dbug dvar(sp.getBackgroundColor().getprint()) eol;
  //dbug dvar(sp.getCamera()->getCenter().getprint()) eol;
  //dbug dvar(sp.getCamera()->getDirection().getprint()) eol;
  //dbug dvar(sp.getCamera()->getUp().getprint()) eol;
  //dbug dvar(sp.getCamera()->getHorizontal().getprint()) eol;
*/
  // //dbug dvar(sp.getCamera()->getFOV()) eol;

  // 7. @Todo ... Then loop over each pixel in the image,
  float image_plane_width  = std::stoi(argument_lookup.find("size")->second[0]);
  float image_plane_height = std::stoi(argument_lookup.find("size")->second[1])       ;

  // for all pixels in the image plane, store the hit information:
  std::vector<Hit> pixel_hits;
  std::vector<Vector3f> pixel_colors;
  std::vector<Vector3f> pixel_depth;
  pixel_hits.reserve(image_plane_width*image_plane_height);
  pixel_colors.reserve(image_plane_width*image_plane_height);

  int hit_counter = 0;

  const char* output_img_name = argument_lookup.find("output")->second[0];

  for (int x = 0; x <= image_plane_width; x++)
  {
    for (int y = 0; y <= image_plane_height; y++)
    {
      //dbug x << " " << y eol;

      PerspectiveCamera* scene_camera = (PerspectiveCamera*)sp.getCamera();
      // image plane is -1 to 1 from left to right, up to botton (2 units)
      Ray r = scene_camera->generateRay( Vector2f(-1 + (2/image_plane_width*x ),
                                                  -1 + (2/image_plane_height*y)) );

      pixel_hits.emplace_back(Hit()); // put empty pixel

      // Write the color at the intersection to that pixel ...
      if (sp.getGroup()->intersect(/* ray */ r,
                                   /* hit */ pixel_hits[(image_plane_width+1)*x+y],
                                   /* tmin*/ 0.01 ))
        {
          pixel_colors.emplace_back(Vector3f(1.));
          hit_counter++;
        }
        else pixel_colors.emplace_back(Vector3f(0.));

      // if the user wants depth output as an image:
      if (argument_lookup.find("depth") != argument_lookup.end() )
      {
        pixel_depth.reserve(image_plane_width*image_plane_height);
        output_img_name = argument_lookup.find("depth")->second[2];
        // Write the color at the intersection to that pixel ...
        float depth = sp.getGroup()->intersect_with_depth(
          /* ray */ r,
          /* hit */ pixel_hits[(image_plane_width+1)*x+y],
          /* tmin*/ 0.01 );

        float depth_percent = clampedDepth(
          /* depthInput */ depth,
          /* depthMin */   std::stof(argument_lookup.find("depth")->second[0]),
          /* depthMax */   std::stof(argument_lookup.find("depth")->second[1]));
        if (depth > 0.01 && depth < FLT_MAX)
        {
          // dbug dvar(depth) eol;
          pixel_depth.emplace_back(Vector3f(depth_percent));
          // hit_counter++;
        }
        else pixel_depth.emplace_back(Vector3f(0.));
      }

    }
  }

  saveToImage (/* width  */ image_plane_width,
               /* height */ image_plane_height,
               /* name   */ argument_lookup.find("output")->second[0],
               /* colors */ pixel_colors);

  // if depth information also requested, save that as a separate image too:
  if (argument_lookup.find("depth") != argument_lookup.end() )
  {
    saveToImage (/* width  */ image_plane_width,
                 /* height */ image_plane_height,
                 /* name   */ argument_lookup.find("depth")->second[2],
                 /* colors */ pixel_depth);
  }

  std::cout << hit_counter << '/' << image_plane_width*image_plane_height << '\n';

  // ... shooting a ray through that pixel

  // ... and finding its intersection with the scene.

  ///TODO: below demonstrates how to use the provided Image class
  ///Should be removed when you start
  // Vector3f pixelColor (1.0f,0,0);
  // ///width and height
  // Image image_test( 10 , 15 );
  // image_test.SetPixel( 5,5, pixelColor );
  // image_test.SaveImage("demo.bmp");

  // make output image from collected colors:
  /*
  Image image( image_plane_width+1, image_plane_height+1 );
  for (int x = 0; x <= image_plane_width; x++)
  {
    for (int y = 0; y <= image_plane_height; y++)
    {
      image.SetPixel( y, x, pixelColor );
      image.SetPixel( y, x, pixel_colors[(image_plane_width+1)*x+y] );
    }
  }
  // image.SaveImage(argument_lookup.find("output")->second[0]);
  image.SaveImage(output_img_name);
*/
  return 0;
}
