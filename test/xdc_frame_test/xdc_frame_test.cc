#include <xdl.h>
#include <xdc.h>

res_win_t g_main = NULL;

res_win_t MainFrame_Create(const tchar_t* mname);

#ifdef _OS_WINDOWS
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
#else
int main(int argc, const char * argv[])
#endif
{

	xdl_process_init(XDL_APARTMENT_PROCESS);

	xdc_process_init();

	g_main = MainFrame_Create(_T("Main"));

	msg_t msg;

	do{
		while (message_peek(&msg))
		{
			message_fetch(&msg, NULL);

			if (!message_translate(&msg))
			{
				message_dispatch(&msg);
			}
		}
	} while (widget_is_valid(g_main));

	xdc_process_uninit();

	xdl_process_uninit();

	return 0;
}

#define MAINFRAME_TOOLBAR_HEIGHT	(float)25
#define MAINFRAME_TREEBAR_WIDTH		(float)50
#define MAINFRAME_TITLEBAR_HEIGHT	(float)8
#define MAINFRAME_STATUSBAR_HEIGHT	(float)8

#define IDC_MAINFRAME				2999
#define IDC_MAINFRAME_TOOLBAR		3000
#define IDC_MAINFRAME_TITLEBAR		3001
#define IDC_MAINFRAME_STATUSBAR		3002
#define IDC_MAINFRAME_TREEBAR		3003
#define IDC_MAINFRAME_OWNERPANEL	3004
#define IDC_MAINFRAME_CALENDARPANEL	3005
#define IDC_MAINFRAME_NOTESPANEL	3006
#define IDC_MAINFRAME_PANELPANEL	3007
#define IDC_MAINFRAME_CURVEPANEL	3008
#define IDC_MAINFRAME_MODELPANEL	3009
#define IDC_MAINFRAME_PLOTPANEL		3010

#define IDA_OWNER			2002
#define IDA_CALENDAR		2003
#define IDA_NOTES			2004
#define IDA_PANEL			2005
#define IDA_CURVE			2006
#define IDA_MODEL			2007
#define IDA_PLOT			2008

#define PANEL_CLASS_OWNER		_T("OWNER")
#define PANEL_CLASS_CALENDAR	_T("CALENDAR")
#define PANEL_CLASS_NOTES		_T("NOTES")
#define PANEL_CLASS_PANEL		_T("PANEL")
#define PANEL_CLASS_CURVE		_T("CURVE")
#define PANEL_CLASS_MODEL		_T("MODEL")
#define PANEL_CLASS_PLOT		_T("PLOT")

#define MAINFRAME_ACCEL_COUNT		1

accel_t	MAINFRAME_ACCEL[MAINFRAME_ACCEL_COUNT] = {
	KEY_CONTROL, _T('O'), IDA_OWNER,
};

typedef struct tagMainFrameDelta{
	res_win_t hToolBar;
	res_win_t hTitleBar;
	res_win_t hTreeBar;
	res_win_t hStatusBar;

	res_win_t hToolTip;
	res_win_t hKeyBox;
	res_win_t hNaviBox;

	bool_t bDirty;
	bool_t bMode;
}MainFrameDelta;

#define GETMAINFRAMEDELTA(widget) 			(MainFrameDelta*)widget_get_user_delta(widget)
#define SETMAINFRAMEDELTA(widget,ptd)		widget_set_user_delta(widget,(var_long)ptd)

void _MainFrame_CalcToolBar(res_win_t widget, xrect_t* pxr)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	widget_get_dock_rect(widget, WS_DOCK_TOP, pxr);
}

void _MainFrame_CalcStatusBar(res_win_t widget, xrect_t* pxr)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	widget_get_dock_rect(widget, WS_DOCK_BOTTOM, pxr);
}

void _MainFrame_CalcTreeBar(res_win_t widget, xrect_t* pxr)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	widget_get_dock_rect(widget, WS_DOCK_LEFT, pxr);
}

void _MainFrame_CalcTitleBar(res_win_t widget, xrect_t* pxr)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);
	xsize_t xs;

	xs.fx = 0;
	xs.fy = MAINFRAME_TITLEBAR_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_dock_rect(widget, 0, pxr);
	pxr->h = xs.cy;
}

void _MainFrame_CalcPanelBar(res_win_t widget, xrect_t* pxr)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);
	xsize_t xs;

	xs.fx = 0;
	xs.fy = MAINFRAME_TITLEBAR_HEIGHT;
	widget_size_to_pt(widget, &xs);

	widget_get_dock_rect(widget, 0, pxr);
	pxr->y += xs.cy;
	pxr->h -= xs.cy;
}

