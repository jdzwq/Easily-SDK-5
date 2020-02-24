#ifndef XDLWIDGET_H
#define XDLWIDGET_H

#include "xdldef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WIDGET_CHILD			1
#define WIDGET_POPUP			2
#define WIDGET_DIALOG			3

#define WIDGET_NOTIFY_ACCEPT		1
#define WIDGET_NOTIFY_CANCEL		2

#define WIDGETCLASS				UN("WIDGET")

typedef struct _widget_t*	widget_t_ptr;
typedef struct _event_t*	event_t_ptr;

typedef int	(*on_event)(widget_t_ptr pwd,event_t_ptr pev);

typedef struct _event_t{
	handle_t wnd;
	unsigned long msg;
	unsigned long wp;
	void*	lp;
	
	int x;
	int y;
	handle_t sub;
	unsigned long id;
	unsigned long code;
	unsigned long mask;
	wchar_t key;
}event_t;


typedef struct _widget_t{
	handle_t owner;	
	handle_t self;

	int type;
	int x;
	int y;
	int width;
	int height;
	unsigned long id;
	unsigned long style;
	wchar_t* cname;
	wchar_t* wname;
	wchar_t* text;
	void*	extra;
	void*	proc;

	on_event	pf_on_create;
	on_event	pf_on_destroy;

	on_event	pf_on_prepare_message;
	on_event	pf_on_process_message;

	on_event	pf_on_keydown;
	on_event	pf_on_keyup;
	on_event	pf_on_char;

	on_event	pf_on_left_button_down;
	on_event	pf_on_left_button_up;
	on_event	pf_on_left_button_click;
	on_event	pf_on_right_button_down;
	on_event	pf_on_right_button_up;
	on_event	pf_on_right_button_click;
	on_event	pf_on_mouse_move;
	on_event	pf_on_mouse_wheel;
	on_event	pf_on_mouse_hover;
	on_event	pf_on_mouse_leave;

	on_event	pf_on_paint_window;

	on_event	pf_on_erase_client;
	on_event	pf_on_paint_client;

	on_event	pf_on_notify;
	on_event	pf_on_command;

}widget_t;

XDL_API bool_t widget_init(void);

XDL_API void widget_uninit(void);

XDL_API widget_t* widget_alloc(const wchar_t* cname,const wchar_t* wname,unsigned long id,int type,int x,int y,int width,int height,handle_t owner);

XDL_API void widget_free(widget_t* pwd);

XDL_API bool_t widget_create(widget_t* pwd);

XDL_API void widget_destroy(widget_t* pwd);

XDL_API void widget_set_focus(widget_t* pwd);


#ifdef __cplusplus
}
#endif

#endif /*XDLWIDGET_H*/