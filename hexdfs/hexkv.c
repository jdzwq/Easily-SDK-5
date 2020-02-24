/***********************************************************************
	Easily HexDFS 1.0

	(c) 2005-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc hexdfs document

	@module	hexkv.c | hex kv document implement file

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

#include "hexkv.h"
#include "hexdb.h"

typedef struct _hex_kv_t
{
	hex_obj_hdr hdr;

	link_t_ptr dict;
	hex_obj_t hdb;
}hex_kv_t;

hex_obj_t hexkv_create(hex_obj_t hdb)
{
	hex_kv_t* pobj;

	pobj = (hex_kv_t*)xmem_alloc(sizeof(hex_kv_t));

	pobj->dict = create_dict_table();
	pobj->hdb = hdb;

	pobj->hdr.tag = HEX_OBJ_KV;
	return &(pobj->hdr);
}

void hexkv_destroy(hex_obj_t hkv)
{
	hex_kv_t* pobj = (hex_kv_t*)hkv;

	XDL_ASSERT(hkv && hkv->tag == HEX_OBJ_KV);

	destroy_dict_table(pobj->dict);

	xmem_free(pobj);
}

void hexkv_write(hex_obj_t hkv, variant_t key, object_t val)
{
	hex_kv_t* pobj = (hex_kv_t*)hkv;
	link_t_ptr ent;
	var_long mask;

	XDL_ASSERT(hkv && hkv->tag == HEX_OBJ_KV);

	ent = write_dict_item(pobj->dict, key, val);

	mask = get_dict_entity_delta(ent);
	mask &= ~HEXKV_MASK_DELETED;

	set_dict_entity_delta(ent, mask | HEXKV_MASK_UPDATED);

	if (pobj->hdb && hexdb_save(pobj->hdb, key, val))
	{
		mask &= ~HEXKV_MASK_UPDATED;
		mask |= HEXKV_MASK_PERSIST;
		set_dict_entity_delta(ent, mask);
	}
}

bool_t hexkv_update(hex_obj_t hkv, variant_t key, object_t val)
{
	hex_kv_t* pobj = (hex_kv_t*)hkv;
	link_t_ptr ent;
	var_long mask;

	XDL_ASSERT(hkv && hkv->tag == HEX_OBJ_KV);

	ent = get_dict_entity(pobj->dict, key);
	if (!ent)
		return 0;

	set_dict_entity_val(ent, val);

	mask = get_dict_entity_delta(ent);
	mask &= ~HEXKV_MASK_DELETED;

	set_dict_entity_delta(ent, mask | HEXKV_MASK_UPDATED);

	if (pobj->hdb && hexdb_save(pobj->hdb, key, val))
	{
		mask &= ~HEXKV_MASK_UPDATED;
		mask |= HEXKV_MASK_PERSIST;
		set_dict_entity_delta(ent, mask);
	}

	return 1;
}

bool_t hexkv_read(hex_obj_t hkv, variant_t key, object_t val)
{
	hex_kv_t* pobj = (hex_kv_t*)hkv;
	link_t_ptr ent;
	var_long mask;

	XDL_ASSERT(hkv && hkv->tag == HEX_OBJ_KV);

	ent = get_dict_entity(pobj->dict, key);
	if (ent)
	{
		mask = get_dict_entity_delta(ent);
		if (mask & HEXKV_MASK_DELETED)
			return 0;

		get_dict_entity_val(ent, val);
		return 1;
	}

	if (!pobj->hdb)
		return 0;

	if (pobj->hdb && hexdb_load(pobj->hdb, key, val))
	{
		ent = write_dict_item(pobj->dict, key, val);
		set_dict_entity_delta(ent, HEXKV_MASK_PERSIST);
		return 1;
	}

	return 0;
}

void hexkv_delete(hex_obj_t hkv, variant_t key)
{
	hex_kv_t* pobj = (hex_kv_t*)hkv;
	link_t_ptr ent;
	var_long mask;
	
	XDL_ASSERT(hkv && hkv->tag == HEX_OBJ_KV);

	ent = get_dict_entity(pobj->dict, key);
	if (!ent)
	{
		if (pobj->hdb) hexdb_clean(pobj->hdb, key);
		return;
	}

	mask = get_dict_entity_delta(ent);
	if (!(mask & HEXKV_MASK_PERSIST))
	{
		delete_dict_entity(ent);
		return;
	}

	set_dict_entity_val(ent, NULL);
	set_dict_entity_delta(ent, mask | HEXKV_MASK_DELETED);
}

void hexkv_attach(hex_obj_t hkv, variant_t key, object_t val)
{
	hex_kv_t* pobj = (hex_kv_t*)hkv;
	link_t_ptr ent;
	var_long mask;

	XDL_ASSERT(hkv && hkv->tag == HEX_OBJ_KV);

	ent = write_dict_item(pobj->dict, key, NULL);
	attach_dict_entity_val(ent, val);

	mask = get_dict_entity_delta(ent);
	mask &= ~HEXKV_MASK_DELETED;

	set_dict_entity_delta(ent, mask | HEXKV_MASK_UPDATED);

	if (pobj->hdb && hexdb_save(pobj->hdb, key, val))
	{
		mask &= ~HEXKV_MASK_UPDATED;
		mask |= HEXKV_MASK_PERSIST;
		set_dict_entity_delta(ent, mask);
	}
}

object_t hexkv_detach(hex_obj_t hkv, variant_t key)
{
	hex_kv_t* pobj = (hex_kv_t*)hkv;
	link_t_ptr ent;
	var_long mask;
	object_t val;

	XDL_ASSERT(hkv && hkv->tag == HEX_OBJ_KV);

	ent = get_dict_entity(pobj->dict, key);
	if (!ent)
		return NULL;

	val = detach_dict_entity_val(ent);

	mask = get_dict_entity_delta(ent);
	if (!(mask & HEXKV_MASK_PERSIST))
	{
		delete_dict_entity(ent);
		return val;
	}

	set_dict_entity_val(ent, NULL);
	set_dict_entity_delta(ent, mask | HEXKV_MASK_DELETED);

	return val;
}

void hexkv_flush(hex_obj_t hkv)
{
	hex_kv_t* pobj = (hex_kv_t*)hkv;
	dict_enum_t de = { 0 };
	link_t_ptr ent,nxt;
	var_long mask;
	const variant_t *pkey;
	const object_t* pval;

	XDL_ASSERT(hkv && hkv->tag == HEX_OBJ_KV);

	de.dict = pobj->dict;
	de.entity = LINK_FIRST;

	ent = get_dict_next_entity(&de);
	while (ent)
	{
		nxt = get_dict_next_entity(&de);

		mask = get_dict_entity_delta(ent);
		if (mask & HEXKV_MASK_DELETED)
		{
			pkey = get_dict_entity_key_ptr(ent);

			if (pobj->hdb && hexdb_clean(pobj->hdb, *pkey))
			{
				delete_dict_entity(ent);
			}
			else if (!pobj->hdb)
			{
				delete_dict_entity(ent);
			}
		}
		else if (mask & HEXKV_MASK_UPDATED)
		{
			pkey = get_dict_entity_key_ptr(ent);
			pval = get_dict_entity_val_ptr(ent);

			if (pobj->hdb && hexdb_save(pobj->hdb, *pkey, *pval))
			{
				mask &= ~HEXKV_MASK_UPDATED;
				mask |= HEXKV_MASK_PERSIST;
				set_dict_entity_delta(ent, mask);
			}
		}

		ent = nxt;
	}
}