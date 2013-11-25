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
		x(x),y(y),width(width),height(height),display_width(width),display_height(height),hover(hover),down(down),
		pressed(pressed),disabled(disabled),visible(visible),background_color_from(background_color_from),
		background_color_to(background_color_to),outline_color(outline_color),
		outline_color_hover(outline_color_hover),outline_color_disabled(outline_color_disabled),
		text_color(text_color),text_color_disabled(text_color_disabled)
{}

msl::button::button(const std::string& value,const double x,const double y):widget(x,y),value(value),padding_(4)
{}

void msl::button::loop(const double dt)
{
	display_width=width;

	if(width<0)
		display_width=msl::text_width(value)+padding_*2;

	display_height=height;

	if(height<0)
		display_height=1.5*14;

	if(visible)
	{
		if(!disabled)
		{
			bool new_hover=(msl::mouse_x>=x-display_width/2.0&&msl::mouse_x<=x+display_width/2.0&&
				msl::mouse_y>=y-display_height/2.0&&msl::mouse_y<=y+display_height/2.0);

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

		msl::draw_rectangle_gradient(x,y,display_width,display_height,true,but_col_from,but_col_from,but_col_to,but_col_to);
		msl::draw_rectangle(x,y,display_width,display_height,false,out_col);
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
	hover=button_.hover;
	down=button_.down;
	pressed=button_.pressed;
	button_.disabled=disabled;
	button_.visible=visible;
	button_.width=width;
	button_.height=height;
	button_.display_width=width;
	button_.display_height=height;
	display_width=width;
	display_height=height;
	button_.background_color_from=background_color_from;
	button_.background_color_to=background_color_to;
	button_.outline_color=outline_color;
	button_.outline_color_hover=outline_color_hover;
	button_.outline_color_disabled=outline_color_disabled;
	button_.text_color=text_color;
	button_.text_color_disabled=text_color_disabled;

	button_.loop(dt);
}


























msl::dropdown::dropdown(const double x,const double y):widget(x,y,-1,-1),value(-1),button_("",x,y),
	selected(false),padding_(4)
{}

void msl::dropdown::loop(const double dt)
{
	double max_width=0;

	for(unsigned int ii=0;ii<options.size();++ii)
		if(msl::text_width(options[ii])>max_width)
			max_width=msl::text_width(options[ii]+"      ");

	button_.width=max_width+padding_*2;

	if(visible)
	{
		update_button(dt);

		if(pressed)
			selected=!selected;

		if(selected)
			button_.down=true;

		if(msl::input_check(mb_left)&&!hover)
			selected=false;
	}
}

void msl::dropdown::draw()
{
	if(visible)
	{
		msl::color out_col=outline_color;
		msl::color tex_col=text_color;

		if(hover)
			out_col=outline_color_hover;

		if(disabled)
		{
			out_col=outline_color_disabled;
			tex_col=text_color_disabled;
		}

		button_.draw();

		if(selected)
		{
			double drop_menu_height=options.size()*14*1.5;

			msl::draw_rectangle(x,y-button_.display_height/2.0-drop_menu_height/2.0,button_.display_width,drop_menu_height,true);

			double diff=mouse_y-(y-button_.display_height/2.0-drop_menu_height);
			unsigned int index=options.size()-diff/(14*1.5);

			if(msl::mouse_x<x-button_.display_width/2.0||msl::mouse_x>x+button_.display_width/2.0)
				index=-1;

			for(unsigned int ii=0;ii<options.size();++ii)
			{
				msl::color new_tex_col=tex_col;

				if(ii==index)
				{
					msl::draw_rectangle(x,y-button_.display_height/2.0-(ii+0.5)*14*1.5,button_.display_width,14*1.5,true,msl::color(0.2,0.4,1,1));
					new_tex_col=msl::color(1,1,1,1);
				}

				msl::draw_text(x-button_.display_width/2.0+padding_,y-(button_.display_height+padding_)/2.0-ii*14*1.5,options[ii],new_tex_col);
			}

			msl::draw_rectangle(x,y-button_.display_height/2.0-drop_menu_height/2.0,button_.display_width,drop_menu_height,false,out_col);

			if(msl::input_check_pressed(mb_left)&&index>=0&&index<options.size())
			{
				value=index;
				selected=false;
			}
		}

		msl::draw_triangle(x+button_.display_width/2.0-padding_*2,y-padding_/2.0,x+button_.display_width/2.0-padding_*1,
			y+padding_/2.0,x+button_.display_width/2.0-padding_*3,y+padding_/2.0,true,tex_col);

		if(value>=0&&value<options.size())
			msl::draw_text(x-button_.display_width/2.0+padding_,y+14/2.0,options[value],tex_col);

		msl::draw_text(x,y-100,msl::to_string(value));
	}
}

void msl::dropdown::update_button(const double dt)
{
	hover=button_.hover;
	down=button_.down;
	pressed=button_.pressed;
	button_.disabled=disabled;
	button_.visible=visible;
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

	button_.display_width=width;
	button_.display_height=height;
	display_width=width;
	display_height=height;

	button_.background_color_from=background_color_from;
	button_.background_color_to=background_color_to;
	button_.outline_color=outline_color;
	button_.outline_color_hover=outline_color_hover;
	button_.outline_color_disabled=outline_color_disabled;
	button_.text_color=text_color;
	button_.text_color_disabled=text_color_disabled;

	button_.loop(dt);
}

msl::textbox::textbox(const std::string& value,const double x,const double y):widget(x,y,-1,-1),
	value(value),cursor(0),focus(false),readonly(false),background_color(1,1,1,1),
	background_color_disabled(0.8,0.8,0.8,1),padding_(4),view_start_(cursor),view_end_(value.size()),
	blink_timer_(msl::millis()),blink_show_(false),repeat_timer_(msl::millis()),repeat_wait_(150),
	repeat_key_(0),repeat_(false)
{}

void msl::textbox::loop(const double dt)
{
	update_display_dimensions();

	if(visible)
	{
		for(unsigned int ii=0;ii<value.size();++ii)
			if(value[ii]=='\n')
				value.erase(ii,1);

		if(!disabled&&!readonly)
		{
			bool new_hover=(msl::mouse_x>=x-display_width/2.0&&msl::mouse_x<=x+display_width/2.0&&
				msl::mouse_y>=y-display_height/2.0&&msl::mouse_y<=y+display_height/2.0);

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
					double start_x=x-display_width/2.0+padding_;
					double test=mouse_x-start_x;

					cursor=-1;

					for(int ii=0;ii<view_end_-view_start_;++ii)
					{
						if(msl::text_width(value.substr(view_start_,ii))>=test)
						{
							cursor=view_start_+ii;
							break;
						}
					}

					if(cursor<0)
						cursor=view_end_;
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

				if(msl::input_check_pressed(kb_left)||(repeat_&&repeat_key_==kb_left&&msl::millis()>repeat_timer_))
				{
					--cursor;
					repeat_check(kb_left);
				}

				if(msl::input_check_pressed(kb_right)||(repeat_&&repeat_key_==kb_right&&msl::millis()>repeat_timer_))
				{
					++cursor;
					repeat_check(kb_right);
				}

				for(int ii=32;ii<=126;++ii)
				{
					if(msl::input_check_pressed(ii)||(repeat_&&repeat_key_==ii&&msl::millis()>repeat_timer_))
					{
						type(ii);
						repeat_check(ii);
					}
				}

				if(msl::input_check_pressed(kb_tab)||(repeat_&&repeat_key_==kb_tab&&msl::millis()>repeat_timer_))
				{
					type('\t');
					repeat_check(kb_tab);
				}

				if(msl::input_check_pressed(kb_backspace)||(repeat_&&repeat_key_==kb_backspace&&msl::millis()>repeat_timer_))
				{
					backspace();
					repeat_check(kb_backspace);
				}

				if(msl::input_check_pressed(kb_delete)||(repeat_&&repeat_key_==kb_delete&&msl::millis()>repeat_timer_))
				{
					del();
					repeat_check(kb_delete);
				}

				repeat_update();
			}
		}
		else
		{
			hover=false;
			down=false;
			pressed=false;
		}
	}

	update_cursor();
}

void msl::textbox::draw()
{
	if(visible)
	{
		update_display_dimensions();

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

		msl::draw_rectangle(x,y,display_width,display_height,true,bg_col);
		msl::draw_rectangle(x,y,display_width,display_height,false,out_col);

		msl::draw_text(x-display_width/2.0+padding_,y+14/2.0,value.substr(view_start_,view_end_-view_start_),tex_col);

		if(focus&&blink_show_)
		{
			double cursor_x=msl::text_width(value.substr(view_start_,cursor-view_start_));
			msl::draw_rectangle(x-display_width/2.0+padding_+cursor_x,y,1,14,true,tex_col);
		}

		msl::draw_text(x,-100,msl::to_string(repeat_)+"\t"+msl::to_string(repeat_key_)+"\t"+msl::to_string(msl::input_check(repeat_key_)));
	}
}

void msl::textbox::constrain_cursor()
{
	if(cursor<0)
		cursor=0;

	if((unsigned int)cursor>value.size())
		cursor=value.size();

	if(view_end_<0)
		view_end_=0;

	if((unsigned int)view_end_>value.size())
		view_end_=value.size();

	if(view_start_<0)
		view_start_=0;

	if(view_start_>view_end_)
		view_start_=view_end_;
}

void msl::textbox::update_cursor()
{
	constrain_cursor();

	if(cursor<view_start_)
	{
		view_start_=cursor;
		view_end_update_from_start();
	}

	if(cursor>view_end_)
	{
		view_end_=cursor;
		view_start_update_from_end();
	}

	type(' ');
	backspace();
}

void msl::textbox::view_end_update_from_start()
{
	update_display_dimensions();

	view_end_=view_start_;
	double textbox_max_width=display_width-padding_*2;

	while((unsigned int)view_end_<value.size()&&msl::text_width(value.substr(view_start_,view_end_-view_start_))<
		textbox_max_width)
		++view_end_;
}

void msl::textbox::view_start_update_from_end()
{
	update_display_dimensions();

	view_start_=view_end_;
	double textbox_max_width=display_width-padding_*2;

	while(view_start_>0&&msl::text_width(value.substr(view_start_,view_end_-view_start_))<textbox_max_width)
		--view_start_;
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

void msl::textbox::repeat_check(const int key)
{
	if(!repeat_)
	{
		repeat_=true;
		repeat_key_=key;
		repeat_timer_=msl::millis()+repeat_wait_;
	}

	if(repeat_&&repeat_key_!=key)
		repeat_=false;
}

void msl::textbox::repeat_update()
{
	if(repeat_&&msl::millis()>repeat_timer_)
		repeat_timer_=msl::millis()+repeat_wait_;

	if(!msl::input_check(repeat_key_)||msl::input_check_released(repeat_key_))
		repeat_=false;
}

void msl::textbox::update_display_dimensions()
{
	display_width=width;

	if(width<0)
		display_width=msl::text_width(value)+padding_*2;

	display_height=height;

	if(height<0)
		display_height=1.5*14;
}