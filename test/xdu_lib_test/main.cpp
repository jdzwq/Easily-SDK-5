//
//  main.cpp
//  xdu_lib_test
//
//  Created by 张文权 on 18/10/4.
//  Copyright © 2018年 张文权. All rights reserved.
//

#include <iostream>
#include <xdu.h>

if_context_t if_context = {0};
if_widget_t if_widget = {0};
res_win_t g_main = NULL;

int pop_on_create(res_win_t wt, void* param)
{
    printf("pop on_create\n");

    return 0;
}

void pop_on_destroy(res_win_t wt)
{
    printf("pop on_destroy\n");
}

void pop_on_mouse_move(res_win_t wt, dword_t ms, const xpoint_t* ppt)
{
    if(ms & MS_WITH_LBUTTON)
        printf("pop on_mouse_move x:%d y:%d with lbutton\n", ppt->x, ppt->y);
    else if(ms & MS_WITH_RBUTTON)
        printf("pop on_mouse_move x:%d y:%d with rbutton\n", ppt->x, ppt->y);
    else
        printf("pop on_mouse_move x:%d y:%d\n", ppt->x, ppt->y);
}

void pop_on_mouse_hover(res_win_t wt, dword_t ms, const xpoint_t* ppt)
{
    if(ms & MS_WITH_LBUTTON)
        printf("pop on_mouse_hover x:%d y:%d with lbutton\n", ppt->x, ppt->y);
    else if(ms & MS_WITH_RBUTTON)
        printf("pop on_mouse_hover x:%d y:%d with rbutton\n", ppt->x, ppt->y);
    else
        printf("pop on_mouse_hover x:%d y:%d\n", ppt->x, ppt->y);
}

void pop_on_mouse_leave(res_win_t wt, dword_t ms, const xpoint_t* ppt)
{
    if(ms & MS_WITH_LBUTTON)
        printf("pop on_mouse_leave x:%d y:%d with lbutton\n", ppt->x, ppt->y);
    else if(ms & MS_WITH_RBUTTON)
        printf("pop on_mouse_leave x:%d y:%d with rbutton\n", ppt->x, ppt->y);
    else
        printf("pop on_mouse_leave x:%d y:%d\n", ppt->x, ppt->y);
}

void pop_on_lbutton_up(res_win_t wt, const xpoint_t* ppt)
{
    printf("pop on_lbutton_up x:%d y:%d\n", ppt->x, ppt->y);

    res_win_t par =  (*if_widget.pf_widget_get_parent)(wt);
    (*if_widget.pf_widget_post_command)(par, 1, 10, 100);

    (*if_widget.pf_widget_destroy)(wt);
}

/****************************************************************************/
int child_on_create(res_win_t wt, void* param)
{
    printf("child on_create\n");

    return 0;
}

void child_on_destroy(res_win_t wt)
{
    printf("child on_destroy\n");
}

void child_on_mouse_move(res_win_t wt, dword_t ms, const xpoint_t* ppt)
{
    if(ms & MS_WITH_LBUTTON)
        printf("child on_mouse_move x:%d y:%d with lbutton\n", ppt->x, ppt->y);
    else if(ms & MS_WITH_RBUTTON)
        printf("child on_mouse_move x:%d y:%d with rbutton\n", ppt->x, ppt->y);
    else
        printf("child on_mouse_move x:%d y:%d\n", ppt->x, ppt->y);
}

void child_on_mouse_hover(res_win_t wt, dword_t ms, const xpoint_t* ppt)
{
    if(ms & MS_WITH_LBUTTON)
        printf("child on_mouse_hover x:%d y:%d with lbutton\n", ppt->x, ppt->y);
    else if(ms & MS_WITH_RBUTTON)
        printf("child on_mouse_hover x:%d y:%d with rbutton\n", ppt->x, ppt->y);
    else
        printf("child on_mouse_hover x:%d y:%d\n", ppt->x, ppt->y);
}

void child_on_mouse_leave(res_win_t wt, dword_t ms, const xpoint_t* ppt)
{
    if(ms & MS_WITH_LBUTTON)
        printf("child on_mouse_leave x:%d y:%d with lbutton\n", ppt->x, ppt->y);
    else if(ms & MS_WITH_RBUTTON)
        printf("child on_mouse_leave x:%d y:%d with rbutton\n", ppt->x, ppt->y);
    else
        printf("child on_mouse_leave x:%d y:%d\n", ppt->x, ppt->y);
}

