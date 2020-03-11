
#include <xdl.h>
#include <xds.h>

#ifdef _OS_WINDOWS
#include <conio.h>
#endif

void test_svg()
{
	LINKPTR svg = create_svg_doc();

	canvas_t canv = create_svg_canvas(svg);

	xcolor_t xc;
	parse_xcolor(&xc, GDI_ATTR_RGB_BLACK);

	xrect_t xr;
	xr.fx = 10;
	xr.fy = 10;
	xr.fw = 40;
	xr.fh = 10;
	
	svg_draw_code128(canv, &xc, &xr, _T("1234567890"), -1);

	xr.fx = 10;
	xr.fy = 30;
	xr.fw = 50;
	xr.fh = 50;
	
	svg_draw_pdf417(canv, &xc, &xr, _T("12345678901234567890"), -1);

	xr.fx = 10;
	xr.fy = 70;
	xr.fw = 100;
	xr.fh = 100;
	
	svg_draw_qrcode(canv, &xc, &xr, _T("12345678901234567890"), -1);

	save_dom_doc_to_file(svg, NULL, _T("./demo.svg"));

	destroy_svg_canvas(canv);

	destroy_svg_doc(svg);
}

int main(int argc, char* argv[])
{
	xdl_process_init(XDL_APARTMENT_PROCESS);

	test_svg();

	xdl_process_uninit();

#ifdef _OS_WINDOWS
	getch();
#endif
	return 0;
}

