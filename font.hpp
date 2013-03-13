//2D Font Header
//	Created By:		Mike Moss
//	Modified On:	02/26/2013

//This requires GLEW,GLUT, and libSOIL to work!

//Begin Define Guards
#ifndef MSL_FONT_H
#define MSL_FONT_H

//2d Utility Header
#include "2d_util.hpp"

//Sprite Header
#include "sprite.hpp"

//String Header
#include <string>

//MSL Namespace
namespace msl
{
	//Alignment Enum
	enum halign{LEFT,CENTER,RIGHT};
	enum valign{TOP,MIDDLE,BOTTOM};

	//Font Class Decalaration
	class font
	{
		public:
			//Constructor (Default, String Filename)
			font(const std::string& filename="");

			//Draw Function
			void draw(const double x,const double y,const std::string& text,const halign& h_align=msl::LEFT,
				const valign& v_align=msl::TOP,const msl::color& color=msl::color(1,1,1,1)) const;

		private:
			//Member Variables
			msl::sprite _sprite;
	};
}

//End Define Guards
#endif
