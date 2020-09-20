﻿/***********************************************************************
	Easily xdc v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc utility document

	@module	xdcutil.c | implement file

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

#include "printbag.h"
#include "xdcimp.h"

#ifdef XDU_SUPPORT_CONTEXT_PRINTER

void print_form(const dev_prn_t* pdev, link_t_ptr form)
{
	res_ctx_t rdc; 
	canvas_t canv;
	if_canvas_t* pic;
	canvbox_t cb = { 0 };
	int i, pages;
	dev_prn_t dev = { 0 };

	if (pdev)
	{
		xmem_copy((void*)&dev, (void*)pdev, sizeof(dev_prn_t));
	}
	else
	{
		dev.paper_height = (short)(get_form_height(form) * 10);
		dev.paper_width = (short)(get_form_width(form) * 10);
		dev.landscape = (compare_text(get_form_printing_ptr(form), -1, ATTR_PRINTING_LANDSCAPE, -1, 0) == 0) ? 1 : 0;
	}

	rdc = create_printer_context(&dev);
	if (!rdc)
		return;

	canv = create_printer_canvas(rdc);
	XDL_ASSERT(canv != NULL);

	cb.fw = get_canvas_horz_size(canv);
	cb.fh = get_canvas_vert_size(canv);

	pic = create_canvas_interface(canv);
	XDL_ASSERT(pic != NULL);

	pages = calc_form_pages(canv, form);

	begin_doc(rdc, _T("FORM"));

	for (i = 0; i < pages; i++)
	{
		begin_page(rdc);

		draw_form_page(pic, &cb, form, i+1);

		end_page(rdc);
	}

	end_doc(rdc);

	destroy_canvas_interface(pic);
	destroy_printer_canvas(canv);
	destroy_printer_context(rdc);
}

void print_grid(const dev_prn_t* pdev, link_t_ptr grid)
{
	res_ctx_t rdc;
	canvas_t canv;
	if_canvas_t* pic;
	canvbox_t cb = { 0 };
	int i, pages;
	dev_prn_t dev = { 0 };
	xsize_t xs;

	if (pdev)
	{
		xmem_copy((void*)&dev, (void*)pdev, sizeof(dev_prn_t));
	}
	else
	{
		dev.paper_height = (short)(get_grid_height(grid) * 10);
		dev.paper_width = (short)(get_grid_width(grid) * 10);
		dev.landscape = (compare_text(get_grid_printing_ptr(grid), -1, ATTR_PRINTING_LANDSCAPE, -1, 0) == 0) ? 1 : 0;
	}

	rdc = create_printer_context(&dev);
	if (!rdc)
		return;

	canv = create_printer_canvas(rdc);
	XDL_ASSERT(canv != NULL);

	cb.fw = get_canvas_horz_size(canv);
	cb.fh = get_canvas_vert_size(canv);

	pic = create_canvas_interface(canv);
	XDL_ASSERT(pic != NULL);

	xs.fx = get_canvas_horz_size(canv);
	xs.fy = get_canvas_vert_size(canv);

	pages = calc_grid_pages(&cb, grid);

	begin_doc(rdc, _T("GRID"));

	for (i = 0; i < pages; i++)
	{
		begin_page(rdc);

		draw_grid_page(pic, &cb, grid, i+1);

		end_page(rdc);
	}

	end_doc(rdc);

	destroy_canvas_interface(pic);
	destroy_printer_canvas(canv);
	destroy_printer_context(rdc);
}

void print_statis(const dev_prn_t* pdev, link_t_ptr statis)
{
	res_ctx_t rdc;
	canvas_t canv;
	if_canvas_t* pic;
	canvbox_t cb = { 0 };
	dev_prn_t dev = { 0 };
	int i, pages;

	if (pdev)
	{
		xmem_copy((void*)&dev, (void*)pdev, sizeof(dev_prn_t));
	}
	else
	{
		dev.paper_height = (short)(get_statis_height(statis) * 10);
		dev.paper_width = (short)(get_statis_width(statis) * 10);
		dev.landscape = (compare_text(get_statis_printing_ptr(statis), -1, ATTR_PRINTING_LANDSCAPE, -1, 0) == 0) ? 1 : 0;
	}

	rdc = create_printer_context(pdev);
	if (!rdc)
		return;

	canv = create_printer_canvas(rdc);
	XDL_ASSERT(canv != NULL);

	cb.fw = get_canvas_horz_size(canv);
	cb.fh = get_canvas_vert_size(canv);

	pic = create_canvas_interface(canv);
	XDL_ASSERT(pic != NULL);

	pages = calc_statis_pages(&cb, statis);

	begin_doc(rdc, _T("STATIS"));

	for (i = 0; i < pages; i++)
	{
		begin_page(rdc);

		draw_statis_page(pic, &cb, statis, i + 1);

		end_page(rdc);
	}

	end_doc(rdc);

	destroy_canvas_interface(pic);
	destroy_printer_canvas(canv);
	destroy_printer_context(rdc);
}

void print_topog(const dev_prn_t* pdev, link_t_ptr topog)
{
	res_ctx_t rdc;
	canvas_t canv;
	if_canvas_t* pic;
	canvbox_t cb = { 0 };

	rdc = create_printer_context(pdev);
	if (!rdc)
		return;

	canv = create_printer_canvas(rdc);
	XDL_ASSERT(canv != NULL);

	cb.fw = get_canvas_horz_size(canv);
	cb.fh = get_canvas_vert_size(canv);

	pic = create_canvas_interface(canv);
	XDL_ASSERT(pic != NULL);

	begin_doc(rdc, _T("TOPOG"));

	begin_page(rdc);

	draw_topog(pic, &cb, topog);

	end_page(rdc);

	end_doc(rdc);

	destroy_canvas_interface(pic);
	destroy_printer_canvas(canv);
	destroy_printer_context(rdc);
}

void print_dialog(const dev_prn_t* pdev, link_t_ptr dialog)
{
	res_ctx_t rdc;
	canvas_t canv;
	if_canvas_t* pic;
	canvbox_t cb = { 0 };

	rdc = create_printer_context(pdev);
	if (!rdc)
		return;

	canv = create_printer_canvas(rdc);
	XDL_ASSERT(canv != NULL);

	cb.fw = get_canvas_horz_size(canv);
	cb.fh = get_canvas_vert_size(canv);

	pic = create_canvas_interface(canv);
	XDL_ASSERT(pic != NULL);

	begin_doc(rdc, _T("DIALOG"));

	begin_page(rdc);

	draw_dialog(pic, &cb, dialog);

	end_page(rdc);

	end_doc(rdc);

	destroy_canvas_interface(pic);
	destroy_printer_canvas(canv);
	destroy_printer_context(rdc);
}

void print_diagram(const dev_prn_t* pdev, link_t_ptr diagram)
{
	res_ctx_t rdc;
	canvas_t canv;
	if_canvas_t* pic;
	canvbox_t cb = { 0 };
	dev_prn_t dev = { 0 };

	if (pdev)
	{
		xmem_copy((void*)&dev, (void*)pdev, sizeof(dev_prn_t));
	}
	else
	{
		dev.paper_height = (short)(get_diagram_height(diagram) * 10);
		dev.paper_width = (short)(get_diagram_width(diagram) * 10);
		dev.landscape = (compare_text(get_diagram_printing_ptr(diagram), -1, ATTR_PRINTING_LANDSCAPE, -1, 0) == 0) ? 1 : 0;
	}

	rdc = create_printer_context(pdev);
	if (!rdc)
		return;

	canv = create_printer_canvas(rdc);
	XDL_ASSERT(canv != NULL);

	cb.fw = get_canvas_horz_size(canv);
	cb.fh = get_canvas_vert_size(canv);

	pic = create_canvas_interface(canv);
	XDL_ASSERT(pic != NULL);

	begin_doc(rdc, _T("DIAGRAM"));

	begin_page(rdc);

	draw_diagram(pic, &cb, diagram);

	end_page(rdc);

	end_doc(rdc);

	destroy_canvas_interface(pic);
	destroy_printer_canvas(canv);
	destroy_printer_context(rdc);
}

void print_memo(const dev_prn_t* pdev, const xfont_t* pxf, const xface_t* pxa, link_t_ptr memo)
{
	res_ctx_t rdc;
	canvas_t canv;
	if_canvas_t* pic;
	xrect_t xr = { 0 };
	int i, pages;
	dev_prn_t dev = { 0 };

	if (pdev)
	{
		xmem_copy((void*)&dev, (void*)pdev, sizeof(dev_prn_t));
	}
	else
	{
		dev.paper_height = (short)(get_memo_height(memo) * 10);
		dev.paper_width = (short)(get_memo_width(memo) * 10);
	}

	rdc = create_printer_context(pdev);
	if (!rdc)
		return;

	canv = create_printer_canvas(rdc);
	XDL_ASSERT(canv != NULL);

	pic = create_canvas_interface(canv);
	XDL_ASSERT(pic != NULL);

	xr.fw = get_canvas_horz_size(canv);
	xr.fh = get_canvas_vert_size(canv);

	pages = (*pic->pf_calc_memo_pages)(pic->canvas, pxf, pxa, &xr, memo);

	begin_doc(rdc, _T("MEMO"));

	for (i = 0; i < pages; i++)
	{
		begin_page(rdc);

		(*pic->pf_draw_memo_text)(pic->canvas, pxf, pxa, &xr, memo, i + 1);

		end_page(rdc);
	}

	end_doc(rdc);

	destroy_canvas_interface(pic);
	destroy_printer_canvas(canv);
	destroy_printer_context(rdc);
}

void print_rich(const dev_prn_t* pdev, const xfont_t* pxf, const xface_t* pxa, link_t_ptr rich)
{
	res_ctx_t rdc;
	canvas_t canv;
	if_canvas_t* pic;
	xrect_t xr = { 0 };
	int i, pages;
	dev_prn_t dev = { 0 };

	if (pdev)
	{
		xmem_copy((void*)&dev, (void*)pdev, sizeof(dev_prn_t));
	}
	else
	{
		dev.paper_height = (short)(get_rich_height(rich) * 10);
		dev.paper_width = (short)(get_rich_width(rich) * 10);
	}

	rdc = create_printer_context(pdev);
	if (!rdc)
		return;

	canv = create_printer_canvas(rdc);
	XDL_ASSERT(canv != NULL);

	pic = create_canvas_interface(canv);
	XDL_ASSERT(pic != NULL);

	xr.fw = get_canvas_horz_size(canv);
	xr.fh = get_canvas_vert_size(canv);

	pages = (*pic->pf_calc_rich_pages)(pic->canvas, pxf, pxa, &xr, rich);

	begin_doc(rdc, _T("RICH"));

	for (i = 0; i < pages; i++)
	{
		begin_page(rdc);

		(*pic->pf_draw_rich_text)(pic->canvas, pxf, pxa, &xr, rich, i + 1);

		end_page(rdc);
	}

	end_doc(rdc);

	destroy_canvas_interface(pic);
	destroy_printer_canvas(canv);
	destroy_printer_context(rdc);
}

void print_svg(const dev_prn_t* pdev, link_t_ptr svg)
{
	res_ctx_t rdc;
	canvas_t canv;
	xrect_t xr = { 0 };

	rdc = create_printer_context(pdev);
	if (!rdc)
		return;

	canv = create_printer_canvas(rdc);
	XDL_ASSERT(canv != NULL);

	xr.fw = get_canvas_horz_size(canv);
	xr.fh = get_canvas_vert_size(canv);

	begin_doc(rdc, _T("SVG"));

	begin_page(rdc);

	draw_svg(canv, &xr, svg);

	end_page(rdc);

	end_doc(rdc);

	destroy_printer_canvas(canv);
	destroy_printer_context(rdc);
}



#endif //
