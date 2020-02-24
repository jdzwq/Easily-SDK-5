#include "stdafx.h"
#include "EasilyModule.h"
#include "EasilySplitor.h"

void	EasilySplitor_OnItemSizing(res_win_t widget, link_t_ptr ilk, short x, short y, LPSPLITOR ptd);
void	EasilySplitor_OnItemSized(res_win_t widget, short x, short y, LPSPLITOR ptd);
void	EasilySplitor_OnSize(res_win_t widget, short cx, short cy, LPSPLITOR ptd);
void	EasilySplitor_OnPaint(res_win_t widget, res_dc_t dc, const xrect_t* pxr, LPSPLITOR ptd);
void	EasilySplitor_OnMouseMove(res_win_t widget, short x, short y, LPSPLITOR ptd);
void	EasilySplitor_OnLButtonDown(res_win_t widget, short x, short y, LPSPLITOR ptd);
void	EasilySplitor_OnLButtonUp(res_win_t widget, short x, short y, LPSPLITOR ptd);

LRESULT CALLBACK EasilySplitorProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		EasilySplitor_OnLButtonDown(hWnd, (short)LOWORD(lParam), (short)HIWORD(lParam), (LPSPLITOR)dwRefData);
		break;
	case WM_LBUTTONUP:
		EasilySplitor_OnLButtonUp(hWnd, (short)LOWORD(lParam), (short)HIWORD(lParam), (LPSPLITOR)dwRefData);
		break;
	case WM_MOUSEMOVE:
		EasilySplitor_OnMouseMove(hWnd, (short)LOWORD(lParam), (short)HIWORD(lParam), (LPSPLITOR)dwRefData);
		break;
	case WM_SIZE:
		EasilySplitor_OnSize(hWnd, LOWORD(lParam), HIWORD(lParam), (LPSPLITOR)dwRefData);
		break;
	case WM_ERASEBKGND:
	/*{
		RECT rtBack;
		GetClientRect(hWnd, &rtBack);

		xrect_t xr;
		xr.x = rtBack.left;
		xr.y = rtBack.top;
		xr.w = rtBack.right - rtBack.left;
		xr.h = rtBack.bottom - rtBack.top;

		xscpy(xn.brush_color, g_face[g_face_index].bkg);
		xscpy(xn.pen_color, g_face[g_face_index].frg);
		xscpy(xn.text_color, g_face[g_face_index].txt);

		hand_split_erase(hWnd,(HDC)wParam, &xr, &xn);
	}*/
		return 0;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		RECT rtFront;

		if (GetUpdateRect(hWnd, &rtFront, FALSE))
		{
			xrect_t pat;
			pat.x = rtFront.left;
			pat.y = rtFront.top;
			pat.w = rtFront.right - rtFront.left;
			pat.h = rtFront.bottom - rtFront.top;

			BeginPaint(hWnd, &ps);

			EasilySplitor_OnPaint(hWnd, ps.hdc, &pat, (LPSPLITOR)dwRefData);

			EndPaint(hWnd, &ps);
		}
	}
		break;
	}
	return DefSubclassProc(hWnd, message, wParam, lParam);
}

/*************************************************************************************************/

int _SizeSplitorWindow(link_t_ptr ilk, void* pv)
{
	LPSPLITOR ptd = (LPSPLITOR)pv;
	res_win_t win;
	xrect_t xr;

	if (get_split_item_splited(ilk))
		return 0;

	xr.x = get_split_item_x(ilk);
	xr.w = get_split_item_width(ilk);
	xr.y = get_split_item_y(ilk);
	xr.h = get_split_item_height(ilk);

	win = (res_win_t)get_split_item_widget(ilk);

	widget_rect_to_pt(win, &xr);

	if (widget_is_valid(win))
	{
		widget_resize(win, &xr);
	}

	return 0;
}

void _ResetSplitorPage(res_win_t widget, LPSPLITOR ptd)
{
	xrect_t xr;

	widget_get_client_rect(widget, &xr);

	widget_rect_to_tm(widget, &xr);

	set_split_item_x(ptd->split, 0);
	set_split_item_y(ptd->split, 0);
	set_split_item_width(ptd->split, xr.w);
	set_split_item_height(ptd->split, xr.h);
}

