#ifndef FIREDATE_H
#define FIREDATE_H

#include "xdldef.h"
#include "xdlwidget.h"

#ifdef __cplusplus
extern "C" {
#endif

XDL_API widget_t* firedate_create(const wchar_t* wname,unsigned long id,int x,int y,int width,int height,handle_t owner);

XDL_API void firedate_destroy(widget_t* pwd);

XDL_API void firedate_set_datetime(widget_t* pwd,const xdate_t* pdt);

XDL_API void firedate_get_datetime(widget_t* pwd,xdate_t* pdt);

#ifdef __cplusplus
}
#endif

#endif /*FIREDATE_H*/