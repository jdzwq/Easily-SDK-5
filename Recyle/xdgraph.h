#ifndef XDRGRAPH_H
#define XDRGRAPH_H

#include "xdrdef.h"
#include "xdrcontext.h"

#ifdef __cplusplus
extern "C" {
#endif

XDR_API void draw_line(context_t* pct,const xpen_t* pxp,int x1,int y1,int x2,int y2);

XDR_API void draw_rect(context_t* pct,const xpen_t* pxp,int x1,int y1,int x2,int y2);

XDR_API void fill_rect(context_t* pct,const xbrush_t* pxb,int x1,int y1,int x2,int y2);

XDR_API void draw_round(context_t* pct,const xpen_t* pxp,int x1,int y1,int x2,int y2,int r);

XDR_API void fill_round(context_t* pct,const xbrush_t* pxb,int x1,int y1,int x2,int y2,int r);

XDR_API void draw_ellipse(context_t* pct,const xpen_t* pxp,int x1,int y1,int x2,int y2);

XDR_API void fill_ellipse(context_t* pct,const xbrush_t* pxb,int x1,int y1,int x2,int y2);

XDR_API void draw_text(context_t* pct,const xfont_t* pxf,int x1,int y1,int x2,int y2,const wchar_t* txt,int len);

#ifdef __cplusplus
}
#endif

#endif /*XDRGRAPH_H*/