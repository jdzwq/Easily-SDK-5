
#ifndef _XDLOEM_H
#define _XDLOEM_H

#include "oemconv.h"
#include "oembas.h"
#include "oemacp.h"


#ifdef XDL_SUPPORT_BMP
#include "oembmp.h"
#endif

#ifdef XDL_SUPPORT_PDG
#include "oempdg.h"
#endif

#ifdef GPL_SUPPORT_PNG
#include "oempng.h"
#endif

#ifdef GPL_SUPPORT_JPG
#include "oemjpg.h"
#endif

#ifdef XDL_SUPPORT_BAR
#include "oembrc.h"
#include "oemqrc.h"
#endif

#ifdef XDL_SUPPORT_PDG
#include "oempdg.h"
#endif

#ifdef GPL_SUPPORT_ZIP
#include "oemzip.h"
#endif

#ifdef XDL_SUPPORT_ASN
#include "oemasn.h"
#endif

#ifdef XDL_SUPPORT_CRYPT
#include "oemaes.h"
#include "oemarc.h"
#include "oemcrc.h"
#include "oemdes.h"
#include "oemmd2.h"
#include "oemmd4.h"
#include "oemmd5.h"
#include "oemsha1.h"
#include "oemsha2.h"
#include "oemsha4.h"
#include "oemrsa.h"
#endif

#ifdef XDL_SUPPORT_RAND
#include "oemhave.h"
#include "oemlock.h"
#include "oemrand.h"
#endif

#ifdef XDL_SUPPORT_MPI
#include "oemmpi.h"
#endif

#ifdef XDL_SUPPORT_DHM
#include "oemdhm.h"
#endif

#ifdef XDL_SUPPORT_CERT
#include "oemcert.h"
#endif

#ifdef XDL_SUPPORT_SIP
#include "oemsip.h"
#endif

#ifdef XDL_SUPPORT_MUR
#include "oemmur.h"
#endif

#ifdef XDL_SUPPORT_LZF
#include "oemlzf.h"
#endif

#endif //_XDLOEM_H