void child_on_lbutton_down(res_win_t wt, const xpoint_t* ppt)
{
    printf("child on_lbutton_down x:%d y:%d\n", ppt->x, ppt->y);

    (*if_widget.pf_widget_set_focus)(wt);
}

void child_on_lbutton_up(res_win_t wt, const xpoint_t* ppt)
{
    printf("child on_lbutton_up x:%d y:%d\n", ppt->x, ppt->y);

    (*if_widget.pf_widget_post_char)(0, 'A');

    (*if_widget.pf_widget_erase)(wt, NULL);
}

void child_on_lbutton_dbclick(res_win_t wt, const xpoint_t* ppt)
{
    printf("child on_lbutton_dbclick x:%d y:%d\n", ppt->x, ppt->y);
}

void child_on_rbutton_up(res_win_t wt, const xpoint_t* ppt)
{
    printf("child on_rbutton_up x:%d y:%d\n", ppt->x, ppt->y);

     if_event_t ev = {0};

    ev.pf_on_create = pop_on_create;
	ev.pf_on_destroy = pop_on_destroy;
    ev.pf_on_mouse_move = pop_on_mouse_move;
	ev.pf_on_mouse_hover = pop_on_mouse_hover;
	ev.pf_on_mouse_leave = pop_on_mouse_leave;
    ev.pf_on_lbutton_up = pop_on_lbutton_up;

    xrect_t xr;
    xr.x = ppt->x;
    xr.y = ppt->y;
    xr.w = 200;
    xr.h = 300;

    res_win_t pop = (*if_widget.pf_widget_create)(_T("popup"),WD_STYLE_POPUP,&xr,g_main,&ev);

    (*if_widget.pf_widget_show)(pop, 0);

    (*if_widget.pf_widget_take)(pop, WS_TAKE_TOPMOST);

    (*if_widget.pf_widget_do_trace)(pop);
}

void child_on_keydown(res_win_t wt, dword_t ks, int key)
{
    if(ks & KS_WITH_CONTROL)
        printf("child on_key_down keycode:%02x with control\n", key);
    else if(ks & KS_WITH_SHIFT)
        printf("child on_key_down keycode:%02x with shift\n", key);
    else
        printf("child on_key_down keycode:%02x\n", key);
}

void child_on_set_focus(res_win_t wt, res_win_t from)
{
    printf("child on_set_focus \n");
}

void child_on_kill_focus(res_win_t wt, res_win_t to)
{
    printf("child on_kill_focus \n");
}

void child_on_menu_command(res_win_t wt, int code, int cid, var_long data)
{
    printf("child on_menu_command child id: %d code: %d \n", cid, code);
}

void child_on_parent_command(res_win_t, int code, var_long data)
{
    printf("child on_parent_command code: %d \n", code);
}