void _MainFrame_CreateToolBar(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	xrect_t xr = { 0 };

	_MainFrame_CalcToolBar(widget, &xr);

	pdt->hToolBar = toolctrl_create(_T("ToolBar"), WD_STYLE_CONTROL | WD_STYLE_HOTOVER, &xr, widget);
	widget_set_user_id(pdt->hToolBar, IDC_MAINFRAME_TOOLBAR);
	widget_set_owner(pdt->hToolBar, widget);

	LINKPTR ptrTool = create_tool_doc();

	LINKPTR glk = insert_tool_group(ptrTool, LINK_LAST);
	set_tool_group_name(glk, _T("TextOnly"));
	set_tool_group_title(glk, _T("1"));
	set_tool_group_show(glk, ATTR_SHOW_TEXTONLY);
	set_tool_group_item_width(glk, 18);
	set_tool_group_item_height(glk, 7);

	tchar_t token[NUM_LEN + 1] = { 0 };

	LINKPTR ilk = insert_tool_group_item(glk, LINK_LAST);
	xsprintf(token, _T("%d"), IDA_OWNER);
	set_tool_item_id(ilk, token);
	set_tool_item_title(ilk, _T("2"));

	glk = insert_tool_group(ptrTool, LINK_LAST);
	set_tool_group_name(glk, _T("ImageOnly"));
	set_tool_group_title(glk, _T("3"));
	set_tool_group_show(glk, ATTR_SHOW_IMAGEONLY);
	set_tool_group_item_width(glk, 7);
	set_tool_group_item_height(glk, 7);

	ilk = insert_tool_group_item(glk, LINK_LAST);
	xsprintf(token, _T("%d"), IDA_OWNER);
	set_tool_item_id(ilk, token);
	set_tool_item_title(ilk, _T("4"));
	set_tool_item_icon(ilk, GDI_ATTR_GIZMO_USER);

	glk = insert_tool_group(ptrTool, LINK_LAST);
	set_tool_group_name(glk, _T("ImageText"));
	set_tool_group_title(glk, _T("5"));
	set_tool_group_item_width(glk, 18);
	set_tool_group_item_height(glk, 7);

	ilk = insert_tool_group_item(glk, LINK_LAST);
	xsprintf(token, _T("%d"), IDA_OWNER);
	set_tool_item_id(ilk, token);
	set_tool_item_title(ilk, _T("6"));
	set_tool_item_icon(ilk, GDI_ATTR_GIZMO_USER);

	ilk = insert_tool_group_item(glk, LINK_LAST);
	xsprintf(token, _T("%d"), IDA_CALENDAR);
	set_tool_item_id(ilk, token);
	set_tool_item_title(ilk, _T("7"));
	set_tool_item_icon(ilk, GDI_ATTR_GIZMO_USER);

	ilk = insert_tool_group_item(glk, LINK_LAST);
	xsprintf(token, _T("%d"), IDA_NOTES);
	set_tool_item_id(ilk, token);
	set_tool_item_title(ilk, _T("8"));
	set_tool_item_icon(ilk, GDI_ATTR_GIZMO_USER);

	ilk = insert_tool_group_item(glk, LINK_LAST);
	xsprintf(token, _T("%d"), IDA_PANEL);
	set_tool_item_id(ilk, token);
	set_tool_item_title(ilk, _T("9"));
	set_tool_item_icon(ilk, GDI_ATTR_GIZMO_USER);

	ilk = insert_tool_group_item(glk, LINK_LAST);
	xsprintf(token, _T("%d"), IDA_CURVE);
	set_tool_item_id(ilk, token);
	set_tool_item_title(ilk, _T("10"));
	set_tool_item_icon(ilk, GDI_ATTR_GIZMO_USER);

	ilk = insert_tool_group_item(glk, LINK_LAST);
	xsprintf(token, _T("%d"), IDA_MODEL);
	set_tool_item_id(ilk, token);
	set_tool_item_title(ilk, _T("hh"));
	set_tool_item_icon(ilk, GDI_ATTR_GIZMO_USER);

	ilk = insert_tool_group_item(glk, LINK_LAST);
	xsprintf(token, _T("%d"), IDA_PLOT);
	set_tool_item_id(ilk, token);
	set_tool_item_title(ilk, _T("11"));
	set_tool_item_icon(ilk, GDI_ATTR_GIZMO_USER);

	toolctrl_attach(pdt->hToolBar, ptrTool);
	widget_show(pdt->hToolBar, WS_SHOW_NORMAL);
}

void _MainFrame_CreateTitleBar(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	xrect_t xr = { 0 };

	_MainFrame_CalcTitleBar(widget, &xr);

	pdt->hTitleBar = titlectrl_create(_T("TitleBar"), WD_STYLE_CONTROL | WD_STYLE_HOTOVER, &xr, widget);
	widget_set_user_id(pdt->hTitleBar, IDC_MAINFRAME_TITLEBAR);
	widget_set_owner(pdt->hTitleBar, widget);

	LINKPTR ptrTitle = create_title_doc();

	set_title_oritation(ptrTitle, ATTR_ORITATION_BOTTOM);

	titlectrl_attach(pdt->hTitleBar, ptrTitle);

	widget_show(pdt->hTitleBar, WS_SHOW_NORMAL);
}

