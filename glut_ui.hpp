//Glut User Interface Header
//	Created By:		Mike Moss
//	Modified On:	11/22/2013

//Required Libraries:
//	gl
//	glew
//	glu
//	glui
//	glut/freeglut

//Begin Define Guards
#ifndef MSL_GLUT_UI_H
#define MSL_GLUT_UI_H

//2D Utility Header
#include "2d_util.hpp"

//String Header
#include <string>

//MSL Namespace
namespace msl
{
	class widget
	{
		friend void ui_loop(const double dt);
		friend void ui_draw();

		public:
			widget(const double x,const double y,const double width=-1,const double height=-1,
				const bool hover=false,const bool down=false,const bool pressed=false,const bool disabled=false,
				const bool visible=true,
				const msl::color& background_color_from=msl::color(0.9,0.9,0.9,1.0),
				const msl::color& background_color_to=msl::color(0.6,0.6,0.6,1.0),
				const msl::color& outline_color=msl::color(0.3,0.3,0.3,1),
				const msl::color& outline_color_hover=msl::color(0.0,0.0,0.0,1.0),
				const msl::color& outline_color_disabled=msl::color(0.3,0.3,0.3,1),
				const msl::color& text_color=msl::color(0.0,0.0,0.0,1.0),
				const msl::color& text_color_disabled=msl::color(0.3,0.3,0.3,1));

			virtual void loop(const double dt)=0;
			virtual void draw()=0;

			double x;
			double y;
			double width;
			double height;
			bool hover;
			bool down;
			bool pressed;
			bool disabled;
			bool visible;
			msl::color background_color_from;
			msl::color background_color_to;
			msl::color outline_color;
			msl::color outline_color_hover;
			msl::color outline_color_disabled;
			msl::color text_color;
			msl::color text_color_disabled;
	};

	class button:public widget
	{
		public:
			button(const std::string& value="",const double x=0,const double y=0);

			void loop(const double dt);
			void draw();

			std::string value;
	};

	class checkbox:public widget
	{
		public:
			checkbox(const bool value=false,const double x=0,const double y=0);

			void loop(const double dt);
			void draw();

			bool value;

		private:
			void update_button(const double dt);

			button button_;
	};

	class slider:public widget
	{

		public:
			slider(const double value,const double min,const double max,const double x,const double y,const bool vertical=false,const double length=100);

			void loop(const double dt);
			void draw();

			double value;
			double min;
			double max;
			bool vertical;
			double length;
			msl::color track_color;
			msl::color track_color_disabled;

		private:
			void update_button(const double dt);

			button button_;
			bool drag_;
	};

	class textbox:public widget
	{
		public:
			textbox(const std::string& value="",const double x=0,const double y=0);

			void loop(const double dt);
			void draw();

			std::string value;
	};
}

#endif