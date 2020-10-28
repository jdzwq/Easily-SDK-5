/***********************************************************************
	Easily HexDFS 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tkdfs document

	@module	tkkv.c | tk kv document implement file

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

#include "tkkv.h"
#include "tkdb.h"

typedef struct _tk_kv_ctx
{
	tk_kv_hdr hdr;

	link_t_ptr dict;
	tk_db_t hdb;
}tk_kv_ctx;

tk_kv_t tkkv_create(tk_db_t hdb)
{
	tk_kv_ctx* pobj;

	pobj = (tk_kv_ctx*)xmem_alloc(sizeof(tk_kv_ctx));

	pobj->dict = create_dict_table();
	pobj->hdb = hdb;

	pobj->hdr.tag = TK_OBJ_KV;
	return &(pobj->hdr);
}

void tkkv_destroy(tk_kv_t hkv)
{
	tk_kv_ctx* pobj = (tk_kv_ctx*)hkv;

	XDL_ASSERT(hkv && hkv->tag == TK_OBJ_KV);

	destroy_dict_table(pobj->dict);

	xmem_free(pobj);
}

void tkkv_write(tk_kv_t hkv, variant_t key, object_t val)
{
	tk_kv_ctx* pobj = (tk_kv_ctx*)hkv;
	link_t_ptr ent;
	var_long mask;

	XDL_ASSERT(hkv && hkv->tag == TK_OBJ_KV);

	ent = write_dict_item(pobj->dict, key, val);

	mask = get_dict_entity_delta(ent);
	mask &= ~TKKV_MASK_DELETED;

	set_dict_entity_delta(ent, mask | TKKV_MASK_UPDATED);

	if (pobj->hdb && tkdb_save(pobj->hdb, key, val))
	{
		mask &= ~TKKV_MASK_UPDATED;
		mask |= TKKV_MASK_PERSIST;
		set_dict_entity_delta(ent, mask);
	}
}

bool_t tkkv_update(tk_kv_t hkv, variant_t key, object_t val)
{
	tk_kv_ctx* pobj = (tk_kv_ctx*)hkv;
	link_t_ptr ent;
	var_long mask;

	XDL_ASSERT(hkv && hkv->tag == TK_OBJ_KV);

	ent = get_dict_entity(pobj->dict, key);
	if (!ent)
		return 0;

	set_dict_entity_val(ent, val);

	mask = get_dict_entity_delta(ent);
	mask &= ~TKKV_MASK_DELETED;

	set_dict_entity_delta(ent, mask | TKKV_MASK_UPDATED);

	if (pobj->hdb && tkdb_save(pobj->hdb, key, val))
	{
		mask &= ~TKKV_MASK_UPDATED;
		mask |= TKKV_MASK_PERSIST;
		set_dict_entity_delta(ent, mask);
	}

	return 1;
}

bool_t tkkv_read(tk_kv_t hkv, variant_t key, object_t val)
{
	tk_kv_ctx* pobj = (tk_kv_ctx*)hkv;
	link_t_ptr ent;
	var_long mask;

	XDL_ASSERT(hkv && hkv->tag == TK_OBJ_KV);

	ent = get_dict_entity(pobj->dict, key);
	if (ent)
	{
		mask = get_dict_entity_delta(ent);
		if (mask & TKKV_MASK_DELETED)
			return 0;

		get_dict_entity_val(ent, val);
		return 1;
	}

	if (!pobj->hdb)
		return 0;

	if (pobj->hdb && tkdb_load(pobj->hdb, key, val))
	{
		ent = write_dict_item(pobj->dict, key, val);
		set_dict_entity_delta(ent, TKKV_MASK_PERSIST);
		return 1;
	}

	return 0;
}

void tkkv_delete(tk_kv_t hkv, variant_t key)
{
	tk_kv_ctx* pobj = (tk_kv_ctx*)hkv;
	link_t_ptr ent;
	var_long mask;
	
	XDL_ASSERT(hkv && hkv->tag == TK_OBJ_KV);

	ent = get_dict_entity(pobj->dict, key);
	if (!ent)
	{
		if (pobj->hdb) tkdb_clean(pobj->hdb, key);
		return;
	}

	mask = get_dict_entity_delta(ent);
	if (!(mask & TKKV_MASK_PERSIST))
	{
		delete_dict_entity(ent);
		return;
	}

	set_dict_entity_val(ent, NULL);
	set_dict_entity_delta(ent, mask | TKKV_MASK_DELETED);
}

void tkkv_attach(tk_kv_t hkv, variant_t key, object_t val)
{
	tk_kv_ctx* pobj = (tk_kv_ctx*)hkv;
	link_t_ptr ent;
	var_long mask;

	XDL_ASSERT(hkv && hkv->tag == TK_OBJ_KV);

	ent = write_dict_item(pobj->dict, key, NULL);
	attach_dict_entity_val(ent, val);

	mask = get_dict_entity_delta(ent);
	mask &= ~TKKV_MASK_DELETED;

	set_dict_entity_delta(ent, mask | TKKV_MASK_UPDATED);

	if (pobj->hdb && tkdb_save(pobj->hdb, key, val))
	{
		mask &= ~TKKV_MASK_UPDATED;
		mask |= TKKV_MASK_PERSIST;
		set_dict_entity_delta(ent, mask);
	}
}

object_t tkkv_detach(tk_kv_t hkv, variant_t key)
{
	tk_kv_ctx* pobj = (tk_kv_ctx*)hkv;
	link_t_ptr ent;
	var_long mask;
	object_t val;

	XDL_ASSERT(hkv && hkv->tag == TK_OBJ_KV);

	ent = get_dict_entity(pobj->dict, key);
	if (!ent)
		return NULL;

	val = detach_dict_entity_val(ent);

	mask = get_dict_entity_delta(ent);
	if (!(mask & TKKV_MASK_PERSIST))
	{
		delete_dict_entity(ent);
		return val;
	}

	set_dict_entity_val(ent, NULL);
	set_dict_entity_delta(ent, mask | TKKV_MASK_DELETED);

	return val;
}

void tkkv_flush(tk_kv_t hkv)
{
	tk_kv_ctx* pobj = (tk_kv_ctx*)hkv;
	dict_enum_t de = { 0 };
	link_t_ptr ent,nxt;
	var_long mask;
	const variant_t *pkey;
	const object_t* pval;

	XDL_ASSERT(hkv && hkv->tag == TK_OBJ_KV);

	de.dict = pobj->dict;
	de.entity = LINK_FIRST;

	ent = get_dict_next_entity(&de);
	while (ent)
	{
		nxt = get_dict_next_entity(&de);

		mask = get_dict_entity_delta(ent);
		if (mask & TKKV_MASK_DELETED)
		{
			pkey = get_dict_entity_key_ptr(ent);

			if (pobj->hdb && tkdb_clean(pobj->hdb, *pkey))
			{
				delete_dict_entity(ent);
			}
			else if (!pobj->hdb)
			{
				delete_dict_entity(ent);
			}
		}
		else if (mask & TKKV_MASK_UPDATED)
		{
			pkey = get_dict_entity_key_ptr(ent);
			pval = get_dict_entity_val_ptr(ent);

			if (pobj->hdb && tkdb_save(pobj->hdb, *pkey, *pval))
			{
				mask &= ~TKKV_MASK_UPDATED;
				mask |= TKKV_MASK_PERSIST;
				set_dict_entity_delta(ent, mask);
			}
		}

		ent = nxt;
	}
}