void _MainFrame_CreateTreeBar(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	xrect_t xr = { 0 };

	_MainFrame_CalcTreeBar(widget, &xr);

	pdt->hTreeBar = treectrl_create(_T("TreeBar"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hTreeBar, IDC_MAINFRAME_TREEBAR);
	widget_set_owner(pdt->hTreeBar, widget);

	LINKPTR ptrTree = create_tree_doc();

	set_tree_title(ptrTree, _T("��Դ�б�"));
	set_tree_title_icon(ptrTree, GDI_ATTR_GIZMO_PROPER);
	treectrl_attach(pdt->hTreeBar, ptrTree);
	treectrl_set_lock(pdt->hTreeBar, 0);

	widget_show(pdt->hTreeBar, WS_SHOW_NORMAL);
}

void _MainFrame_CreateStatusBar(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	xrect_t xr = { 0 };

	_MainFrame_CalcStatusBar(widget, &xr);

	pdt->hStatusBar = statusctrl_create(_T("StatusBar"), WD_STYLE_CONTROL, &xr, widget);
	widget_set_user_id(pdt->hStatusBar, IDC_MAINFRAME_STATUSBAR);
	widget_set_owner(pdt->hStatusBar, widget);

	LINKPTR ptrStatus = create_status_doc();

	set_status_alignment(ptrStatus, ATTR_ALIGNMENT_FAR);

	LINKPTR ilk;

	ilk = insert_status_item(ptrStatus, LINK_LAST);
	set_status_item_name(ilk, _T("navibox"));
	set_status_item_title(ilk, _T("������"));
	set_status_item_width(ilk, DEF_TOUCH_SPAN * 6 + 1);

	statusctrl_attach(pdt->hStatusBar, ptrStatus);

	widget_show(pdt->hStatusBar, WS_SHOW_NORMAL);

	ilk = get_status_item(ptrStatus, _T("navibox"));
	statusctrl_get_item_rect(pdt->hStatusBar, ilk, &xr);

	pdt->hNaviBox = navibox_create(pdt->hStatusBar, WD_STYLE_CONTROL, &xr);
	widget_set_owner(pdt->hNaviBox, pdt->hStatusBar);
	widget_show(pdt->hNaviBox, WS_SHOW_NORMAL);
}

void _MainFrame_DestroyToolBar(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	LINKPTR ptrTool = toolctrl_detach(pdt->hToolBar);
	if (ptrTool)
		destroy_tool_doc(ptrTool);
	widget_destroy(pdt->hToolBar);
}

void _MainFrame_DestroyTitleBar(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	LINKPTR ptrTitle = titlectrl_detach(pdt->hTitleBar);
	if (ptrTitle)
		destroy_title_doc(ptrTitle);
	widget_destroy(pdt->hTitleBar);
}

void _MainFrame_DestroyTreeBar(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	LINKPTR ptrTree = treectrl_detach(pdt->hTreeBar);
	if (ptrTree)
		destroy_tree_doc(ptrTree);
	widget_destroy(pdt->hTreeBar);
}

void _MainFrame_DestroyStatusBar(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	LINKPTR ptrStatus = statusctrl_detach(pdt->hStatusBar);
	if (ptrStatus)
		destroy_status_doc(ptrStatus);
	widget_destroy(pdt->hStatusBar);
}

res_win_t _MainFrame_CreatePanel(res_win_t widget, const tchar_t* wclass)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	res_win_t hPanel = NULL;
	xrect_t xr = { 0 };

	link_t_ptr ptr_title, ilk;

	_MainFrame_CalcPanelBar(widget, &xr);

	tchar_t wname[RES_LEN + 1] = { 0 };

	if (compare_text(wclass, -1, PANEL_CLASS_OWNER, -1, 0) == 0)
	{
		if (is_null(wname))
			xscpy(wname, _T("NewUser"));

		hPanel = ownerctrl_create(_T("OwnerPanel"), WD_STYLE_CONTROL, &xr, widget);
		widget_set_user_id(hPanel, IDC_MAINFRAME_OWNERPANEL);
		widget_set_owner(hPanel, widget);
		ownerctrl_redraw(hPanel);
	}
	else if (compare_text(wclass, -1, PANEL_CLASS_CALENDAR, -1, 0) == 0)
	{
		if (is_null(wname))
			xscpy(wname, _T("NewCalendar"));

		hPanel = calendarctrl_create(_T("CalendarPanel"), WD_STYLE_CONTROL, &xr, widget);
		widget_set_user_id(hPanel, IDC_MAINFRAME_CALENDARPANEL);
		widget_set_owner(hPanel, widget);

		LINKPTR ptr_calendar = create_calendar_doc();

		xdate_t dt;
		get_loc_date(&dt);
		tchar_t sz_date[DATE_LEN];
		format_date(&dt, sz_date);
		set_calendar_today(ptr_calendar, sz_date);

		calendarctrl_attach(hPanel, ptr_calendar);
		calendarctrl_redraw(hPanel);
	}
	else if (compare_text(wclass, -1, PANEL_CLASS_NOTES, -1, 0) == 0)
	{
		if (is_null(wname))
			xscpy(wname, _T("NewCalendar"));

		hPanel = notesctrl_create(_T("NotesPanel"), WD_STYLE_CONTROL, &xr, widget);
		widget_set_user_id(hPanel, IDC_MAINFRAME_NOTESPANEL);
		widget_set_owner(hPanel, widget);

		LINKPTR ptr_arch = create_arch_doc();

		LINKPTR ptr_notes = create_notes_doc();
		set_notes_time(ptr_notes, _T("2019-11-25 10:00:00"));
		set_notes_text(ptr_notes, _T("������Ϣ1"), -1);
		insert_arch_document(ptr_arch, LINK_LAST, ptr_notes);

		ptr_notes = create_notes_doc();
		set_notes_time(ptr_notes, _T("2019-11-26 10:00:00"));
		set_notes_text(ptr_notes, _T("������Ϣ2"), -1);
		insert_arch_document(ptr_arch, LINK_LAST, ptr_notes);

		ptr_notes = create_notes_doc();
		set_notes_time(ptr_notes, _T("2019-11-27 10:00:00"));
		set_notes_text(ptr_notes, _T("������Ϣ3"), -1);
		insert_arch_document(ptr_arch, LINK_LAST, ptr_notes);

		notesctrl_attach(hPanel, ptr_arch);
		notesctrl_redraw(hPanel);
	}
	else if (compare_text(wclass, -1, PANEL_CLASS_PANEL, -1, 0) == 0)
	{
		if (is_null(wname))
			xscpy(wname, _T("NewPanel"));

		hPanel = panelctrl_create(_T("PanelPanel"), WD_STYLE_CONTROL, &xr, widget);
		widget_set_user_id(hPanel, IDC_MAINFRAME_PANELPANEL);
		widget_set_owner(hPanel, widget);

		LINKPTR ptr_arch = create_arch_doc();

		LINKPTR ptr_notes = create_notes_doc();
		set_notes_time(ptr_notes, _T("2019-11-25 10:00:00"));
		set_notes_text(ptr_notes, _T("������Ϣ1"), -1);
		insert_arch_document(ptr_arch, LINK_LAST, ptr_notes);

		ptr_notes = create_notes_doc();
		set_notes_time(ptr_notes, _T("2019-11-26 10:00:00"));
		set_notes_text(ptr_notes, _T("������Ϣ2"), -1);
		insert_arch_document(ptr_arch, LINK_LAST, ptr_notes);

		ptr_notes = create_notes_doc();
		set_notes_time(ptr_notes, _T("2019-11-27 10:00:00"));
		set_notes_text(ptr_notes, _T("������Ϣ3"), -1);
		insert_arch_document(ptr_arch, LINK_LAST, ptr_notes);

		notesctrl_attach(hPanel, ptr_arch);
		notesctrl_redraw(hPanel);
	}
	else if (compare_text(wclass, -1, PANEL_CLASS_CURVE, -1, 0) == 0)
	{
		if (is_null(wname))
			xscpy(wname, _T("NewCurve"));

		hPanel = curvectrl_create(_T("CurvePanel"), WD_STYLE_CONTROL, &xr, widget);
		widget_set_user_id(hPanel, IDC_MAINFRAME_CURVEPANEL);
		widget_set_owner(hPanel, widget);
		curvectrl_redraw(hPanel);
	}
	else if (compare_text(wclass, -1, PANEL_CLASS_MODEL, -1, 0) == 0)
	{
		if (is_null(wname))
			xscpy(wname, _T("NewModel"));

		hPanel = modelctrl_create(_T("ModelPanel"), WD_STYLE_CONTROL, &xr, widget);
		widget_set_user_id(hPanel, IDC_MAINFRAME_MODELPANEL);
		widget_set_owner(hPanel, widget);

		LINKPTR ptr_anno = create_anno_doc();

		modelctrl_attach(hPanel, ptr_anno);
		modelctrl_redraw(hPanel);
	}
	else if (compare_text(wclass, -1, PANEL_CLASS_PLOT, -1, 0) == 0)
	{
		if (is_null(wname))
			xscpy(wname, _T("NewPlot"));

		hPanel = plotbox_create(widget, WD_STYLE_CONTROL, &xr);
		widget_set_user_id(hPanel, IDC_MAINFRAME_PLOTPANEL);
		widget_set_owner(hPanel, widget);

		xpen_t xp = { 0 };
		widget_get_xpen(hPanel, &xp);
		xp.adorn.feed = 2;
		xp.adorn.size = 1;
		//widget_set_xpen(hPanel, &xp);

		xbrush_t xb = { 0 };
		widget_get_xbrush(hPanel, &xb);
		xb.shadow.offx = 3;
		xb.shadow.offy = 3;
		widget_set_xbrush(hPanel, &xb);

		vector_t* pvt = vector_alloc(5, 2);
		vector_parse(pvt, _T(" {(0,1) (2,3)(4, 5) (6, 7) (8,9)}"), -1);
		plotbox_set_vetor(hPanel, *pvt);
		vector_free(pvt);

		plot_t plt = { 0 };
		plt.y_base = 5;
		plt.y_step = 1;
		plt.x_step = 1;
		parse_xcolor(&(plt.clr_argv[0]), GDI_ATTR_RGB_RED);
		parse_xcolor(&(plt.clr_argv[1]), GDI_ATTR_RGB_GREEN);
		parse_xcolor(&(plt.clr_argv[2]), GDI_ATTR_RGB_BLUE);
		parse_xcolor(&(plt.clr_argv[3]), GDI_ATTR_RGB_GRAY);
		parse_xcolor(&(plt.clr_argv[4]), GDI_ATTR_RGB_CYAN);

		xscpy(plt.x_unit, _T("mmHg"));
		xscpy(plt.y_unit, _T("��"));
		plotbox_set_title(hPanel, _T("Ѫѹ����ͼ"));
		plotbox_set_plot(hPanel, &plt);

		plotbox_set_type(hPanel, ATTR_PLOT_TYPE_PANTOGRAM);
		plotbox_calc_plot(hPanel);
	}

	if (!hPanel)
		return NULL;

	ptr_title = titlectrl_fetch(pdt->hTitleBar);

	ilk = insert_title_item(ptr_title, LINK_LAST);

	set_title_item_name(ilk, wclass);
	set_title_item_title(ilk, wname);
	set_title_item_delta(ilk, (var_long)hPanel);

	titlectrl_redraw(pdt->hTitleBar);
	titlectrl_set_focus_item(pdt->hTitleBar, ilk);

	return hPanel;
}

