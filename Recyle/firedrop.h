#ifndef FIREDROP_H
#define FIREDROP_H

#include "xdldef.h"
#include "xdlwidget.h"

#ifdef __cplusplus
extern "C" {
#endif

XDL_API widget_t* firedrop_create(const wchar_t* wname,unsigned long id,int x,int y,int width,int height,handle_t owner);

XDL_API void firedrop_destroy(widget_t* pwd);

XDL_API void firedrop_set_options(widget_t* pwd,const wchar_t* options,int len, wchar_t itemfeed, wchar_t linefeed);

#ifdef __cplusplus
}
#endif

#endif /*FIREDROP_H*/