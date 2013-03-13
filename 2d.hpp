//2D Graphics Header
//	Created By:		Mike Moss
//	Modified On:	03/12/2013

//Required Libraries:
//	gl
//	glew
//	glu
//	glui
//	glut/freeglut
//	soil

//Begin Define Guards
#ifndef MSL_2D_H
#define MSL_2D_H

//2D Utilities Header
#include "2d_util.hpp"

//C Standard Library Header
#include <cstdlib>

//Glut Input Header
#include "glut_input.hpp"

//Glut UI Header
#include "glut_ui.hpp"

//Font Header
#include "font.hpp"

//Sprite Header
#include "sprite.hpp"

//String Header
#include <string>

//String Utility Header
#include "string_util.hpp"

//Externs
extern void setup();
extern void loop(const double dt);
extern void draw();

//MSL Namespace
namespace msl
{
	//Input Variables
	extern double view_width;
	extern double view_height;
	extern double window_width;
	extern double window_height;

	//2D Start Function
	void start_2d(const std::string& window_title="",const int window_width=640,const int window_height=480,const bool window_scale=true,
		const msl::color& color=msl::color(0.5,0.5,0.5,1),int argc=0,char** argv=NULL);

	//2D Stop Function
	void stop_2d();

	//Basic Shape Drawing Functions
	void draw_point(const double x,const double y,const msl::color& color=msl::color(1,1,1,1));
	void draw_line(const double x1,const double y1,const double x2,const double y2,const msl::color& color=msl::color(1,1,1,1));
	void draw_triangle(const double x1,const double y1,const double x2,const double y2,const double x3,const double y3,
		const msl::color& color=msl::color(1,1,1,1));
	void draw_quad(const double x1,const double y1,const double x2,const double y2,const double x3,const double y3,
		const double x4,const double y4,const msl::color& color=msl::color(1,1,1,1));
	void draw_circle(const double x,const double y,const double radius,const msl::color& color=msl::color(1,1,1,1));

	//Text Drawing Function
	void draw_text(const double x,const double y,const std::string& text,const msl::color& color=msl::color(1,1,1,1));
}

//End Define Guards
#endif