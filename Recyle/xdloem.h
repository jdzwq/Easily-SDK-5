
#ifndef _XDLOEM_H
#define _XDLOEM_H

#ifdef XDL_SUPPORT_PNG
#include "oempng.h"
#endif

#ifdef XDL_SUPPORT_JPG
#include "oemjpg.h"
#endif

#ifdef XDL_SUPPORT_ENC
#include "oemmd5.h"
#endif

#ifdef XDL_SUPPORT_ENC
#include "oemsha.h"
#endif

#ifdef XDL_SUPPORT_ZIP
#include "oemzlib.h"
#endif

#ifdef XDL_SUPPORT_BRCODE
#include "oembrcode.h"
#endif

#ifdef XDL_SUPPORT_BARCODE
#include "oemqrcode.h"
#endif

#ifdef XDL_SUPPORT_SSL
#include "oemssl.h"
#endif


#endif //XDLOEM_H
