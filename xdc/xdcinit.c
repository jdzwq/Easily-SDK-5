/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc initialize document

	@module	xdcinit.c | implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
	@devnote 张文权 2018.01 - 2018.12	v5.5
***********************************************************************/

/**********************************************************************
This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
LICENSE.GPL3 for more details.
***********************************************************************/


#include "xdcinit.h"

xdc_mou_t g_xdc_mou = { 0 };


//mount system call
void xdc_process_init()
{
	if (g_xdc_mou.if_ok)
		return;

    g_xdc_mou.if_ok = 1;


#ifdef XDU_SUPPORT_BLUT
	xdu_impl_blut(&g_xdc_mou.if_blut);
#endif

#ifdef XDU_SUPPORT_CONTEXT
	xdu_impl_context(&g_xdc_mou.if_context);

	if (g_xdc_mou.if_context.pf_context_startup)
	{
		g_xdc_mou.ctx_ver = (*g_xdc_mou.if_context.pf_context_startup)();
	}

#ifdef XDU_SUPPORT_CONTEXT_BITMAP
	xdu_impl_context_bitmap(&g_xdc_mou.if_context);
#endif //XDU_SUPPORT_CONTEXT_BITMAP

#ifdef XDU_SUPPORT_CONTEXT_PRINTER
	xdu_impl_context_printer(&g_xdc_mou.if_context);
#endif
#ifdef XDU_SUPPORT_CONTEXT_REGION
	xdu_impl_context_region(&g_xdc_mou.if_context);
#endif

	xdu_impl_context_graphic(&g_xdc_mou.if_context);

#endif //XDU_SUPPORT_CONTEXT

#ifdef XDU_SUPPORT_CLIPBOARD
	xdu_impl_clipboard(&g_xdc_mou.if_clipboard);
#endif //XDU_SUPPORT_CLIPBOARD

#ifdef XDU_SUPPORT_SHELL
	xdu_impl_shell(&g_xdc_mou.if_shell);
#ifdef XDU_SUPPORT_SHELL_DIALOG
	xdu_impl_shell_dialog(&g_xdc_mou.if_shell);
#endif
#endif //XDU_SUPPORT_SHELL

#ifdef XDU_SUPPORT_WIDGET
	xdu_impl_widget(&g_xdc_mou.if_widget);
	//start widget context
	if (g_xdc_mou.if_widget.pf_widget_startup)
	{
		(*(g_xdc_mou.if_widget.pf_widget_startup))(g_xdc_mou.ctx_ver);
	}
#endif //XDU_SUPPORT_WIDGET
}

//unmount system call
void xdc_process_uninit()
{
	if (!g_xdc_mou.if_ok)
		return;

#ifdef XDU_SUPPORT_CONTEXT
	//stop gdi context
	(*g_xdc_mou.if_context.pf_context_cleanup)();
#endif

#ifdef XDU_SUPPORT_WIDGET
	//stop widget context
	(*g_xdc_mou.if_widget.pf_widget_cleanup)();
#endif

	memset((void*)&g_xdc_mou, 0, sizeof(xdc_mou_t));
}