res_win_t _MainFrame_GetActivePanel(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	XDL_ASSERT(pdt != NULL);

	LINKPTR nlk = titlectrl_get_focus_item(pdt->hTitleBar);
	if (!nlk)
		return NULL;

	return (res_win_t)get_title_item_delta(nlk);
}

/*******************************************************************************************************/
void MainFrame_ToolBar_OnLBClick(res_win_t widget, NOTICE_TOOL* pnt)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);
	xrect_t xr = { 0 };
	xpoint_t pt = { 0 };

	if (!pnt->item)
		return;

	long nID = xstol(get_tool_item_id_ptr(pnt->item));

	toolctrl_get_item_rect(pdt->hToolBar, pnt->item, &xr);
	pt.x = xr.x;
	pt.y = xr.y + xr.h;
	widget_client_to_screen(pdt->hToolBar, &pt);

	widget_send_command(widget, 0, nID, (var_long)&pt);
}

void MainFrame_ToolBar_OnItemHover(res_win_t widget, NOTICE_TOOL* pnt)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	xpoint_t xp, *ppt;
	ppt = (xpoint_t*)pnt->data;

	xp.x = ppt->x + 10;
	xp.y = ppt->y + 10;

	widget_client_to_screen(pnt->widget, &xp);

	if (widget_is_valid(pdt->hToolTip))
		reset_toolbox(pdt->hToolTip, &xp, get_tool_item_title_ptr(pnt->item));
	else
		pdt->hToolTip = show_toolbox(&xp, get_tool_item_title_ptr(pnt->item));
}

