#ifndef FIREMEMO_H
#define FIREMEMO_H

#include "xdldef.h"
#include "xdlwidget.h"

#ifdef __cplusplus
extern "C" {
#endif

XDL_API widget_t* firememo_create(const wchar_t* wname,unsigned long id,int x,int y,int width,int height,handle_t owner);

XDL_API void firememo_destroy(widget_t* pwd);

XDL_API void firememo_set_text(widget_t* pwd,const wchar_t* text,int len);

XDL_API int firememo_get_text(widget_t* pwd,wchar_t* buf,int max);

#ifdef __cplusplus
}
#endif

#endif /*FIREMEMO_H*/