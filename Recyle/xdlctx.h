#ifndef XDLCTX_H
#define XDLCTX_H

#include "xdldef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define INCHPERMM	0.0394
#define MMPERINCH	25.4
#define PTPERINCH	1440
#define PTPERMM		56.693

typedef enum{
	CONTEXT_DISPLAY = 0,
	CONTEXT_PRINTER = 1
}CONTEXT_TYPE;

typedef struct _context_t{
	int			ct;
	handle_t	hdc;
	int			scale;
	int			xoff;
	int			yoff;
}context_t;

XDL_API void		initial_context(void);

XDL_API void		uninitial_context(void);

XDL_API context_t*	create_printer_context(const wchar_t* devname);

XDL_API void		destroy_printer_context(context_t* pct);

XDL_API context_t*	create_dispaly_context(const wchar_t* devname);

XDL_API void		destroy_dispaly_context(context_t* pct);

XDL_API int			mm_to_pt(const context_t* pct,float mm,bool_t horz);

XDL_API float		pt_to_mm(const context_t* pct,int pt,bool_t horz);


#if defined(_WINDOWS)
#define	get_windows_dc(pct)		((HDC)pct->hdc)
#endif

#ifdef __cplusplus
}
#endif

#endif /*XDLCTX_H*/