void MainFrame_TitleBar_OnItemInsert(res_win_t widget, NOTICE_TITLE* pnt)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);
}

void MainFrame_TitleBar_OnItemDelete(res_win_t widget, NOTICE_TITLE* pnt)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	res_win_t hPanel = (res_win_t)get_title_item_delta(pnt->item);

	if (!widget_is_valid(hPanel))
		return;

	const tchar_t* wclass = get_title_item_name_ptr(pnt->item);
	LINKPTR ptrDoc = NULL;

	if (compare_text(wclass, -1, PANEL_CLASS_CALENDAR, -1, 0) == 0)
	{
		ptrDoc = calendarctrl_fetch(hPanel);
	}
	else if (compare_text(wclass, -1, PANEL_CLASS_NOTES, -1, 0) == 0)
	{
		ptrDoc = notesctrl_fetch(hPanel);
	}
	else if (compare_text(wclass, -1, PANEL_CLASS_PANEL, -1, 0) == 0)
	{
		ptrDoc = panelctrl_fetch(hPanel);
	}
	else if (compare_text(wclass, -1, PANEL_CLASS_MODEL, -1, 0) == 0)
	{
		ptrDoc = modelctrl_fetch(hPanel);
	}

	widget_close(hPanel, 0);

	if (widget_is_valid(hPanel))
	{
		pnt->ret = 1;
		return;
	}

	if (compare_text(wclass, -1, PANEL_CLASS_CALENDAR, -1, 0) == 0)
	{
		destroy_calendar_doc(ptrDoc);
	}
	else if (compare_text(wclass, -1, PANEL_CLASS_NOTES, -1, 0) == 0)
	{
		destroy_arch_doc(ptrDoc);
	}
	else if (compare_text(wclass, -1, PANEL_CLASS_PANEL, -1, 0) == 0)
	{
		destroy_arch_doc(ptrDoc);
	}
	else if (compare_text(wclass, -1, PANEL_CLASS_MODEL, -1, 0) == 0)
	{
		destroy_anno_doc(ptrDoc);
	}
}

void MainFrame_TitleBar_OnItemChanging(res_win_t widget, NOTICE_TITLE* pnt)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	res_win_t hPanel = (res_win_t)get_title_item_delta(pnt->item);

	if (widget_is_valid(hPanel))
	{
		widget_show(hPanel, WS_SHOW_HIDE);
	}
}

void MainFrame_TitleBar_OnItemChanged(res_win_t widget, NOTICE_TITLE* pnt)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	res_win_t hPanel = (res_win_t)get_title_item_delta(pnt->item);

	if (widget_is_valid(hPanel))
	{
		clr_mod_t clr;
		widget_get_color_mode(widget, &clr);

		widget_set_color_mode(hPanel, &clr);

		widget_show(hPanel, WS_SHOW_NORMAL);
	}
}

void MainFrame_TitleBar_OnItemHover(res_win_t widget, NOTICE_TITLE* pnt)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	xpoint_t xp, *ppt;
	ppt = (xpoint_t*)pnt->data;

	xp.x = ppt->x + 10;
	xp.y = ppt->y + 10;

	widget_client_to_screen(pnt->widget, &xp);

	if (widget_is_valid(pdt->hToolTip))
		reset_toolbox(pdt->hToolTip, &xp, get_title_item_title_ptr(pnt->item));
	else
		pdt->hToolTip = show_toolbox(&xp, get_title_item_title_ptr(pnt->item));
}