/************************************************************************************/

void EasilySplitor_OnItemSizing(res_win_t widget, link_t_ptr ilk, short x, short y, LPSPLITOR ptd)
{
	bool_t bHorz;

	ptd->org_item = ilk;
	ptd->org_x = x;
	ptd->org_y = y;

	bHorz = (compare_text(get_split_item_layer_ptr(ptd->org_item), -1, ATTR_LAYER_HORZ, -1, 1) == 0) ? 1 : 0;
	if (bHorz)
		widget_set_cursor(widget, CURS_SIZENS);
	else
		widget_set_cursor(widget, CURS_SIZEWE);

}

void EasilySplitor_OnItemSized(res_win_t widget, short x, short y, LPSPLITOR ptd)
{
	long cx, cy, h, w, m;
	float r;
	bool_t bHorz;
	xrect_t xr;
	xpoint_t pt_org, pt_cur;
	link_t_ptr ilk;
	tchar_t token[NUM_LEN + 1];

	ilk = ptd->org_item;
	ptd->org_item = NULL;

	pt_org.x = ptd->org_x;
	pt_org.y = ptd->org_y;
	pt_cur.x = x;
	pt_cur.y = y;

	widget_point_to_tm(widget, &pt_org);
	widget_point_to_tm(widget, &pt_cur);

	bHorz = (compare_text(get_split_item_layer_ptr(ilk), -1, ATTR_LAYER_HORZ, -1, 1) == 0) ? 1 : 0;

	if (bHorz)
	{
		if (y == ptd->org_y)
			return;

		cy = pt_cur.y - pt_org.y;
		r = (float)(xstof(get_split_item_ratio_ptr(ilk)));
		h = get_split_item_height(ilk);

		if (r > 1)
			m = (long)r + cy;
		else
			m = (long)(r * h) + cy;

		if (m > h)
			m = h;

		xsprintf(token, _T("%.2f"), (float)m / (float)h);

		set_split_item_ratio(ilk, token);
	}
	else
	{
		if (x == ptd->org_x)
			return;

		cx = pt_cur.x - pt_org.x;
		r = (float)(xstof(get_split_item_ratio_ptr(ilk)));
		w = get_split_item_width(ilk);

		if (r > 1)
			m = (long)r + cx;
		else
			m = (long)(r * w) + cx;

		if (m > w)
			m = w;

		xsprintf(token, _T("%.2f"), (float)m / (float)w);

		set_split_item_ratio(ilk, token);
	}

	resize_split_item(ilk);
	enum_split_item(ilk, (PF_LINKENUM)_SizeSplitorWindow, (void*)ptd);

	calc_split_bar_rect(ptd->split, ilk, &xr);

	widget_rect_to_pt(widget, &xr);

	widget_invalid_rect(widget, &xr, 0);
}

