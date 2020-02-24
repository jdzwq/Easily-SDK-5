//
//  main.cpp
//  xdu_lib_test
//
//  Created by 张文权 on 18/10/4.
//  Copyright © 2018年 张文权. All rights reserved.
//

#include <iostream>
#include <xdu.h>

 
void test_context()
{
    if_context_t if_context = { 0 };
    
    xdu_impl_context(&if_context);
    
    (*if_context.pf_context_startup)();
    
    dev_cap_t dev = {0};
    
    (*if_context.pf_get_device_caps)(NULL, &dev);
    
    res_ctx_t rdc = (*if_context.pf_create_display_context)();
    
    res_ctx_t xdc = (*if_context.pf_create_compatible_context)(rdc);
    
    res_pmp_t pmp = (*if_context.pf_create_compatible_pixmap)(xdc, 32, 32);
    
    res_pmp_t org = (*if_context.pf_select_pixmap)(xdc, pmp);
    
    (*if_context.pf_destroy_pixmap)(org);
    
     (*if_context.pf_destroy_context)(xdc);
    
    xfont_t xf;
    default_xfont(&xf);
    
    float fw, fh;
    
    (*if_context.pf_text_mm_size)(rdc, &xf, _T("hello world!"), -1, &fw, &fh);
    
    float f = (*if_context.pf_pt_per_mm)(rdc,1);
    
    int s = (*if_context.pf_font_size)(rdc,7 * 3.77);
    
    (*if_context.pf_destroy_context)(rdc);
    
    (*if_context.pf_context_cleanup)();
}

void test_bitmap()
{
    if_context_t if_context = { 0 };
    
    xdu_impl_context(&if_context);
    
    xdu_impl_context_bitmap(&if_context);

    (*if_context.pf_context_startup)();
    
    res_ctx_t rdc = (*if_context.pf_create_display_context)();
    
    xcolor_t clr;
    parse_xcolor(&clr, GDI_ATTR_RGB_AZURE);

    res_bmp_t bmp = (*if_context.pf_create_color_bitmap)(rdc, &clr, 32, 32);
    
    int n = (*if_context.pf_save_bitmap_to_bytes)(rdc, bmp, NULL, MAX_LONG);
    
    unsigned char* buf = (unsigned char*)calloc(1, n);
    
    (*if_context.pf_save_bitmap_to_bytes)(rdc, bmp, buf, n);
    
    free(buf);
    
    (*if_context.pf_destroy_bitmap)(bmp);

    (*if_context.pf_destroy_context)(rdc);

    (*if_context.pf_context_cleanup)();
}

void test_widget()
{
    if_context_t if_context = { 0 };
    
    xdu_impl_context(&if_context);
    
    (*if_context.pf_context_startup)();

    
    if_widget_t if_widget = {0};
    
    xdu_impl_widget(&if_widget);
    
    xrect_t xr ;
    xr.x = 100;
    xr.y = 100;
    xr.w = 800;
    xr.h = 600;
    
    res_win_t win = (*if_widget.pf_widget_create)(_T("demo"),WD_STYLE_DIALOG,&xr,NULL,NULL);

    (*if_widget.pf_widget_show)(win, WD_SHOW_MINIMIZE);

    xr.x = 200;
    xr.y = 200;
    xr.w = 300;
    xr.h = 200;
    res_win_t child = (*if_widget.pf_widget_create)(_T("child"),WD_STYLE_CHILD,&xr,win,NULL);

    (*if_widget.pf_widget_set_user_id)(child, 1000);

    child = (*if_widget.pf_widget_get_child)(win, 1000);

    (*if_widget.pf_widget_show)(child, 0);

    win = (*if_widget.pf_widget_get_parent)(child);

    bool_t b = (*if_widget.pf_widget_is_minimized)(win);

    xr.x = 0;
    xr.y = 0;
    xr.w = 200;
    xr.h = 300;
    res_win_t pop = (*if_widget.pf_widget_create)(_T("popup"),WD_STYLE_POPUP,&xr,NULL,NULL);

    (*if_widget.pf_widget_show)(pop, 0);

    (*if_widget.pf_widget_do_modal)(win);
    
    (*if_widget.pf_widget_destroy)(child);

    (*if_widget.pf_widget_destroy)(win);

    (*if_widget.pf_widget_destroy)(pop);
    
    (*if_context.pf_context_cleanup)();
}

int main(int argc, const char * argv[]) {
    //struct sigaction sa = {0};
    //sa.sa_handler = SIG_IGN;
    //sigaction(SIGPIPE, &sa, 0 );
    
    // insert code here...
    
    //test_context();
    
    //test_bitmap();
    
    test_widget();
    
    return 0;
}