/*void MainFrame_UserPanel_OnCalc(res_win_t win, PAGE_CALC* ppc)
{
	ppc->total_height = 8096;
	ppc->total_width = 4096;
	ppc->line_height = 10;
	ppc->line_width = 10;
}*/

void MainFrame_UserPanel_OnDraw(res_win_t win, res_ctx_t rdc)
{
	viewbox_t vb;
	xcolor_t xc;

	widget_get_view_rect(win, &vb);

	//parse_xcolor(&xc, GDI_ATTR_RGB_BLUE);

	//test_icon_draw(hDC, &xc, (xrect_t*)&vb);

	tchar_t aa[10] = { 0 };
	xpoint_t pa[20] = { 0 };

	int i = 0;
	int n = 0;
	int feed = 10;

	xpen_t xp;
	widget_get_xpen(win, &xp);
	xbrush_t xb;
	widget_get_xbrush(win, &xb);
	lighten_xbrush(&xb, DEF_HARD_DARKEN);

	xrect_t xr;
	widget_get_client_rect(win, &xr);

	xr.w -= 10;
	xr.h = 50;

	aa[i] = _T('M');
	pa[n].x = xr.x;
	pa[n].y = xr.y + feed;
	i++;
	n++;

	aa[i] = _T('A');
	pa[n].x = feed;
	pa[n].y = feed;
	pa[n+1].x = xr.x + feed;
	pa[n+1].y = xr.y;
	i++;
	n+=2;

	aa[i] = _T('L');
	pa[n].x = xr.x + xr.w - feed;
	pa[n].y = xr.y;
	i++;
	n++;

	aa[i] = _T('A');
	pa[n].x = feed;
	pa[n].y = feed;
	pa[n + 1].x = xr.x + xr.w;
	pa[n + 1].y = xr.y + feed;
	i++;
	n += 2;

	aa[i] = _T('L');
	pa[n].x = xr.x + xr.w;
	pa[n].y = xr.y + xr.h - feed;
	i++;
	n++;

	aa[i] = _T('A');
	pa[n].x = feed;
	pa[n].y = feed;
	pa[n + 1].x = xr.x + xr.w - feed;
	pa[n + 1].y = xr.y + xr.h;
	i++;
	n += 2;

	aa[i] = _T('C');
	pa[n].x = xr.x + xr.w / 8 * 7;
	pa[n].y = xr.y + xr.h - 10;
	pa[n + 1].x = xr.x + xr.w / 4 * 3;
	pa[n + 1].y = xr.y + xr.h - 10;
	pa[n + 2].x = xr.x + xr.w / 2;
	pa[n + 2].y = xr.y + xr.h;
	i++;
	n += 3;

	aa[i] = _T('S');
	pa[n].x = xr.x + xr.w / 4;
	pa[n].y = xr.y + xr.h;
	pa[n + 1].x = xr.x + feed;
	pa[n + 1].y = xr.y + xr.h;
	i++;
	n += 2;
	
	aa[i] = _T('A');
	pa[n].x = feed;
	pa[n].y = feed;
	pa[n + 1].x = xr.x;
	pa[n + 1].y = xr.y + xr.h - feed;
	i++;
	n += 2;

	aa[i] = _T('Z');
	i++;

	xp.adorn.feed = 0;
	xp.adorn.size = 0;
	xb.shadow.offx = 10;
	xb.shadow.offy = 10;

	draw_path_raw(rdc, &xp, &xb, aa, pa, n);

	xr.y = 60;
	xr.x = 10;
	xr.w = 50;
	xr.h = 50;

	xp.adorn.feed = 0;
	xp.adorn.size = 0;
	xb.shadow.offx = 5;
	xb.shadow.offy = 5;
	draw_rect_raw(rdc, &xp, &xb, &xr);

	xr.y = 60;
	xr.x = 80;
	xr.w = 50;
	xr.h = 50;

	xp.adorn.feed = 0;
	xp.adorn.size = 0;
	xb.shadow.offx = 5;
	xb.shadow.offy = 5;
	draw_round_raw(rdc, &xp, &xb, &xr);

	xr.y = 60;
	xr.x = 150;
	xr.w = 50;
	xr.h = 50;

	xp.adorn.feed = 0;
	xp.adorn.size = 0;
	xb.shadow.offx = 5;
	xb.shadow.offy = 5;
	draw_ellipse_raw(rdc, &xp, &xb, &xr);

	xr.y = 60;
	xr.x = 220;
	xr.w = 50;
	xr.h = 50;

	xpoint_t pt;
	pt.x = xr.x + xr.w / 2;
	pt.y = xr.y + xr.h / 2;
	long rx = xr.w / 2;
	long ry = xr.h / 2;

	xp.adorn.feed = 0;
	xp.adorn.size = 0;
	xb.shadow.offx = 5;
	xb.shadow.offy = 5;
	draw_pie_raw(rdc, &xp, &xb, &pt, rx, ry, 0, XPI);

	xr.y = 60;
	xr.x = 280;
	xr.w = 50;
	xr.h = 50;

	pt.x = xr.x + xr.w / 2;
	pt.y = xr.y + xr.h / 2;
	rx = xr.w / 2;
	ry = xr.h / 2;

	xp.adorn.feed = 1;
	xp.adorn.size = 1;
	xb.shadow.offx = 0;
	xb.shadow.offy = 0;
	draw_arc_raw(rdc, &xp, &pt, rx, ry, 0, XPI);

	xr.y = 60;
	xr.x = 340;
	xr.w = 50;
	xr.h = 50;

	xp.adorn.feed = 0;
	xp.adorn.size = 0;
	xb.shadow.offx = 5;
	xb.shadow.offy = 5;
	draw_arrow_raw(rdc, &xp, &xb, &xr, 20, XPI / 3);

}

