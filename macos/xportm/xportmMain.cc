#include <iostream>
#include <xdl.h>
#include <xds.h>

#include "xportm.h"

xportm_param_t xm = { 0 };
res_even_t sign = NULL;


int main(int argc, const char * argv[])
{

	xdl_process_init(XDL_APARTMENT_PROCESS);

	TRY_CATCH;

	xscpy(xm.pname, _T("xportm"));

	sign = xevent_create();

	_xportm_start(&xm);

	if (sign)
	{
		xevent_wait(sign, -1);
	}

	_xportm_stop(&xm);

	xevent_destroy(sign);
	sign = NULL;

	END_CATCH;

	xdl_process_uninit();

	return 0;

ONERROR:

	if (sign)
		xevent_destroy(sign);

	xdl_process_uninit();

	return -1;
}

