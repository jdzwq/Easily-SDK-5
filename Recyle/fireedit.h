#ifndef FIREEDIT_H
#define FIREEDIT_H

#include "xdldef.h"
#include "xdlwidget.h"

#ifdef __cplusplus
extern "C" {
#endif

XDL_API widget_t* fireedit_create(const wchar_t* wname,unsigned long id,int x,int y,int width,int height,handle_t owner);

XDL_API void fireedit_destroy(widget_t* pwd);

#ifdef __cplusplus
}
#endif

#endif /*FIREEDIT_H*/