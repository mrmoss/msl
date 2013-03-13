//2D Sprite Header
//	Created By:		Mike Moss
//	Modified On:	02/26/2013

//This requires GLEW,GLUT, and libSOIL to work!

//Begin Define Guards
#ifndef MSL_SPRITE_H
#define MSL_SPRITE_H

//2D Utilities Header
#include "2d_util.hpp"

//Map Header
#include <map>

//String Header
#include <string>

//MSL Namespace
namespace msl
{
	//Sprite Class Decalaration
	class sprite
	{
		public:
			//Constructor (Default, Raw OpenGL Texture)
			sprite(const unsigned int texture=0,const unsigned int number_of_frames=1);

			//Constructor (String Filename)
			sprite(const std::string& filename,const unsigned int number_of_frames=1);

			//Copy Constructor
			sprite(const sprite& copy);

			//Destructor
			~sprite();

			//Copy Assignment Operator
			sprite& operator=(const sprite& copy);

			//Number of Frames Accessor
			unsigned int number_of_frames() const;

			//Dimensions Accessors
			unsigned int width() const;
			unsigned int height() const;

			//Draw Function
			void draw(const double x,const double y,const double rotation=0.0,const unsigned int frame=0,
				const double scale_x=1,const double scale_y=1,
				const msl::color& color=msl::color(1,1,1,1),const bool smooth=true) const;

		private:
			//Add Reference Function
			void add_reference();

			//Remove Reference Function
			void remove_reference();

			//Static Variables
			static std::map<unsigned int,int> _texture_counts;

			//Member Variables
			unsigned int _texture;
			unsigned int _number_of_frames;
			unsigned int _width;
			unsigned int _height;
	};
}

//End Define Guards
#endif