/******************************************************************************************************/

int MainFrame_OnCreate(res_win_t widget, void* data)
{
	MainFrameDelta* pdt;

	widget_hand_create(widget);

	res_acl_t hac = create_accel_table(MAINFRAME_ACCEL, MAINFRAME_ACCEL_COUNT);

	widget_attach_accel(widget, hac);

	xsize_t xs;

	xs.fx = 0;
	xs.fy = MAINFRAME_TOOLBAR_HEIGHT;
	widget_size_to_pt(widget, &xs);
	widget_dock(widget, WS_DOCK_TOP, 0, xs.cy);

	xs.fx = 0;
	xs.fy = MAINFRAME_STATUSBAR_HEIGHT;
	widget_size_to_pt(widget, &xs);
	widget_dock(widget, WS_DOCK_BOTTOM, 0, xs.cy);

	xs.fx = MAINFRAME_TREEBAR_WIDTH;
	xs.fy = 0;
	widget_size_to_pt(widget, &xs);
	widget_dock(widget, WS_DOCK_LEFT | WS_DOCK_DYNA, xs.cx, 0);

	pdt = (MainFrameDelta*)xmem_alloc(sizeof(MainFrameDelta));
	SETMAINFRAMEDELTA(widget, pdt);

	_MainFrame_CreateToolBar(widget);

	//_MainFrame_CreateTitleBar(widget);

	//_MainFrame_CreateStatusBar(widget);

	//_MainFrame_CreateTreeBar(widget);

	return 0;
}

void MainFrame_OnDestroy(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	res_acl_t hac = widget_get_accel(widget);
	if (hac)
		destroy_accel_table(hac);

	_MainFrame_DestroyToolBar(widget);

	_MainFrame_DestroyTitleBar(widget);

	_MainFrame_DestroyTreeBar(widget);

	_MainFrame_DestroyStatusBar(widget);

	if (widget_is_valid(pdt->hToolTip))
		widget_destroy(pdt->hToolTip);

	if (widget_is_valid(pdt->hKeyBox))
		widget_destroy(pdt->hKeyBox);

	xmem_free(pdt);

	widget_hand_destroy(widget);
}

int MainFrame_OnClose(res_win_t widget)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	LINKPTR ptrTitle = titlectrl_fetch(pdt->hTitleBar);
	LINKPTR plk;

	while (plk = titlectrl_get_focus_item(pdt->hTitleBar))
	{
		if (!titlectrl_delete_item(pdt->hTitleBar, plk))
			break;
	}

	if (get_title_item_count(ptrTitle))
		return 1;


	widget_destroy(widget);

	send_quit_message(0);

	return 0;
}

void MainFrame_OnSize(res_win_t widget, int code, const xsize_t* pxs)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	xrect_t xr;

	_MainFrame_CalcToolBar(widget, &xr);
	if(widget_is_valid(pdt->hToolBar))
	{
		widget_move(pdt->hToolBar, RECTPOINT(&xr));
		widget_size(pdt->hToolBar, RECTSIZE(&xr));
		widget_update(pdt->hToolBar);
	}

	_MainFrame_CalcStatusBar(widget, &xr);
	if(widget_is_valid(pdt->hStatusBar))
	{
		widget_move(pdt->hStatusBar, RECTPOINT(&xr));
		widget_size(pdt->hStatusBar, RECTSIZE(&xr));
		widget_update(pdt->hStatusBar);
	}

	_MainFrame_CalcTreeBar(widget, &xr);
	if(widget_is_valid(pdt->hTreeBar))
	{
		widget_move(pdt->hTreeBar, RECTPOINT(&xr));
		widget_size(pdt->hTreeBar, RECTSIZE(&xr));
		widget_update(pdt->hTreeBar);
	}

	_MainFrame_CalcTitleBar(widget, &xr);
	if(widget_is_valid(pdt->hTitleBar))
	{
		widget_move(pdt->hTitleBar, RECTPOINT(&xr));
		widget_size(pdt->hTitleBar, RECTSIZE(&xr));
		widget_update(pdt->hTitleBar);
	}

	_MainFrame_CalcPanelBar(widget, &xr);

	if(widget_is_valid(pdt->hTitleBar))
	{
		LINKPTR ptrTitle = titlectrl_fetch(pdt->hTitleBar);
		LINKPTR plk = get_title_next_item(ptrTitle, LINK_FIRST);
		while (plk)
		{
			res_win_t hPanel = (res_win_t)get_title_item_delta(plk);

			if (widget_is_valid(hPanel))
			{
				widget_move(hPanel, RECTPOINT(&xr));
				widget_size(hPanel, RECTSIZE(&xr));
				widget_update(hPanel);
			}

			plk = get_title_next_item(ptrTitle, plk);
		}
	}

	widget_erase(widget, NULL);
}

void MainFrame_OnScroll(res_win_t widget, bool_t bHorz, int nLine)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	widget_hand_scroll(widget, bHorz, nLine);
}

