/***********************************************************************
	Easily HexDFS 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc hexdfs document

	@module	hexdb.c | hex db document implement file

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

#include "hexdb.h"

typedef struct _hex_db_t
{
	hex_obj_hdr hdr;

	link_t_ptr tree;

	tchar_t dpath[PATH_LEN];
	tchar_t dname[KEY_LEN];

}hex_db_t;

hex_obj_t hexdb_create(const tchar_t* dpath, const tchar_t* dname)
{
	hex_db_t* pobj;
	link_t_ptr table;
	tchar_t fpath[PATH_LEN] = { 0 };

	pobj = (hex_db_t*)xmem_alloc(sizeof(hex_db_t));

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

	pobj->hdr.tag = HEX_OBJ_DB;
	return &(pobj->hdr);
}

void hexdb_destroy(hex_obj_t hdb)
{
	hex_db_t* pobj = (hex_db_t*)hdb;
	link_t_ptr table;

	XDL_ASSERT(hdb && hdb->tag == HEX_OBJ_DB);

	table = attach_bplus_data_table(pobj->tree, NULL);
	destroy_file_table(table);

	table = attach_bplus_index_table(pobj->tree, NULL);
	destroy_file_table(table);

	destroy_bplus_tree(pobj->tree);

	xmem_free(pobj);
}

bool_t hexdb_save(hex_obj_t hdb, variant_t key, object_t val)
{
	hex_db_t* pobj = (hex_db_t*)hdb;

	XDL_ASSERT(hdb && hdb->tag == HEX_OBJ_DB);

	return insert_bplus_entity(pobj->tree, key, val);
}

bool_t hexdb_load(hex_obj_t hdb, variant_t key, object_t val)
{
	hex_db_t* pobj = (hex_db_t*)hdb;

	XDL_ASSERT(hdb && hdb->tag == HEX_OBJ_DB);

	return find_bplus_entity(pobj->tree, key, val);
}

bool_t hexdb_clean(hex_obj_t hdb, variant_t key)
{
	hex_db_t* pobj = (hex_db_t*)hdb;

	XDL_ASSERT(hdb && hdb->tag == HEX_OBJ_DB);

	return delete_bplus_entity(pobj->tree, key);
}


