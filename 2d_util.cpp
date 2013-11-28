//2D Utilities Source
//	Created By:		Mike Moss
//	Modified On:	11/22/2013

//Required Libraries:
//	gl
//	glew
//	glu
//	glut/freeglut
//	soil

//Definitions for "2d_util.hpp"
#include "2d_util.hpp"

//Algorithm Header
#include <algorithm>

//Math Header
#include <math.h>

//OpenGL Headers
#ifndef __APPLE__
	#include <GL/glew.h>
	#include <GL/glut.h>
#else
	#include <GLEW/glew.h>
	#include <GLUT/glut.h>
#endif

//String Utility Header
#include "string_util.hpp"

//Vector Header
#include <vector>

//Color Class Constructor (Default)
msl::color::color(const float red,const float green,const float blue,const float alpha):r(red),g(green),b(blue),a(alpha)
{}

//Point Direction Function (Returns direction between two points in degrees)
double msl::point_direction(const double x1,const double y1,const double x2,const double y2)
{
	return atan2(y2-y1,x2-x1)*180.0/M_PI;
}

//Point Distance Function (Returns distance between two points)
double msl::point_distance(const double x1,const double y1,const double x2,const double y2)
{
	return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

//Draw Point Function
void msl::draw_point(const double x,const double y,const msl::color& color)
{
	//Enable Transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//Disable Culling
	glDisable(GL_CULL_FACE);

	//Set Color
	glColor4d(color.r,color.g,color.b,color.a);

	//Draw Point
	glBegin(GL_POINTS);
		glVertex2d(x,y);
	glEnd();

	//Disable Transparency
	glDisable(GL_BLEND);
}

//Draw Line Function
void msl::draw_line(const double x1,const double y1,const double x2,const double y2,const msl::color& color)
{
	//Enable Transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//Disable Culling
	glDisable(GL_CULL_FACE);

	//Set Color
	glColor4d(color.r,color.g,color.b,color.a);

	//Draw Line
	glBegin(GL_LINES);
		glVertex2d(x1,y1);
		glVertex2d(x2,y2);
	glEnd();

	//Disable Transparency
	glDisable(GL_BLEND);
}

//Draw Triangle Function
void msl::draw_triangle(const double x1,const double y1,const double x2,const double y2,const double x3,
	const double y3,const bool fill,const msl::color& color)
{
	//Enable Transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//Disable Culling
	glDisable(GL_CULL_FACE);

	//Set Color
	glColor4d(color.r,color.g,color.b,color.a);

	//Draw Triangle
	if(fill)
		glBegin(GL_TRIANGLES);
	else
		glBegin(GL_LINE_LOOP);

		glVertex2d(x1,y1);
		glVertex2d(x2,y2);
		glVertex2d(x3,y3);
	glEnd();

	//Disable Transparency
	glDisable(GL_BLEND);
}

//Draw Rectangle Function
void msl::draw_rectangle(const double x,const double y,const double width,const double height,const bool fill,const msl::color& color)
{
	//Enable Transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//Disable Culling
	glDisable(GL_CULL_FACE);

	//Set Color
	glColor4d(color.r,color.g,color.b,color.a);

	//Draw Rectangle
	if(fill)
		glBegin(GL_QUADS);
    else
		glBegin(GL_LINE_LOOP);

		glVertex2d(x-width/2.0-0.2,y+height/2.0+0.2);
		glVertex2d(x+width/2.0+0.2,y+height/2.0+0.2);
		glVertex2d(x+width/2.0+0.2,y-height/2.0-0.2);
		glVertex2d(x-width/2.0-0.2,y-height/2.0-0.2);
	glEnd();

	//Disable Transparency
	glDisable(GL_BLEND);
}

//Draw Rectangle Gradient Function
void msl::draw_rectangle_gradient(const double x,const double y,const double width,const double height,const bool fill,
	const msl::color& color_top_left,const msl::color& color_top_right,const msl::color& color_bottom_right,
	const msl::color& color_bottom_left)
{
	//Enable Transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//Disable Culling
	glDisable(GL_CULL_FACE);

	//Draw Rectangle
	if(fill)
		glBegin(GL_QUADS);
    else
		glBegin(GL_LINE_LOOP);

		glColor4d(color_top_left.r,color_top_left.g,color_top_left.b,color_top_left.a);
		glVertex2d(x-width/2.0,y+height/2.0);
		glColor4d(color_top_right.r,color_top_right.g,color_top_right.b,color_top_right.a);
		glVertex2d(x+width/2.0,y+height/2.0);
		glColor4d(color_bottom_right.r,color_bottom_right.g,color_bottom_right.b,color_bottom_right.a);
		glVertex2d(x+width/2.0,y-height/2.0);
		glColor4d(color_bottom_left.r,color_bottom_left.g,color_bottom_left.b,color_bottom_left.a);
		glVertex2d(x-width/2.0,y-height/2.0);
	glEnd();

	//Disable Transparency
	glDisable(GL_BLEND);
}

//Draw Circle Function
void msl::draw_circle(const double x,const double y,const double radius,const msl::color& color)
{
	//Enable Transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//Disable Culling
	glDisable(GL_CULL_FACE);

	//Set Color
	glColor4d(color.r,color.g,color.b,color.a);

	//Draw Circle (Triangle Strip)
	glBegin(GL_TRIANGLE_STRIP);

		//Determine "Wedge" Variables
		int segments=std::max(10,(int)radius*2);
		double angle=2.0*M_PI/(double)segments;

		//Draw All But Last "Wedge"
		for(int ii=0;ii<segments;++ii)
		{
			glVertex2d(x+cos(angle*ii)*radius,y+sin(angle*ii)*radius);
			glVertex2d(x,y);
		}

	//Draw Last "Wedge"
	glVertex2d(x+radius,y);

	//Done With Circle
	glEnd();

	//Disable Transparency
	glDisable(GL_BLEND);
}

//Text Width Function (Returns width of text in pixels)
double msl::text_width(const std::string& text)
{
	//Length Variables
	std::vector<double> lengths;
	double current_length=0;

	//Draw String
	for(unsigned int ii=0;ii<text.size();++ii)
	{
		//Newlines
		if(text[ii]=='\n')
		{
			lengths.push_back(current_length);
			current_length=0;
		}

		//Tabs
		else if(text[ii]=='\t')
		{
			//Get Current Line Width
			unsigned int line_width=0;

			for(int jj=ii-1;jj>=0&&text[jj]!='\t'&&text[jj]!='\n';--jj)
				++line_width;

			//Add Indents
			for(unsigned int jj=line_width%4;jj<4;++jj)
				current_length+=glutBitmapWidth(GLUT_BITMAP_HELVETICA_12,' ');
		}

		//Characters
		else
		{
			current_length+=glutBitmapWidth(GLUT_BITMAP_HELVETICA_12,text[ii]);
		}
	}

	//Add Last Length
	lengths.push_back(current_length);

	//Return Max Length
	std::sort(lengths.begin(),lengths.end());

	//Return Longest Length
	return lengths[0];
}

//Text Drawing Function
void msl::draw_text(const double x,const double y,const std::string& text,const msl::color& color)
{
	//Disable Texture
	glDisable(GL_TEXTURE_2D);

	//Go To Project Mode
	glMatrixMode(GL_PROJECTION);

	//Save Current Matrix
	glPushMatrix();

	//Clear New Matrix
	glLoadIdentity();

	//Setup Drawing View
	glOrtho(-glutGet(GLUT_WINDOW_WIDTH)/2,glutGet(GLUT_WINDOW_WIDTH)/2,-glutGet(GLUT_WINDOW_HEIGHT)/2,glutGet(GLUT_WINDOW_HEIGHT)/2,0,1);

	//Set Color
	glColor4d(color.r,color.g,color.b,color.a);

	//Vertical Offset Variable
	double offset_increment=13;
	double offset=offset_increment;

	//Move Text Drawing Position
	glRasterPos2d(x,y-offset+1);

	//Draw String
	for(unsigned int ii=0;ii<text.size();++ii)
	{
		//Newlines
		if(text[ii]=='\n')
		{
			offset+=offset_increment;
			glRasterPos2d(x,y-offset+1);
		}

		//Tabs
		else if(text[ii]=='\t')
		{
			//Get Current Line Width
			unsigned int line_width=0;

			for(int jj=ii-1;jj>=0&&text[jj]!='\t'&&text[jj]!='\n';--jj)
				++line_width;

			//Add Indents
			for(unsigned int jj=line_width%4;jj<4;++jj)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,' ');
		}

		//Characters
		else
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,text[ii]);
		}
	}

	//Reset Color
	glColor4d(1,1,1,1);

	//Load Old Matrix
	glPopMatrix();

	//Return To Matrix Mode
	glMatrixMode(GL_MODELVIEW);
}