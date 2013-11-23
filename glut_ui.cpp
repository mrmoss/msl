//Glut User Interface Source
//	Created By:		Mike Moss
//	Modified On:	11/22/2013

//Required Libraries:
//	gl
//	glew
//	glu
//	glut/freeglut

//Definitions for "glut_ui.hpp"
#include "glut_ui.hpp"

//Glut Input Header
#include "glut_input.hpp"

//OpenGL Headers
#ifndef __APPLE__
	#include <GL/glew.h>
	#include <GL/glut.h>
#else
	#include <GLEW/glew.h>
	#include <GLUT/glut.h>
#endif

msl::widget::widget(const double x,const double y,const double width,const double height,
	const bool hover,const bool down,const bool pressed,const bool disabled,
	const bool visible,const msl::color& background_color_from,const msl::color& background_color_to,
	const msl::color& outline_color,const msl::color& outline_color_hover,
	const msl::color& outline_color_disabled,const msl::color& text_color,const msl::color& text_color_disabled):
		x(x),y(y),width(width),height(height),hover(hover),down(down),pressed(pressed),
		disabled(disabled),visible(visible),background_color_from(background_color_from),
		background_color_to(background_color_to),outline_color(outline_color),
		outline_color_hover(outline_color_hover),outline_color_disabled(outline_color_disabled),
		text_color(text_color),text_color_disabled(text_color_disabled)
{}

msl::button::button(const std::string& value,const double x,const double y):widget(x,y),value(value)
{}

void msl::button::loop(const double dt)
{
	if(visible)
	{
		if(!disabled)
		{
			bool new_hover=(msl::mouse_x>=x-width/2.0&&msl::mouse_x<=x+width/2.0&&
				msl::mouse_y>=y-height/2.0&&msl::mouse_y<=y+height/2.0);

			if(hover&!new_hover)
				glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

			if(!hover&new_hover)
				glutSetCursor(GLUT_CURSOR_INFO);

			hover=new_hover;

			down=hover&&msl::input_check(mb_left);
			pressed=hover&&msl::input_check_released(mb_left);
		}
		else
		{
			hover=false;
			down=false;
			pressed=false;
		}

		if(width<0)
			width=msl::text_width(value+"--");

		if(height<0)
			height=1.5*14;
	}
}

void msl::button::draw()
{
	if(visible)
	{
		msl::color but_col_to=background_color_to;
		msl::color but_col_from=background_color_from;
		msl::color out_col=outline_color;
		msl::color tex_col=text_color;

		if(hover)
			out_col=outline_color_hover;

		if(pressed||down)
			std::swap(but_col_from,but_col_to);

		if(disabled)
		{
			but_col_from=but_col_to;
			out_col=outline_color_disabled;
			tex_col=text_color_disabled;
		}

		double text_width=msl::text_width(value);

		msl::draw_rectangle_gradient(x,y,width,height,true,but_col_from,but_col_from,but_col_to,but_col_to);
		msl::draw_rectangle(x,y,width,height,false,out_col);
		msl::draw_text(x-text_width/2.0,y+14/2.0,value,tex_col);
	}
}

msl::checkbox::checkbox(const bool value,const double x,const double y):widget(x,y,12,12),value(value),button_("",x,y)
{}

void msl::checkbox::loop(const double dt)
{
	update_button(dt);

	if(button_.pressed&&!disabled)
		value=!value;
}

void msl::checkbox::draw()
{
	if(visible)
	{
		button_.x=x;
		button_.y=y;

		button_.draw();

		if(value)
		{
			msl::color col=text_color;

			if(disabled)
				col=text_color_disabled;

			msl::draw_circle(x-3,y+0,1.5,col);
			msl::draw_circle(x-2,y-1,1.5,col);

			for(int ii=-1;ii<=3;++ii)
				msl::draw_circle(x+ii,y+ii-1,1.5,col);
		}
	}
}

void msl::checkbox::update_button(const double dt)
{
	button_.down=down;
	button_.pressed=pressed;
	button_.disabled=disabled;
	button_.visible=visible;
	button_.width=width;
	button_.height=height;
	button_.background_color_from=background_color_from;
	button_.background_color_to=background_color_to;
	button_.outline_color=outline_color;
	button_.outline_color_hover=outline_color_hover;
	button_.outline_color_disabled=outline_color_disabled;
	button_.text_color=text_color;
	button_.text_color_disabled=text_color_disabled;

	button_.loop(dt);
}

msl::slider::slider(const double value,const double min,const double max,const double x,const double y,
	const bool vertical,const double length):
		widget(x,y,12,16),value(value),min(min),max(max),vertical(vertical),length(length),
		track_color(msl::color(0.3,0.3,0.3,1)),track_color_disabled(msl::color(0.4,0.4,0.4,1)),
		button_("",x,y),drag_(false)
{
	if(vertical)
		std::swap(width,height);
}

void msl::slider::loop(const double dt)
{
	if(visible)
	{
		update_button(dt);

		if(button_.down)
			drag_=true;

		if(msl::input_check_released(mb_left))
			drag_=false;

		if(drag_)
			down=true;

		double* pos=&x;
		double* button_pos=&button_.x;
		double* mouse_pos=&msl::mouse_x;

		if(vertical)
		{
			pos=&y;
			button_pos=&button_.y;
			mouse_pos=&msl::mouse_y;
		}

		if(drag_)
			*button_pos=*mouse_pos;
		else
			*button_pos=*pos+(value-min)/(max-min)*length;

		if(*button_pos<*pos)
			*button_pos=*pos;

		if(*button_pos>*pos+length)
			*button_pos=*pos+length;

		value=(*button_pos-*pos)/length*(max-min)+min;
	}
}

void msl::slider::draw()
{
	if(visible)
	{
		if(vertical)
			button_.x=x;
		else
			button_.y=y;

		msl::color track_col=track_color;

		if(disabled)
			track_col=track_color_disabled;

		if(vertical)
			msl::draw_rectangle(x,y+length/2.0,4,length,true,track_col);
		else
			msl::draw_rectangle(x+length/2.0,y,length,4,true,track_col);

		button_.draw();
	}
}

void msl::slider::update_button(const double dt)
{
	button_.down=down;
	button_.pressed=pressed;
	button_.disabled=disabled;
	button_.visible=visible;
	button_.width=width;
	button_.height=height;
	button_.background_color_from=background_color_from;
	button_.background_color_to=background_color_to;
	button_.outline_color=outline_color;
	button_.outline_color_hover=outline_color_hover;
	button_.outline_color_disabled=outline_color_disabled;
	button_.text_color=text_color;
	button_.text_color_disabled=text_color_disabled;

	button_.loop(dt);
}