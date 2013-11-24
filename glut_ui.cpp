//Glut User Interface Source
//	Created By:		Mike Moss
//	Modified On:	11/23/2013

//Required Libraries:
//	gl
//	glew
//	glu
//	glut/freeglut

//Definitions for "glut_ui.hpp"
#include "glut_ui.hpp"

//Glut Input Header
#include "glut_input.hpp"

#include "string_util.hpp"

#include "time_util.hpp"

//OpenGL Headers
#ifndef __APPLE__
	#include <GL/glew.h>
	#include <GL/glut.h>
#else
	#include <GLEW/glew.h>
	#include <GLUT/glut.h>
#endif
#include <iostream>
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
{}

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

	if(vertical)
	{
		button_.width=height;
		button_.height=width;
	}
	else
	{
		button_.width=width;
		button_.height=height;
	}

	button_.background_color_from=background_color_from;
	button_.background_color_to=background_color_to;
	button_.outline_color=outline_color;
	button_.outline_color_hover=outline_color_hover;
	button_.outline_color_disabled=outline_color_disabled;
	button_.text_color=text_color;
	button_.text_color_disabled=text_color_disabled;

	button_.loop(dt);
}

msl::textbox::textbox(const std::string& value,const double x,const double y):widget(x,y,104,-1),
	value(value),cursor(0),focus(false),background_color(1,1,1,1),background_color_disabled(0.8,0.8,0.8,1),
	padding_(4),blink_timer_(msl::millis()),blink_show_(false),
	view_start(cursor),view_end(value.size())
{}

void msl::textbox::constrain_cursor()
{
	if(cursor<0)
		cursor=0;

	if((unsigned int)cursor>value.size())
		cursor=value.size();

	if(view_end<0)
		view_end=0;

	if((unsigned int)view_end>value.size())
		view_end=value.size();

	if(view_start<0)
		view_start=0;

	if(view_start>view_end)
		view_start=view_end;
}

void msl::textbox::update_cursor()
{
	constrain_cursor();

	if(cursor<view_start)
	{
		view_start=cursor;
		view_end_update_from_start();
	}

	if(cursor>view_end)
	{
		view_end=cursor;
		view_start_update_from_end();
	}

	type(' ');
	backspace();
}

void msl::textbox::view_end_update_from_start()
{
	view_end=view_start;
	double textbox_max_width=width-padding_*2;

	while((unsigned int)view_end<value.size()&&msl::text_width(value.substr(view_start,view_end-view_start))<
		textbox_max_width)
		++view_end;
}

void msl::textbox::view_start_update_from_end()
{
	view_start=view_end;
	double textbox_max_width=width-padding_*2;

	while(view_start>0&&msl::text_width(value.substr(view_start,view_end-view_start))<textbox_max_width)
		--view_start;
}

void msl::textbox::backspace()
{
	if(value.size()>0&&cursor-1>=0)
	{
		--cursor;
		value.erase(cursor,1);
	}
}

void msl::textbox::del()
{
	if(value.size()>0&&(unsigned int)(cursor+1)<=value.size())
		value.erase(cursor,1);
}

void msl::textbox::type(const char key)
{
	std::string temp;
	temp+=(char)key;
	value.insert(cursor,temp);
	++cursor;
	view_end_update_from_start();
}

void msl::textbox::loop(const double dt)
{
	if(visible)
	{
		for(unsigned int ii=0;ii<value.size();++ii)
			if(value[ii]=='\n')
				value.erase(ii,1);

		if(!disabled)
		{
			bool new_hover=(msl::mouse_x>=x-width/2.0&&msl::mouse_x<=x+width/2.0&&
				msl::mouse_y>=y-height/2.0&&msl::mouse_y<=y+height/2.0);

			if(hover&!new_hover)
				glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

			if(!hover&new_hover)
				glutSetCursor(GLUT_CURSOR_TEXT);

			hover=new_hover;

			down=hover&&msl::input_check(mb_left);
			pressed=hover&&msl::input_check_released(mb_left);

			if(msl::input_check_pressed(mb_left))
			{
				focus=hover;

				if(focus)
				{
					double start_x=x-width/2.0+padding_;
					double test=mouse_x-start_x;

					cursor=-1;

					for(int ii=0;ii<view_end-view_start;++ii)
					{
						if(msl::text_width(value.substr(view_start,ii))>=test)
						{
							cursor=view_start+ii;
							break;
						}
					}

					if(cursor<0)
						cursor=view_end;
				}
			}

			if(!hover&&(msl::input_check_pressed(mb_right)||msl::input_check_pressed(mb_middle)))
				focus=false;

			if(focus)
			{

				if(msl::millis()>blink_timer_)
				{
					blink_timer_=msl::millis()+500;
					blink_show_=!blink_show_;
				}

				if(msl::input_check_pressed(kb_home))
					cursor=0;

				if(msl::input_check_pressed(kb_end))
					cursor=value.size();

				if(msl::input_check_pressed(kb_left))
				--cursor;

				if(msl::input_check_pressed(kb_right))
					++cursor;

				for(int ii=32;ii<=126;++ii)
					if(msl::input_check_pressed(ii))
						type(ii);

				if(msl::input_check_pressed(kb_tab))
					type('\t');

				if(msl::input_check_pressed(kb_backspace))
					backspace();

				if(msl::input_check_pressed(kb_delete))
					del();
			}
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

	update_cursor();
}

void msl::textbox::draw()
{
	if(visible)
	{
		msl::color bg_col=background_color;
		msl::color out_col=outline_color;
		msl::color tex_col=text_color;

		if(hover)
			out_col=outline_color_hover;

		if(disabled)
		{
			bg_col=background_color_disabled;
			out_col=outline_color_disabled;
			tex_col=text_color_disabled;
		}

		msl::draw_rectangle(x,y,width,height,true,bg_col);
		msl::draw_rectangle(x,y,width,height,false,out_col);

		msl::draw_text(x-width/2.0+padding_,y+14/2.0,value.substr(view_start,view_end-view_start),tex_col);

		if(focus&&blink_show_)
		{
			double cursor_x=msl::text_width(value.substr(view_start,cursor-view_start));
			msl::draw_rectangle(x-width/2.0+padding_+cursor_x,y,1,14,true,tex_col);
		}
	}
}