void child_on_paint(res_win_t wt, res_ctx_t rdc, const xrect_t* prt)
{
    printf("child on_paint the rect is x:%d y:%d w:%d h:%d \n", prt->x, prt->y, prt->w, prt->h);

    res_ctx_t mem = (*if_context.pf_create_compatible_context)(rdc, 100, 100);

    xpen_t xp = {0};
    default_xpen(&xp);
    xscpy(xp.color, GDI_ATTR_RGB_GREEN);

    xbrush_t xb = {0};
    default_xbrush(&xb);
    xscpy(xb.color, GDI_ATTR_RGB_GRAY);

    xrect_t xr;
    xr.x = 0;
    xr.y = 0;
    xr.w = 100;
    xr.h = 98;

    (*if_context.pf_gdi_draw_rect)(mem, &xp, &xb, &xr);

    xscpy(xb.color, GDI_ATTR_RGB_BLUE);

    xpoint_t pt;
    pt.x = xr.x + xr.w / 2;
    pt.y = xr.y + xr.h / 2;
    (*if_context.pf_gdi_draw_pie)(mem, &xp, &xb, &pt, xr.w / 2, xr.h / 2, 0, XPI / 2);

    xscpy(xp.size,"2");
    xscpy(xp.color, GDI_ATTR_RGB_YELLOW);
    (*if_context.pf_gdi_draw_arc)(mem, &xp, &pt, xr.w / 2, xr.h / 2, XPI, XPI * 3 / 2);

    xscpy(xp.size,"1");
    xscpy(xb.color, GDI_ATTR_RGB_GREEN);
    xr.x = 10;
    xr.y = 10;
    xr.w = 10;
    xr.h = 10;
    (*if_context.pf_gdi_draw_arrow)(mem, &xp, &xb, &xr, 20, XPI / 8);

    xpoint_t xp1,xp2;
    xp1.x = 10;
    xp1.y = 10;
    xp2.x = 80;
    xp2.y = 80;

    xscpy(xp.color, GDI_ATTR_RGB_GRAY);
    (*if_context.pf_gdi_draw_line)(mem, &xp, &xp1, &xp2);

    xfont_t xf = {0};
    default_xfont(&xf);
    xscpy(xf.size,"20");
    xscpy(xf.color,GDI_ATTR_RGB_LIGHTCYAN);

    xr.x = 20;
    xr.y = 20;
    xr.w = 100;
    xr.h = 20;
     (*if_context.pf_gdi_draw_text)(mem, &xf, NULL, &xr, "Hello World!", -1);

    xsize_t xs = {0};
    (*if_context.pf_gdi_text_size)(mem, &xf, "Hello World!", -1, &xs);
    (*if_context.pf_gdi_text_metric)(mem, &xf,&xs);

    (*if_context.pf_render_context)(mem, 0,0, rdc, 100, 100, 100, 100);

    (*if_context.pf_destroy_context)(mem);

}
/**********************************************************************************/

int main_on_create(res_win_t wt, void* param)
{
    printf("on_create: main\n");

    if_event_t ev = {0};

    ev.pf_on_create = child_on_create;
	ev.pf_on_destroy = child_on_destroy;
    ev.pf_on_mouse_move = child_on_mouse_move;
	ev.pf_on_mouse_hover = child_on_mouse_hover;
	ev.pf_on_mouse_leave = child_on_mouse_leave;
    ev.pf_on_lbutton_down = child_on_lbutton_down;
    ev.pf_on_lbutton_up = child_on_lbutton_up;
	ev.pf_on_lbutton_dbclick = child_on_lbutton_dbclick;
	ev.pf_on_rbutton_up = child_on_rbutton_up;
	ev.pf_on_keydown = child_on_keydown;
	ev.pf_on_set_focus = child_on_set_focus;
	ev.pf_on_kill_focus = child_on_kill_focus;
	ev.pf_on_paint = child_on_paint;
	ev.pf_on_menu_command = child_on_menu_command;
	ev.pf_on_parent_command = child_on_parent_command;

    xrect_t xr;
    xr.x = 0;
    xr.y = 0;
    xr.w = 300;
    xr.h = 200;
    res_win_t child = (*if_widget.pf_widget_create)(_T("child"),WD_STYLE_CHILD,&xr,wt,&ev);

    (*if_widget.pf_widget_set_user_id)(child, 1000);

    (*if_widget.pf_widget_show)(child, 0);

   // (*if_widget.pf_widget_enable)(child, 0);

    return 0;
}

int main_on_close(res_win_t wt)
{
    printf("main on_closein\n");

    return 0;
}

void main_on_destroy(res_win_t wt)
{
    printf("main on_destroy\n");

    res_win_t child = (*if_widget.pf_widget_get_child)(wt, 1000);

    (*if_widget.pf_widget_destroy)(child);
}

void main_on_lbutton_down(res_win_t wt, const xpoint_t* ppt)
{
    printf("main on_lbutton_down x:%d y:%d\n", ppt->x, ppt->y);
}

void main_on_lbutton_up(res_win_t wt, const xpoint_t* ppt)
{
    printf("main on_lbutton_up x:%d y:%d\n", ppt->x, ppt->y);
}

void main_on_lbutton_dbclick(res_win_t wt, const xpoint_t* ppt)
{
    printf("main on_lbutton_dbclick x:%d y:%d\n", ppt->x, ppt->y);
}

void main_on_rbutton_down(res_win_t wt, const xpoint_t* ppt)
{
    printf("main on_rbutton_down x:%d y:%d\n", ppt->x, ppt->y);
}

void main_on_rbutton_up(res_win_t wt, const xpoint_t* ppt)
{
    printf("main on_rbutton_up x:%d y:%d\n", ppt->x, ppt->y);
}