void MainFrame_OnMenuCommand(res_win_t widget, int code, int cid, var_long data)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	res_win_t hPanel = _MainFrame_GetActivePanel(widget);
	if (hPanel && !code)
	{
		if (widget_send_command(hPanel, code, cid, data))
			return;
	}

	switch (cid)
	{
	case IDA_OWNER:
		_MainFrame_CreatePanel(widget, PANEL_CLASS_OWNER);
		break;
	case IDA_CALENDAR:
		_MainFrame_CreatePanel(widget, PANEL_CLASS_CALENDAR);
		break;
	case IDA_NOTES:
		_MainFrame_CreatePanel(widget, PANEL_CLASS_NOTES);
		break;
	case IDA_PANEL:
		_MainFrame_CreatePanel(widget, PANEL_CLASS_PANEL);
		break;
	case IDA_CURVE:
		_MainFrame_CreatePanel(widget, PANEL_CLASS_CURVE);
		break;
	case IDA_PLOT:
		_MainFrame_CreatePanel(widget, PANEL_CLASS_PLOT);
		break;
	}
}

void MainFrame_OnNotice(res_win_t widget, LPNOTICE phdr)
{
	MainFrameDelta* pdt = GETMAINFRAMEDELTA(widget);

	if (phdr->id == IDC_MAINFRAME_TOOLBAR)
	{
		NOTICE_TOOL* pnt = (NOTICE_TOOL*)phdr;
		switch (pnt->code)
		{
		case NC_TOOLLBCLK:
			MainFrame_ToolBar_OnLBClick(widget, pnt);
			break;
		case NC_TOOLITEMHOVER:
			MainFrame_ToolBar_OnItemHover(widget, pnt);
			break;
		}
	}
	else if (phdr->id == IDC_MAINFRAME_TITLEBAR)
	{
		NOTICE_TITLE* pnt = (NOTICE_TITLE*)phdr;
		switch (pnt->code)
		{
		case NC_TITLEITEMCHANGING:
			MainFrame_TitleBar_OnItemChanging(widget, pnt);
			break;
		case NC_TITLEITEMCHANGED:
			MainFrame_TitleBar_OnItemChanged(widget, pnt);
			break;
		case NC_TITLEITEMINSERT:
			MainFrame_TitleBar_OnItemInsert(widget, pnt);
			break;
		case NC_TITLEITEMDELETE:
			MainFrame_TitleBar_OnItemDelete(widget, pnt);
			break;
		case NC_TITLEITEMHOVER:
			MainFrame_TitleBar_OnItemHover(widget, pnt);
			break;
		}
	}
	else if (phdr->id == IDC_MAINFRAME_TREEBAR)
	{
		NOTICE_TREE* pnt = (NOTICE_TREE*)phdr;
		switch (pnt->code)
		{
		case NC_TREELBCLK:
			break;
		case NC_TREEDBCLK:
			break;
		case NC_TREERBCLK:
			break;
		case NC_TREEITEMEDITING:
			break;
		case NC_TREEITEMCOMMIT:
			break;
		case NC_TREEITEMUPDATE:
			break;
		}
	}
	else if (phdr->id == IDC_MAINFRAME_STATUSBAR)
	{
		NOTICE_STATUS* pnt = (NOTICE_STATUS*)phdr;
		switch (pnt->code)
		{
		case NC_STATUSLBCLK:
			break;
		}
	}
	else if (phdr->id == IDC_MAINFRAME_OWNERPANEL)
	{
		NOTICE_OWNER* pnu = (NOTICE_OWNER*)phdr;
		switch (pnu->code)
		{
		case NC_OWNERCALC:
			//MainFrame_UserPanel_OnCalc(pnu->widget, (PAGE_CALC*)pnu->data);
			break;
		case NC_OWNERDRAW:
			MainFrame_UserPanel_OnDraw(pnu->widget, (res_ctx_t)pnu->data);
			break;
		}
	}
}

res_win_t MainFrame_Create(const tchar_t* mname)
{
	res_win_t widget;
	xrect_t xr = { 0 };

	if_event_t ev = { 0 };

	EVENT_BEGIN_DISPATH(&ev)

		EVENT_ON_CREATE(MainFrame_OnCreate)
		EVENT_ON_DESTROY(MainFrame_OnDestroy)
		EVENT_ON_CLOSE(MainFrame_OnClose)

		EVENT_ON_SIZE(MainFrame_OnSize)
		EVENT_ON_SCROLL(MainFrame_OnScroll)

		EVENT_ON_NOTICE(MainFrame_OnNotice)
		EVENT_ON_MENU_COMMAND(MainFrame_OnMenuCommand)

		EVENT_ON_NC_IMPLEMENT
		EVENT_ON_DOCKER_IMPLEMENT
		EVENT_ON_DOCKER_IMPLEMENT

	SUBPROC_END_DISPATH

	get_desktop_size(RECTSIZE(&xr));

	widget = widget_create(_T("TEST"), WD_STYLE_FRAME | WD_STYLE_DOCKER | WD_STYLE_MENUBAR | WD_STYLE_OWNERNC, &xr, NULL, &ev);

	if (!widget)
	{
		return 0;
	}

	widget_show(widget, WS_SHOW_NORMAL);
	widget_update(widget);

	return widget;
}


