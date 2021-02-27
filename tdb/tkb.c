/***********************************************************************
	Easily tkb 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tdfs document

	@module	tkb.c | t db document implement file

	@devnote 张文权 2018.01 - 2018.12	v1.0
***********************************************************************/

/**********************************************************************
This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
LICENSE.GPL3 for more details.
***********************************************************************/

#include "tkb.h"

typedef struct _t_kb_ctx
{
	t_kb_hdr hdr;

	link_t_ptr ind_table;
	link_t_ptr dat_table;

	link_t_ptr tree;

	tchar_t dpath[PATH_LEN + 1];
	tchar_t dname[KEY_LEN + 1];

}t_kb_ctx;

t_kb_t tkb_create(const tchar_t* dpath, const tchar_t* dname, dword_t dmode)
{
	t_kb_ctx* pobj = NULL;
	tchar_t fpath[PATH_LEN + 1] = { 0 };

	TRY_CATCH;

	pobj = (t_kb_ctx*)xmem_alloc(sizeof(t_kb_ctx));

	xsncpy(pobj->dpath, dpath, PATH_LEN);
	xsncpy(pobj->dname, dname, KEY_LEN);

	xsprintf(fpath, _T("%s/%s.ind"), pobj->dpath, pobj->dname);
	pobj->ind_table = create_file_table(fpath, BLOCK_SIZE_4096, dmode);
	if (!(pobj->ind_table))
	{
		raise_user_error(_T("tkb_create"), fpath);
	}

	xsprintf(fpath, _T("%s/%s.dat"), pobj->dpath, pobj->dname);
	pobj->dat_table = create_file_table(fpath, BLOCK_SIZE_512, dmode);
	if (!(pobj->dat_table))
	{
		raise_user_error(_T("tkb_create"), fpath);
	}
	
	pobj->tree = create_bplus_file_table(pobj->ind_table, pobj->dat_table);
	if (!(pobj->tree))
	{
		raise_user_error(_T("tkb_create"), _T("create bplus tree failed"));
	}

	END_CATCH;

	pobj->hdr.tag = T_OBJ_DB;
	return &(pobj->hdr);

ONERROR:
	XDL_TRACE_LAST;

	if (pobj)
	{
		if (pobj->ind_table)
			destroy_file_table(pobj->ind_table);
		if (pobj->dat_table)
			destroy_file_table(pobj->dat_table);
		if (pobj->tree)
			destroy_bplus_tree(pobj->tree);

		xmem_free(pobj);
	}

	return NULL;
}

void tkb_destroy(t_kb_t hdb)
{
	t_kb_ctx* pobj = (t_kb_ctx*)hdb;

	XDL_ASSERT(hdb && hdb->tag == T_OBJ_DB);

	destroy_bplus_tree(pobj->tree);

	destroy_file_table(pobj->ind_table);

	destroy_file_table(pobj->dat_table);

	xmem_free(pobj);
}

bool_t tkb_save(t_kb_t hdb, variant_t key, object_t val)
{
	t_kb_ctx* pobj = (t_kb_ctx*)hdb;

	XDL_ASSERT(hdb && hdb->tag == T_OBJ_DB);

	return insert_bplus_entity(pobj->tree, key, val);
}

bool_t tkb_load(t_kb_t hdb, variant_t key, object_t val)
{
	t_kb_ctx* pobj = (t_kb_ctx*)hdb;

	XDL_ASSERT(hdb && hdb->tag == T_OBJ_DB);

	return find_bplus_entity(pobj->tree, key, val);
}

bool_t tkb_clean(t_kb_t hdb, variant_t key)
{
	t_kb_ctx* pobj = (t_kb_ctx*)hdb;

	XDL_ASSERT(hdb && hdb->tag == T_OBJ_DB);

	return delete_bplus_entity(pobj->tree, key);
}

void tkb_enum(t_kb_t hdb, CALLBACK_ENUMBPLUSENTITY pf, void* param)
{
	t_kb_ctx* pobj = (t_kb_ctx*)hdb;

	XDL_ASSERT(hdb && hdb->tag == T_OBJ_DB);

	enum_bplus_entity(pobj->tree, pf, param);
}
