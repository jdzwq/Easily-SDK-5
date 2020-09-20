﻿
#ifndef _XDL_H
#define _XDL_H

#include "../xdl/xdldef.h"
#include "../xdl/xdlimp.h"
#include "../xdl/xdlstd.h"

#include "../xdl/xdlnet.h"
#include "../xdl/xdlsrv.h"

#ifdef XDL_SUPPORT_DOC
#include "../xdl/xdldoc.h"
#endif

#ifdef XDL_SUPPORT_VIEW
#include "../xdl/xdlview.h"
#endif

#ifdef XDL_SUPPORT_SVG
#include "../xdl/xdlsvg.h"
#endif

#include "../xdl/xdlinit.h"

typedef	link_t_ptr	LINKPTR;
typedef xhand_t		XHANDLE;

#ifdef _OS_WINDOWS
#pragma comment(lib,"xdl.lib")
#endif

#endif //_XDL_H
