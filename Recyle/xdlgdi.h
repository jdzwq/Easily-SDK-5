#ifndef XDLGDI_H
#define XDLGDI_H

#include "xdldef.h"
#include "xdlctx.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XPI  3.1415926535

XDL_API void draw_line_raw(const context_t* pct,const xpen_t* pxp,int x1,int y1,int x2,int y2);

XDL_API void draw_polyline_raw(const context_t* pct,const xpen_t* pxp,int* x,int* y,int n);

XDL_API void draw_polygon_raw(const context_t* pct,const xpen_t* pxp,int* x,int* y,int n);

XDL_API void fill_polygon_raw(const context_t* pct,const xbrush_t* pxb,int* x,int* y,int n);

XDL_API void draw_rect_raw(const context_t* pct,const xpen_t* pxp,int x1,int y1,int x2,int y2);

XDL_API void fill_rect_raw(const context_t* pct,const xbrush_t* pxb,int x1,int y1,int x2,int y2);

XDL_API void draw_round_raw(const context_t* pct,const xpen_t* pxp,int x1,int y1,int x2,int y2);

XDL_API void fill_round_raw(const context_t* pct,const xbrush_t* pxb,int x1,int y1,int x2,int y2);

XDL_API void draw_ellipse_raw(const context_t* pct,const xpen_t* pxp,int x1,int y1,int x2,int y2);

XDL_API void fill_ellipse_raw(const context_t* pct,const xbrush_t* pxb,int x1,int y1,int x2,int y2);

XDL_API void draw_text_raw(const context_t* pct,const xfont_t* pxf,int x1,int y1,int x2,int y2,const wchar_t* txt,int len);

XDL_API void  draw_triangle_raw(const context_t* pct,const xpen_t* pxp,int x1,int y1,int x2,int y2,const wchar_t* ori);

XDL_API void  fill_triangle_raw(const context_t* pct,const xbrush_t* pxb,int x1,int y1,int x2,int y2,const wchar_t* ori);

XDL_API void  draw_pie_raw(const context_t* pct,const xpen_t* pxp,int x1,int y1,int x2,int y2,double fang,double tang);

XDL_API void  fill_pie_raw(const context_t* pct,const xbrush_t* pxb,int x1,int y1,int x2,int y2,double fang,double tang);

XDL_API void draw_shape_raw(const context_t* pct,const xpen_t* pxp,int x1,int y1,int x2,int y2,int shape);

XDL_API void fill_shape_raw(const context_t* pct,const xbrush_t* pxb,int x1,int y1,int x2,int y2,int shape);

#ifdef __cplusplus
}
#endif

#endif /*XDLGDI_H*/