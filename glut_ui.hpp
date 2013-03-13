//Glut User Interface Header
//	Created By:		Mike Moss
//	Modified On:	03/12/2013

//This requires:
//	gl
//	glew
//	glu
//	glui
//	glut/freeglut

//Begin Define Guards
#ifndef MSL_GLUT_UI_H
#define MSL_GLUT_UI_H

//OpenGL Headers
#ifndef __APPLE__
	#include <GL/glew.h>
	#include <GL/glut.h>
	#include <GL/glui.h>
#else
	#include <GLUT/glew.h>
	#include <GLUT/glut.h>
	#include <GLUT/glui.h>
#endif

//String Header
#include <string>

//Vector Header
#include <vector>

//Panel Position Bindings
#define ui_panel_left   GLUI_SUBWINDOW_LEFT
#define ui_panel_right  GLUI_SUBWINDOW_RIGHT
#define ui_panel_top    GLUI_SUBWINDOW_TOP
#define ui_panel_bottom GLUI_SUBWINDOW_BOTTOM

//Alignment Bindings
#define ui_left         GLUI_ALIGN_LEFT
#define ui_center       GLUI_ALIGN_CENTER
#define ui_right        GLUI_ALIGN_RIGHT

//MSL Namespace
namespace msl
{
	//UI Setup Function (Sets up glui)
	void ui_setup(const int glut_window);

	//UI Stop Function (Stops UI and cleans up glui...as best as it can...because glui is full of memory leaks...not my fault...)
	void ui_stop();

	//UI In Use Function (Checks if glui is being used...helpful for setting callbacks...because glui has it's own callback functions...)
	bool ui_in_use();

	//UI Idle Function (Updates and syncs values)
	void ui_idle();

	//UI Reset Function (Resets button pressed events...should be done at the very end of the Glut idle function)
	void ui_reset();

	//Panel Commands (Creates UI panels, position can be any of the panel position bindings at the top of this document)
	void ui_panel_begin(const long position);
	void ui_panel_end();

	//Group Commands (Groups UI controls on the panel, the options are faily easy to understand...I hope...)
	void ui_group_begin(const std::string& caption="",const bool outline=true);
	void ui_group_end();

	//Rollout Commands (Like groups but they can be collapsed and they always have an outline, open is to have to rollout opened or closed when the application starts)
	void ui_rollout_begin(const std::string caption,const bool open=true);
	void ui_rollout_end();

	//Radio Commands (Creates radio buttons, value is the variable glui changes when you change the radio option)
	void ui_radio_begin(int& value);
	void ui_radio_add(const std::string& caption);
	void ui_radio_end();

	//Selection Commands (Creates a text selection box, value is the variable glui changes when you change the selection option)
	void ui_selection_begin(const std::string& name,int& value);
	void ui_selection_add(const std::string caption);
	void ui_selection_end();

	//Separator Command (Creates a horizontal separator)
	void ui_separator_add(const bool visible=true);

	//Collumn Command (Creates a new column on the panel)
	void ui_column_add(const bool visible=true);

	//Alignment Command (Sets horizontal placement alignment for controls..alignment can be any of the alignment bindings at the top of this document)
	void ui_set_alignment(const int alignment);

	//Text Command (Creates text)
	void ui_add_text(const std::string& text);

	//Checkbox Command (Creates Checkboxes)
	void ui_add_checkbox(const std::string& caption,bool& value);

	//Spinner Commands (Creates a text field where there is a number you can edit by typing or use small arrows on the side...value is the variable glui changes when you change the number...
	//	min and max are the limits for the number...MUST BE SIGNED INT OR FLOAT NOT UNSIGNED INT OR DOUBLE!!!)
	void ui_spinner_add(const std::string& caption,float& value,const float min,const float max);
	void ui_spinner_add(const std::string& caption,int& value,const int min,const int max);

	//Textbox Command (Creates a text field you can edit by typing...value is the variable glui changes when you change the text...)
	void ui_textbox_add(const std::string& caption,std::string& value);

	//Button Command (Creates a button you can click on with your mouse...down_value is the variable glui changes when you hold the button down...
	//	pressed_value is the variable glui changes when you click the button...
	void ui_button_add(const std::string& caption,bool& down_value,bool& pressed_value);

	//Scrollbar Commands (Creates a scrollbar...value is the variable glui changes when you move the scrollbar...MUST BE SIGNED INT OR FLOAT NOT UNSIGNED INT OR DOUBLE!!!)
	void ui_scrollbar_horizontal_add(float& value,const float min,const float max);
	void ui_scrollbar_horizontal_add(int& value,const int min,const int max);
	void ui_scrollbar_vertical_add(float& value,const float min,const float max);
	void ui_scrollbar_vertical_add(int& value,const int min,const int max);
}

//End Define Guards
#endif