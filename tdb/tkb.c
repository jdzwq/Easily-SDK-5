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

	link_t_ptr tree;

	tchar_t dpath[PATH_LEN + 1];
	tchar_t dname[KEY_LEN + 1];

}t_kb_ctx;

t_kb_t tkb_create(const tchar_t* dpath, const tchar_t* dname)
{
	t_kb_ctx* pobj;
	link_t_ptr table;
	tchar_t fpath[PATH_LEN + 1] = { 0 };

	pobj = (t_kb_ctx*)xmem_alloc(sizeof(t_kb_ctx));

	xsncpy(pobj->dpath, dpath, PATH_LEN);
	xsncpy(pobj->dname, dname, KEY_LEN);

	pobj->tree = create_bplus_tree();

	xsprintf(fpath, _T("%s/%s.ind"), pobj->dpath, pobj->dname);
	table = create_file_table(fpath);
	if (!table)
	{
		destroy_bplus_tree(pobj->tree);
		xmem_free(pobj);
		return NULL;
	}
	attach_bplus_index_table(pobj->tree, table);

	xsprintf(fpath, _T("%s/%s.dat"), pobj->dpath, pobj->dname);
	table = create_file_table(fpath);
	if (!table)
	{
		table = attach_bplus_index_table(pobj->tree, NULL);
		destroy_file_table(table);

		destroy_bplus_tree(pobj->tree);
		xmem_free(pobj);
		return NULL;
	}
	attach_bplus_data_table(pobj->tree, table);

	pobj->hdr.tag = T_OBJ_DB;
	return &(pobj->hdr);
}

void tkb_destroy(t_kb_t hdb)
{
	t_kb_ctx* pobj = (t_kb_ctx*)hdb;
	link_t_ptr table;

	XDL_ASSERT(hdb && hdb->tag == T_OBJ_DB);

	table = attach_bplus_data_table(pobj->tree, NULL);
	destroy_file_table(table);

	table = attach_bplus_index_table(pobj->tree, NULL);
	destroy_file_table(table);

	destroy_bplus_tree(pobj->tree);

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


