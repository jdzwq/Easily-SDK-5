#ifndef XDLDRAW_H
#define XDLDRAW_H

#include "xdldef.h"
#include "xdlctx.h"

#ifdef __cplusplus
extern "C" {
#endif

XDL_API void draw_line(const context_t* pct,const wchar_t* style,const xpoint_t *ppt1, const xpoint_t *ppt2);

XDL_API void draw_polyline(const context_t* pct,const wchar_t* style,const xpoint_t *ppt,int n);

XDL_API void draw_polygon(const context_t* pct,const wchar_t* style,const xpoint_t *ppt,int n);

XDL_API void draw_rect(const context_t* pct,const wchar_t* style,const xrect_t *prt);

XDL_API void draw_round(const context_t* pct,const wchar_t* style,const xrect_t *prt);

XDL_API void draw_ellipse(const context_t* pct,const wchar_t* style,const xrect_t *prt);

XDL_API void draw_text(const context_t* pct,const wchar_t* style,const xrect_t *prt,const wchar_t* txt,int len);

XDL_API void  draw_triangle(const context_t* pct,const wchar_t* style,const xrect_t *prt,const wchar_t* ori);

XDL_API void  draw_pie(const context_t* pct,const wchar_t* style,const xrect_t *prt,double fang,double tang);

//XDL_API void draw_shape(const context_t* pct,const wchar_t* style,const xrect_t *prt,int shape);

#ifdef __cplusplus
}
#endif

#endif /*XDLDRAW_H*/