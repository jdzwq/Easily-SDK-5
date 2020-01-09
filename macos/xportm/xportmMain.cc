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

	sign = event_create();

	_xportm_start(&xm);

	if (sign)
	{
		event_wait(sign, -1);
	}

	_xportm_stop(&xm);

	event_destroy(sign);
	sign = NULL;

	END_CATCH;

	xdl_process_uninit();

	return 0;

ONERROR:

	if (sign)
		event_destroy(sign);

	xdl_process_uninit();

	return -1;
}