void main_on_mouse_move(res_win_t wt, dword_t ms, const xpoint_t* ppt)
{
    if(ms & MS_WITH_LBUTTON)
        printf("main on_mouse_move x:%d y:%d with lbutton\n", ppt->x, ppt->y);
    else if(ms & MS_WITH_RBUTTON)
        printf("main on_mouse_move x:%d y:%d with rbutton\n", ppt->x, ppt->y);
    else
        printf("main on_mouse_move x:%d y:%d\n", ppt->x, ppt->y);
}

void main_on_mouse_hover(res_win_t wt, dword_t ms, const xpoint_t* ppt)
{
    if(ms & MS_WITH_LBUTTON)
        printf("main on_mouse_hover x:%d y:%d with lbutton\n", ppt->x, ppt->y);
    else if(ms & MS_WITH_RBUTTON)
        printf("main on_mouse_hover x:%d y:%d with rbutton\n", ppt->x, ppt->y);
    else
        printf("main on_mouse_hover x:%d y:%d\n", ppt->x, ppt->y);
}

void main_on_mouse_leave(res_win_t wt, dword_t ms, const xpoint_t* ppt)
{
    if(ms & MS_WITH_LBUTTON)
        printf("main on_mouse_leave x:%d y:%d with lbutton\n", ppt->x, ppt->y);
    else if(ms & MS_WITH_RBUTTON)
        printf("main on_mouse_leave x:%d y:%d with rbutton\n", ppt->x, ppt->y);
    else
        printf("main on_mouse_leave x:%d y:%d\n", ppt->x, ppt->y);
}

void main_on_whell(res_win_t wt, bool_t horz, int delta)
{
    if(horz)
        printf("main on_whell_horz delta:%d\n", delta);
    else
       printf("main on_whell_vert delta:%d\n", delta);
}

void main_on_scroll(res_win_t wt, bool_t horz, int pos)
{
    if(horz)
        printf("main on_scroll_horz position:%d\n", pos);
    else
       printf("main on_scroll_vert position:%d\n", pos);
}

void main_on_keydown(res_win_t wt, dword_t ks, int key)
{
    if(ks & KS_WITH_CONTROL)
        printf("main on_key_down keycode:%02x with control\n", key);
    else if(ks & KS_WITH_SHIFT)
        printf("main on_key_down keycode:%02x with shift\n", key);
    else
        printf("main on_key_down keycode:%02x\n", key);
}

void main_on_keyup(res_win_t wt, dword_t ks, int key)
{
    if(ks & KS_WITH_CONTROL)
        printf("main on_key_up keycode:%02x with control\n", key);
    else if(ks & KS_WITH_SHIFT)
        printf("main on_key_up keycode:%02x with shift\n", key);
    else
        printf("main on_key_up keycode:%02x\n", key);
}

void main_on_char(res_win_t wt, tchar_t ch)
{
    printf("main on_char keycode:%c\n", ch);
}

void main_on_size(res_win_t wt, int type, const xsize_t* pst)
{
    if(type == WS_SIZE_MAXIMIZED)
        printf("main on_size maximized width:%d height:%d\n", pst->cx, pst->cy);
    else if(type == WS_SIZE_MINIMIZED)
        printf("main on_size minimized width:%d height:%d\n", pst->cx, pst->cy);
    else if(type == WS_SIZE_LAYOUT)
        printf("main on_size layout width:%d height:%d\n", pst->cx, pst->cy);
    else
        printf("main on_size restore width:%d height:%d\n", pst->cx, pst->cy);
}

void main_on_move(res_win_t wt, const xpoint_t* ppt)
{
    printf("main on_move position x:%d y:%d\n", ppt->x, ppt->y);
}

void main_on_show(res_win_t wt, bool_t show)
{
    if(show)
        printf("main on_show \n");
    else
        printf("main on_hide \n");
}

void main_on_activate(res_win_t wt, int ma)
{
    if(ma == WS_ACTIVE_CLICK)
        printf("main on_activate by mouse click \n");
    else
        printf("main on_activate by other \n");
}

void main_on_paint(res_win_t wt, res_ctx_t rdc, const xrect_t* prt)
{
    printf("main main on_paint the rect is x:%d y:%d w:%d h:%d \n", prt->x, prt->y, prt->w, prt->h);
}

