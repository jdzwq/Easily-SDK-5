/***********************************************************************
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
	visual_t rdc; 
	canvas_t canv;
	if_drawing_t ifc = {0};
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

	get_canvas_interface(canv, &ifc);
	
	ifc.rect.fx = 0;
	ifc.rect.fy = 0;
	ifc.rect.fw = get_canvas_horz_size(canv);
	ifc.rect.fh = get_canvas_vert_size(canv);

	pages = calc_form_pages(&ifc, form);

	begin_doc(rdc, _T("FORM"));

	for (i = 0; i < pages; i++)
	{
		begin_page(rdc);

		draw_form_page(&ifc, form, i+1);

		end_page(rdc);
	}

	end_doc(rdc);

	
	destroy_printer_canvas(canv);
	destroy_printer_context(rdc);
}

void print_grid(const dev_prn_t* pdev, link_t_ptr grid)
{
	visual_t rdc;
	canvas_t canv;
	if_drawing_t ifc = {0};
	int i, pages;
	dev_prn_t dev = { 0 };

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

	get_canvas_interface(canv, &ifc);
	

	ifc.rect.fx = 0;
	ifc.rect.fy = 0;
	ifc.rect.fw = get_canvas_horz_size(canv);
	ifc.rect.fh = get_canvas_vert_size(canv);

	pages = calc_grid_pages(grid);

	begin_doc(rdc, _T("GRID"));

	for (i = 0; i < pages; i++)
	{
		begin_page(rdc);

		draw_grid_page(&ifc, grid, i+1);

		end_page(rdc);
	}

	end_doc(rdc);

	
	destroy_printer_canvas(canv);
	destroy_printer_context(rdc);
}

void print_statis(const dev_prn_t* pdev, link_t_ptr statis)
{
	visual_t rdc;
	canvas_t canv;
	if_drawing_t ifc = {0};
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

	get_canvas_interface(canv, &ifc);
	

	ifc.rect.fx = 0;
	ifc.rect.fy = 0;
	ifc.rect.fw = get_canvas_horz_size(canv);
	ifc.rect.fh = get_canvas_vert_size(canv);

	pages = calc_statis_pages(statis);

	begin_doc(rdc, _T("STATIS"));

	for (i = 0; i < pages; i++)
	{
		begin_page(rdc);

		draw_statis_page(&ifc, statis, i + 1);

		end_page(rdc);
	}

	end_doc(rdc);

	
	destroy_printer_canvas(canv);
	destroy_printer_context(rdc);
}

void print_topog(const dev_prn_t* pdev, link_t_ptr topog)
{
	visual_t rdc;
	canvas_t canv;
	if_drawing_t ifc = {0};

	rdc = create_printer_context(pdev);
	if (!rdc)
		return;

	canv = create_printer_canvas(rdc);
	XDL_ASSERT(canv != NULL);

	get_canvas_interface(canv, &ifc);
	

	ifc.rect.fx = 0;
	ifc.rect.fy = 0;
	ifc.rect.fw = get_canvas_horz_size(canv);
	ifc.rect.fh = get_canvas_vert_size(canv);

	begin_doc(rdc, _T("TOPOG"));

	begin_page(rdc);

	draw_topog(&ifc, topog);

	end_page(rdc);

	end_doc(rdc);

	
	destroy_printer_canvas(canv);
	destroy_printer_context(rdc);
}

void print_dialog(const dev_prn_t* pdev, link_t_ptr dialog)
{
	visual_t rdc;
	canvas_t canv;
	if_drawing_t ifc = {0};

	rdc = create_printer_context(pdev);
	if (!rdc)
		return;

	canv = create_printer_canvas(rdc);
	XDL_ASSERT(canv != NULL);

	get_canvas_interface(canv, &ifc);
	

	ifc.rect.fx = 0;
	ifc.rect.fy = 0;
	ifc.rect.fw = get_canvas_horz_size(canv);
	ifc.rect.fh = get_canvas_vert_size(canv);

	begin_doc(rdc, _T("DIALOG"));

	begin_page(rdc);

	draw_dialog(&ifc, dialog);

	end_page(rdc);

	end_doc(rdc);

	
	destroy_printer_canvas(canv);
	destroy_printer_context(rdc);
}

void print_diagram(const dev_prn_t* pdev, link_t_ptr diagram)
{
	visual_t rdc;
	canvas_t canv;
	if_drawing_t ifc = {0};
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

	get_canvas_interface(canv, &ifc);
	

	ifc.rect.fx = 0; 
	ifc.rect.fy = 0;
	ifc.rect.fw = get_canvas_horz_size(canv);
	ifc.rect.fh = get_canvas_vert_size(canv);

	begin_doc(rdc, _T("DIAGRAM"));

	begin_page(rdc);

	draw_diagram(&ifc, diagram);

	end_page(rdc);

	end_doc(rdc);

	
	destroy_printer_canvas(canv);
	destroy_printer_context(rdc);
}

void print_memo(const dev_prn_t* pdev, const xfont_t* pxf, const xface_t* pxa, link_t_ptr memo)
{
	visual_t rdc;
	canvas_t canv;
	if_drawing_t ifc = {0};
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

	get_canvas_interface(canv, &ifc);
	

	xr.fw = get_canvas_horz_size(canv);
	xr.fh = get_canvas_vert_size(canv);

	pages = calc_memo_pages(&ifc, pxf, pxa, &xr, memo);

	begin_doc(rdc, _T("MEMO"));

	for (i = 0; i < pages; i++)
	{
		begin_page(rdc);

		draw_memo_text(&ifc, pxf, pxa, &xr, memo, i + 1);

		end_page(rdc);
	}

	end_doc(rdc);

	
	destroy_printer_canvas(canv);
	destroy_printer_context(rdc);
}

void print_rich(const dev_prn_t* pdev, const xfont_t* pxf, const xface_t* pxa, link_t_ptr rich)
{
	visual_t rdc;
	canvas_t canv;
	if_drawing_t ifc = {0};
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

	get_canvas_interface(canv, &ifc);
	

	xr.fw = get_canvas_horz_size(canv);
	xr.fh = get_canvas_vert_size(canv);

	pages = calc_rich_pages(&ifc, pxf, pxa, &xr, rich);

	begin_doc(rdc, _T("RICH"));

	for (i = 0; i < pages; i++)
	{
		begin_page(rdc);

		draw_rich_text(&ifc, pxf, pxa, &xr, rich, i + 1);

		end_page(rdc);
	}

	end_doc(rdc);

	
	destroy_printer_canvas(canv);
	destroy_printer_context(rdc);
}

void print_svg(const dev_prn_t* pdev, link_t_ptr svg)
{
	xrect_t xr = { 0 };
	visual_t rdc;
	canvas_t canv;
	if_drawing_t ifc = {0};

	rdc = create_printer_context(pdev);
	if (!rdc)
		return;

	canv = create_printer_canvas(rdc);
	
	get_canvas_interface(canv, &ifc);

	xr.fw = get_canvas_horz_size(canv);
	xr.fh = get_canvas_vert_size(canv);

	begin_doc(rdc, _T("SVG"));

	begin_page(rdc);

	draw_svg(&ifc, &xr, svg);

	end_page(rdc);

	end_doc(rdc);

	
	destroy_printer_canvas(canv);
	destroy_printer_context(rdc);
}



#endif //