void EasilySplitor_OnPaint(res_win_t widget, res_dc_t rdc, const xrect_t* pxr, LPSPLITOR ptd)
{
	link_t_ptr ilk;
	link_t_ptr st = NULL;
	xrect_t xr;
	long span;
	xbrush_t xb = { 0 };
	xgradi_t xg_horz, xg_vert;
	float r;
	long m;

	const tchar_t *style;

	if (!ptd)
		return;

	widget_get_client_rect(widget, &xr);

	style = get_split_style_ptr(ptd->split);

	default_xbrush(&xb);
	parse_xbrush_from_style(&xb, style);
	xscpy(xb.color, g_face[g_face_index].bkg);

	default_xgradi(&xg_vert);
	xscpy(xg_vert.color, xb.color);
	xscpy(xg_vert.type, GDI_ATTR_GRADIENT_TYPE_HORZ);

	default_xgradi(&xg_horz);
	xscpy(xg_horz.color, xb.color);
	xscpy(xg_vert.type, GDI_ATTR_GRADIENT_TYPE_VERT);

	ilk = ptd->split;
	while (ilk)
	{
		xr.x = get_split_item_x(ilk);
		xr.y = get_split_item_y(ilk);
		xr.w = get_split_item_width(ilk);
		xr.h = get_split_item_height(ilk);
		span = get_split_item_span(ilk);
		r = (float)(xstof(get_split_item_ratio_ptr(ilk)));

		if (get_split_item_splited(ilk))
		{
			if (compare_text(get_split_item_layer_ptr(ilk), -1, ATTR_LAYER_HORZ, -1, 1) == 0)
			{
				if (r > 1)
					m = ((long)r < xr.h) ? (long)r : xr.h;
				else
					m = (long)(r * xr.h);

				xr.y += (m - span);
				xr.h = 2 * span;
			}
			else
			{
				if (r > 1)
					m = ((long)r < xr.w) ? (long)r : xr.w;
				else
					m = (long)(r * xr.w);

				xr.x += (m - span);
				xr.w = 2 * span;
			}

			widget_rect_to_pt(widget, &xr);

			if (compare_text(get_split_item_layer_ptr(ilk), -1, ATTR_LAYER_HORZ, -1, 1) == 0)
				gradient_rect_raw(rdc, &xg_horz, &xr);
			else
				gradient_rect_raw(rdc, &xg_vert, &xr);
		}

		if (get_split_item_splited(ilk))
		{
			if (!st)
				st = create_stack_table();

			push_stack_node(st, (void*)ilk);

			ilk = get_split_first_child_item(ilk);
			continue;
		}

		while (ilk)
		{
			ilk = get_split_next_sibling_item(ilk);
			if (ilk)
			{
				break;
			}

			ilk = (st) ? (link_t_ptr)pop_stack_node(st) : NULL;
		}
	}

	if (st)
		destroy_stack_table(st);
}

void EasilySplitor_OnSize(res_win_t widget, short cx, short cy, LPSPLITOR ptd)
{
	_ResetSplitorPage(widget, ptd);

	resize_split_item(ptd->split);
	enum_split_item(ptd->split, (PF_LINKENUM)_SizeSplitorWindow, (void*)ptd);

	widget_invalid_rect(widget, NULL, 0);
}

void EasilySplitor_OnMouseMove(res_win_t widget, short x, short y, LPSPLITOR ptd)
{
	bool_t bHorz;
	link_t_ptr plk;
	int nHint;
	xpoint_t pt;

	pt.x = x;
	pt.y = y;

	widget_point_to_tm(widget, &pt);

	nHint = calc_split_hint(ptd->split, &pt, &plk);
	if (plk && get_split_item_fixed(plk))
	{
		return;
	}

	if (nHint == SPLIT_HINT_BAR)
	{
		bHorz = (compare_text(get_split_item_layer_ptr(plk), -1, ATTR_LAYER_HORZ, -1, 1) == 0) ? 1 : 0;
		if (bHorz)
			widget_set_cursor(widget, CURS_SIZENS);
		else
			widget_set_cursor(widget, CURS_SIZEWE);
	}
	else
	{
		widget_set_cursor(widget, NULL);
	}
}

void EasilySplitor_OnLButtonDown(res_win_t widget, short x, short y, LPSPLITOR ptd)
{
	int nHint;
	link_t_ptr ilk;
	xpoint_t pt;

	pt.x = x;
	pt.y = y;

	widget_point_to_tm(widget, &pt);

	nHint = calc_split_hint(ptd->split, &pt, &ilk);
	if (nHint == SPLIT_HINT_BAR)
	{
		if (get_split_item_fixed(ilk))
			return;

		widget_set_capture(widget, 1);

		EasilySplitor_OnItemSizing(widget, ilk, x, y, ptd);
	}
}

void EasilySplitor_OnLButtonUp(res_win_t widget, short x, short y, LPSPLITOR ptd)
{
	widget_set_capture(widget, 0);

	if (ptd->org_item)
	{
		EasilySplitor_OnItemSized(widget, x, y, ptd);
	}
}


