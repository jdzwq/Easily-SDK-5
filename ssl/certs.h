/**
 * \file certs.h
 */
#ifndef CERTS_H
#define CERTS_H

#include "sslcfg.h"

#ifdef __cplusplus
extern "C" {
#endif

extern char test_ca_crt[];
extern char test_ca_key[];
extern char test_ca_pwd[];
extern char test_srv_crt[];
extern char test_srv_key[];
extern char test_cli_crt[];
extern char test_cli_key[];
extern char xyssl_ca_crt[];

#ifdef __cplusplus
}
#endif

#endif /* certs.h */
