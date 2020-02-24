#ifndef FIRELIST_H
#define FIRELIST_H

#include "xdldef.h"
#include "xdlwidget.h"

#ifdef __cplusplus
extern "C" {
#endif

XDL_API widget_t* firelist_create(const wchar_t* wname,unsigned long id,int x,int y,int width,int height,handle_t owner);

XDL_API void firelist_destroy(widget_t* pwd);

XDL_API void firelist_set_options(widget_t* pwd,const wchar_t* options,int len, wchar_t itemfeed, wchar_t linefeed);

#ifdef __cplusplus
}
#endif

#endif /*FIRELIST_H*/