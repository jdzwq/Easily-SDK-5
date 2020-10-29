/***********************************************************************
	Easily tkb 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc tdfs document

	@module	tkv.c | t kv document implement file

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

#include "tkv.h"
#include "tkb.h"

typedef struct _t_kv_ctx
{
	t_kv_hdr hdr;

	link_t_ptr dict;
	t_kb_t hdb;
}t_kv_ctx;

t_kv_t tkv_create(t_kb_t hdb)
{
	t_kv_ctx* pobj;

	pobj = (t_kv_ctx*)xmem_alloc(sizeof(t_kv_ctx));

	pobj->dict = create_dict_table();
	pobj->hdb = hdb;

	pobj->hdr.tag = T_OBJ_KV;
	return &(pobj->hdr);
}

void tkv_destroy(t_kv_t hkv)
{
	t_kv_ctx* pobj = (t_kv_ctx*)hkv;

	XDL_ASSERT(hkv && hkv->tag == T_OBJ_KV);

	destroy_dict_table(pobj->dict);

	xmem_free(pobj);
}

void tkv_write(t_kv_t hkv, variant_t key, object_t val)
{
	t_kv_ctx* pobj = (t_kv_ctx*)hkv;
	link_t_ptr ent;
	var_long mask;

	XDL_ASSERT(hkv && hkv->tag == T_OBJ_KV);

	ent = write_dict_item(pobj->dict, key, val);

	mask = get_dict_entity_delta(ent);
	mask &= ~TKV_MASK_DELETED;

	set_dict_entity_delta(ent, mask | TKV_MASK_UPDATED);

	if (pobj->hdb && tkb_save(pobj->hdb, key, val))
	{
		mask &= ~TKV_MASK_UPDATED;
		mask |= TKV_MASK_PERSIST;
		set_dict_entity_delta(ent, mask);
	}
}

bool_t tkv_update(t_kv_t hkv, variant_t key, object_t val)
{
	t_kv_ctx* pobj = (t_kv_ctx*)hkv;
	link_t_ptr ent;
	var_long mask;

	XDL_ASSERT(hkv && hkv->tag == T_OBJ_KV);

	ent = get_dict_entity(pobj->dict, key);
	if (!ent)
		return 0;

	set_dict_entity_val(ent, val);

	mask = get_dict_entity_delta(ent);
	mask &= ~TKV_MASK_DELETED;

	set_dict_entity_delta(ent, mask | TKV_MASK_UPDATED);

	if (pobj->hdb && tkb_save(pobj->hdb, key, val))
	{
		mask &= ~TKV_MASK_UPDATED;
		mask |= TKV_MASK_PERSIST;
		set_dict_entity_delta(ent, mask);
	}

	return 1;
}

bool_t tkv_read(t_kv_t hkv, variant_t key, object_t val)
{
	t_kv_ctx* pobj = (t_kv_ctx*)hkv;
	link_t_ptr ent;
	var_long mask;

	XDL_ASSERT(hkv && hkv->tag == T_OBJ_KV);

	ent = get_dict_entity(pobj->dict, key);
	if (ent)
	{
		mask = get_dict_entity_delta(ent);
		if (mask & TKV_MASK_DELETED)
			return 0;

		get_dict_entity_val(ent, val);
		return 1;
	}

	if (!pobj->hdb)
		return 0;

	if (pobj->hdb && tkb_load(pobj->hdb, key, val))
	{
		ent = write_dict_item(pobj->dict, key, val);
		set_dict_entity_delta(ent, TKV_MASK_PERSIST);
		return 1;
	}

	return 0;
}

void tkv_delete(t_kv_t hkv, variant_t key)
{
	t_kv_ctx* pobj = (t_kv_ctx*)hkv;
	link_t_ptr ent;
	var_long mask;
	
	XDL_ASSERT(hkv && hkv->tag == T_OBJ_KV);

	ent = get_dict_entity(pobj->dict, key);
	if (!ent)
	{
		if (pobj->hdb) tkb_clean(pobj->hdb, key);
		return;
	}

	mask = get_dict_entity_delta(ent);
	if (!(mask & TKV_MASK_PERSIST))
	{
		delete_dict_entity(ent);
		return;
	}

	set_dict_entity_val(ent, NULL);
	set_dict_entity_delta(ent, mask | TKV_MASK_DELETED);
}

void tkv_attach(t_kv_t hkv, variant_t key, object_t val)
{
	t_kv_ctx* pobj = (t_kv_ctx*)hkv;
	link_t_ptr ent;
	var_long mask;

	XDL_ASSERT(hkv && hkv->tag == T_OBJ_KV);

	ent = write_dict_item(pobj->dict, key, NULL);
	attach_dict_entity_val(ent, val);

	mask = get_dict_entity_delta(ent);
	mask &= ~TKV_MASK_DELETED;

	set_dict_entity_delta(ent, mask | TKV_MASK_UPDATED);

	if (pobj->hdb && tkb_save(pobj->hdb, key, val))
	{
		mask &= ~TKV_MASK_UPDATED;
		mask |= TKV_MASK_PERSIST;
		set_dict_entity_delta(ent, mask);
	}
}

object_t tkv_detach(t_kv_t hkv, variant_t key)
{
	t_kv_ctx* pobj = (t_kv_ctx*)hkv;
	link_t_ptr ent;
	var_long mask;
	object_t val;

	XDL_ASSERT(hkv && hkv->tag == T_OBJ_KV);

	ent = get_dict_entity(pobj->dict, key);
	if (!ent)
		return NULL;

	val = detach_dict_entity_val(ent);

	mask = get_dict_entity_delta(ent);
	if (!(mask & TKV_MASK_PERSIST))
	{
		delete_dict_entity(ent);
		return val;
	}

	set_dict_entity_val(ent, NULL);
	set_dict_entity_delta(ent, mask | TKV_MASK_DELETED);

	return val;
}

void tkv_flush(t_kv_t hkv)
{
	t_kv_ctx* pobj = (t_kv_ctx*)hkv;
	dict_enum_t de = { 0 };
	link_t_ptr ent,nxt;
	var_long mask;
	const variant_t *pkey;
	const object_t* pval;

	XDL_ASSERT(hkv && hkv->tag == T_OBJ_KV);

	de.dict = pobj->dict;
	de.entity = LINK_FIRST;

	ent = get_dict_next_entity(&de);
	while (ent)
	{
		nxt = get_dict_next_entity(&de);

		mask = get_dict_entity_delta(ent);
		if (mask & TKV_MASK_DELETED)
		{
			pkey = get_dict_entity_key_ptr(ent);

			if (pobj->hdb && tkb_clean(pobj->hdb, *pkey))
			{
				delete_dict_entity(ent);
			}
			else if (!pobj->hdb)
			{
				delete_dict_entity(ent);
			}
		}
		else if (mask & TKV_MASK_UPDATED)
		{
			pkey = get_dict_entity_key_ptr(ent);
			pval = get_dict_entity_val_ptr(ent);

			if (pobj->hdb && tkb_save(pobj->hdb, *pkey, *pval))
			{
				mask &= ~TKV_MASK_UPDATED;
				mask |= TKV_MASK_PERSIST;
				set_dict_entity_delta(ent, mask);
			}
		}

		ent = nxt;
	}
}