void main_on_set_focus(res_win_t wt, res_win_t from)
{
    printf("main on_set_focus \n");
}

void main_on_kill_focus(res_win_t wt, res_win_t to)
{
    printf("main on_kill_focus \n");
}

void main_on_notice(res_win_t, NOTICE* pnt)
{
    printf("main on_notice child id: %d code: %d \n", pnt->id, pnt->code);
}

void main_on_menu_command(res_win_t wt, int code, int cid, var_long data)
{
    printf("main on_menu_command child id: %d code: %d \n", cid, code);
}

void main_on_parent_command(res_win_t, int code, var_long data)
{
    printf("main on_parent_command code: %d \n", code);
}

void main_on_child_command(res_win_t wt, int code, var_long data)
{
    printf("main on_child_command code: %d \n", code);
}

void main_on_self_command(res_win_t wt, int code, var_long data)
{
    printf("main on_self_command code: %d \n", code);
}

void main_on_command_find(res_win_t, str_find_t*)
{
}

void main_on_comamnd_replace(res_win_t, str_replace_t*)
{
}

void main_on_syscmd_click(res_win_t, const xpoint_t*)
{
}

void main_on_timer(res_win_t, var_long)
{
}

/********************************************************************************/

void init_instance()
{
    xdu_impl_context(&if_context);

    xdu_impl_context_graphic(&if_context);

    xdu_impl_context_bitmap(&if_context);

    (*if_context.pf_context_startup)();

    xdu_impl_widget(&if_widget);

    (*if_widget.pf_widget_startup)(0);

    accel_t acs[3] = {
        {KEY_CONTROL, 'a', 10},
        {KEY_ALT, 's', 20},
        {KEY_SHIFT, '\t', 30}
    };

    res_acl_t acl = (*if_widget.pf_create_accel_table)(acs, 3);

    if_event_t ev = {0};

    ev.pf_on_create = main_on_create;
	ev.pf_on_close = main_on_close;
	ev.pf_on_destroy = main_on_destroy;
	ev.pf_on_lbutton_down = main_on_lbutton_down;
	ev.pf_on_lbutton_up = main_on_lbutton_up;
	ev.pf_on_lbutton_dbclick = main_on_lbutton_dbclick;
	ev.pf_on_rbutton_down = main_on_rbutton_down;
	ev.pf_on_rbutton_up = main_on_rbutton_up;
	ev.pf_on_mouse_move = main_on_mouse_move;
	ev.pf_on_mouse_hover = main_on_mouse_hover;
	ev.pf_on_mouse_leave = main_on_mouse_leave;
	ev.pf_on_wheel = main_on_whell;
	ev.pf_on_scroll = main_on_scroll;
	ev.pf_on_keydown = main_on_keydown;
	ev.pf_on_keyup = main_on_keyup;
	ev.pf_on_char = main_on_char;
	ev.pf_on_size = main_on_size;
	ev.pf_on_move = main_on_move;
	ev.pf_on_show = main_on_show;
	ev.pf_on_activate = main_on_activate;
	ev.pf_on_set_focus = main_on_set_focus;
	ev.pf_on_kill_focus = main_on_kill_focus;
	ev.pf_on_paint = main_on_paint;
	ev.pf_on_notice = main_on_notice;
	ev.pf_on_menu_command = main_on_menu_command;
	ev.pf_on_parent_command = main_on_parent_command;
	ev.pf_on_child_command = main_on_child_command;
	ev.pf_on_self_command = main_on_self_command;

    xrect_t xr ;
    xr.x = 0;
    xr.y = 0;
    xr.w = 800;
    xr.h = 600;
    
    g_main = (*if_widget.pf_widget_create)(_T("frame"),WD_STYLE_FRAME,&xr,NULL,&ev);

    (*if_widget.pf_widget_show)(g_main, WS_SHOW_NORMAL);

    (*if_widget.pf_widget_set_accel)(g_main, acl);
}

void uninit_instance()
{
    (*if_widget.pf_widget_cleanup)();

    (*if_context.pf_context_cleanup)();
}

int main(int argc, const char * argv[]) {

    init_instance();

    (*if_widget.pf_widget_do_normal)(g_main);

    uninit_instance();
    
    return 0;
}
