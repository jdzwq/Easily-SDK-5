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

static bool_t _load_tdb_node(variant_t key, object_t val, void* pv)
{
	link_t_ptr dict = (link_t_ptr)pv;
	link_t_ptr ent;
	vword_t mask;
	key64_t k;
	key64_t v;
	variant_t kh = NULL;
	object_t vh = NULL;

	variant_hash64(key, &k);
	kh = variant_alloc(VV_LONG);
	variant_set_long(kh, k);

	object_hash64(val, &v);
	vh = object_alloc();
	object_set_bytes(vh, (byte_t*)&v, sizeof(key64_t));

	ent = write_dict_item(dict, kh, NULL);
	attach_dict_entity_val(ent, vh);

	mask = get_dict_entity_delta(ent);
	mask |= TKV_MASK_PERSIST;
	set_dict_entity_delta(ent, mask);

	variant_free(kh);

	return 1;
}

t_kv_t tkv_create(t_kb_t hdb)
{
	t_kv_ctx* pobj;

	pobj = (t_kv_ctx*)xmem_alloc(sizeof(t_kv_ctx));

	pobj->dict = create_dict_table();
	pobj->hdb = hdb;

	pobj->hdr.tag = T_OBJ_KV;

	if (pobj->hdb)
	{
		tkb_enum(pobj->hdb, _load_tdb_node, (void*)pobj->dict);
	}

	return &(pobj->hdr);
}

void tkv_destroy(t_kv_t hkv)
{
	t_kv_ctx* pobj = (t_kv_ctx*)hkv;

	XDL_ASSERT(hkv && hkv->tag == T_OBJ_KV);

	destroy_dict_table(pobj->dict);

	xmem_free(pobj);
}

bool_t tkv_write(t_kv_t hkv, variant_t key, object_t val)
{
	t_kv_ctx* pobj = (t_kv_ctx*)hkv;
	link_t_ptr ent;
	vword_t mask;
	key64_t k;
	key64_t v, s;
	variant_t kh = NULL;
	object_t vh = NULL;
	const object_t* pv;

	tchar_t errcode[NUM_LEN + 1] = { 0 };

	XDL_ASSERT(hkv && hkv->tag == T_OBJ_KV);

	TRY_CATCH;

	if (pobj->hdb)
	{
		variant_hash64(key, &k);
		kh = variant_alloc(VV_LONG);
		variant_set_long(kh, k);

		object_hash64(val, &v);
		vh = object_alloc();
		object_set_bytes(vh, (byte_t*)&v, sizeof(key64_t));

		ent = get_dict_entity(pobj->dict, kh);
		if (ent)
		{
			pv = get_dict_entity_val_ptr(ent);
			if (pv)
			{
				object_get_bytes(*pv, (byte_t*)&s, sizeof(key64_t));
				if (v == s)
				{
					raise_user_error(_T("1"), _T("value exists"));
				}
			}
		}

		if (!tkb_save(pobj->hdb, key, val))
		{
			raise_user_error(_T("0"), _T("tkb save failed"));
		}

		ent = write_dict_item(pobj->dict, kh, NULL);
		attach_dict_entity_val(ent, vh);
		vh = NULL;

		variant_free(kh);
		kh = NULL;

		mask = get_dict_entity_delta(ent);
		mask |= TKV_MASK_PERSIST;
		set_dict_entity_delta(ent, mask);
	}
	else
	{
		ent = write_dict_item(pobj->dict, key, val);

		mask = get_dict_entity_delta(ent);
		mask &= (~TKV_MASK_PERSIST);
		set_dict_entity_delta(ent, mask);
	}

	END_CATCH;

	return 1;

ONERROR:
	get_last_error(errcode, NULL, 0);

	if (kh)
		variant_free(kh);

	if (vh)
		object_free(vh);

	XDL_TRACE_LAST;
	
	return (xstol(errcode)) ? 1 : 0;
}

bool_t tkv_update(t_kv_t hkv, variant_t key, object_t val)
{
	t_kv_ctx* pobj = (t_kv_ctx*)hkv;
	link_t_ptr ent;
	vword_t mask;
	key64_t k;
	key64_t v, s;
	variant_t kh = NULL;
	object_t vh = NULL;
	const object_t* pv;

	tchar_t errcode[NUM_LEN + 1] = { 0 };

	XDL_ASSERT(hkv && hkv->tag == T_OBJ_KV);

	TRY_CATCH;

	if (pobj->hdb)
	{
		variant_hash64(key, &k);
		kh = variant_alloc(VV_LONG);
		variant_set_long(kh, k);

		object_hash64(val, &v);
		vh = object_alloc();
		object_set_bytes(vh, (byte_t*)&v, sizeof(key64_t));

		ent = get_dict_entity(pobj->dict, kh);
		if (!ent)
		{
			raise_user_error(_T("0"), _T("key not exists"));
		}

		pv = get_dict_entity_val_ptr(ent);
		if (pv)
		{
			object_get_bytes(*pv, (byte_t*)&s, sizeof(key64_t));
			if (v == s)
			{
				raise_user_error(_T("1"), _T("value exists"));
			}
		}

		if (!tkb_save(pobj->hdb, key, val))
		{
			raise_user_error(_T("0"), _T("tkb save failed"));
		}

		ent = write_dict_item(pobj->dict, kh, NULL);
		attach_dict_entity_val(ent, vh);
		vh = NULL;

		variant_free(kh);
		kh = NULL;

		mask = get_dict_entity_delta(ent);
		mask |= TKV_MASK_PERSIST;
		set_dict_entity_delta(ent, mask);
	}
	else
	{
		ent = get_dict_entity(pobj->dict, key);
		if (!ent)
		{
			raise_user_error(_T("0"), _T("key not exists"));
		}

		set_dict_entity_val(ent, val);

		mask = get_dict_entity_delta(ent);
		mask &= (~TKV_MASK_PERSIST);
		set_dict_entity_delta(ent, mask);
	}

	END_CATCH;

	return 1;

ONERROR:
	get_last_error(errcode, NULL, 0);

	if (kh)
		variant_free(kh);

	if (vh)
		object_free(vh);

	XDL_TRACE_LAST;

	return (xstol(errcode)) ? 1 : 0;
}

bool_t tkv_read(t_kv_t hkv, variant_t key, object_t val)
{
	t_kv_ctx* pobj = (t_kv_ctx*)hkv;
	link_t_ptr ent;
	vword_t mask;
	key64_t k;
	key64_t v;
	variant_t kh = NULL;
	object_t vh = NULL;

	tchar_t errcode[NUM_LEN + 1] = { 0 };

	XDL_ASSERT(hkv && hkv->tag == T_OBJ_KV);

	TRY_CATCH;

	if (pobj->hdb)
	{
		if (!tkb_load(pobj->hdb, key, val))
		{
			raise_user_error(_T("0"), _T("key not exists"));
		}
		
		variant_hash64(key, &k);
		kh = variant_alloc(VV_LONG);
		variant_set_long(kh, k);

		object_hash64(val, &v);
		vh = object_alloc();
		object_set_bytes(vh, (byte_t*)&v, sizeof(key64_t));

		ent = write_dict_item(pobj->dict, kh, NULL);
		attach_dict_entity_val(ent, vh);
		vh = NULL;

		variant_free(kh);
		kh = NULL;

		mask = get_dict_entity_delta(ent);
		mask |= TKV_MASK_PERSIST;
		set_dict_entity_delta(ent, mask);
	}
	else
	{
		ent = get_dict_entity(pobj->dict, key);
		if (!ent)
		{
			raise_user_error(_T("0"), _T("key not exists"));
		}

		get_dict_entity_val(ent, val);
	}

	END_CATCH;

	return 1;

ONERROR:
	get_last_error(errcode, NULL, 0);

	if (kh)
		variant_free(kh);

	if (vh)
		object_free(vh);

	XDL_TRACE_LAST;

	return (xstol(errcode)) ? 1 : 0;
}

bool_t tkv_delete(t_kv_t hkv, variant_t key)
{
	t_kv_ctx* pobj = (t_kv_ctx*)hkv;
	link_t_ptr ent;
	key64_t k;
	variant_t kh = NULL;

	XDL_ASSERT(hkv && hkv->tag == T_OBJ_KV);

	TRY_CATCH;

	if (pobj->hdb)
	{
		if (!tkb_clean(pobj->hdb, key))
		{
			raise_user_error(_T("0"), _T("tkb clean falied"));
		}

		variant_hash64(key, &k);
		kh = variant_alloc(VV_LONG);
		variant_set_long(kh, k);

		ent = get_dict_entity(pobj->dict, kh);
		if (ent)
		{
			delete_dict_entity(ent);
		}

		variant_free(kh);
		kh = NULL;
	}
	else
	{
		ent = get_dict_entity(pobj->dict, key);
		if (ent)
		{
			delete_dict_entity(ent);
		}
	}

	END_CATCH;

	return 1;

ONERROR:

	if (kh)
		variant_free(kh);

	XDL_TRACE_LAST;

	return 0;
}

bool_t tkv_attach(t_kv_t hkv, variant_t key, object_t val)
{
	t_kv_ctx* pobj = (t_kv_ctx*)hkv;
	link_t_ptr ent;
	vword_t mask;
	key64_t k;
	key64_t v, s;
	variant_t kh = NULL;
	object_t vh = NULL;
	const object_t* pv;

	tchar_t errcode[NUM_LEN + 1] = { 0 };

	XDL_ASSERT(hkv && hkv->tag == T_OBJ_KV);

	TRY_CATCH;

	if (pobj->hdb)
	{
		variant_hash64(key, &k);
		kh = variant_alloc(VV_LONG);
		variant_set_long(kh, k);

		object_hash64(val, &v);
		vh = object_alloc();
		object_set_bytes(vh, (byte_t*)&v, sizeof(key64_t));

		ent = get_dict_entity(pobj->dict, kh);
		if (ent)
		{
			pv = get_dict_entity_val_ptr(ent);
			if (pv)
			{
				object_get_bytes(*pv, (byte_t*)&s, sizeof(key64_t));
				if (v == s)
				{
					raise_user_error(_T("1"), _T("value exists"));
				}
			}
		}

		if (!tkb_save(pobj->hdb, key, val))
		{
			raise_user_error(_T("0"), _T("tkb save failed"));
		}

		object_free(val);
		val = NULL;

		ent = write_dict_item(pobj->dict, kh, NULL);
		attach_dict_entity_val(ent, vh);
		vh = NULL;

		variant_free(kh);
		kh = NULL;

		mask = get_dict_entity_delta(ent);
		mask |= TKV_MASK_PERSIST;
		set_dict_entity_delta(ent, mask);
	}
	else
	{
		ent = write_dict_item(pobj->dict, key, NULL);
		attach_dict_entity_val(ent, val);
		val = NULL;
		
		mask = get_dict_entity_delta(ent);
		mask &= (~TKV_MASK_PERSIST);
		set_dict_entity_delta(ent, mask);
	}

	END_CATCH;

	return 1;

ONERROR:
	get_last_error(errcode, NULL, 0);

	if (kh)
		variant_free(kh);

	if (vh)
		object_free(vh);

	if (val)
		object_free(val);

	XDL_TRACE_LAST;

	return (xstol(errcode)) ? 1 : 0;
}

object_t tkv_detach(t_kv_t hkv, variant_t key)
{
	t_kv_ctx* pobj = (t_kv_ctx*)hkv;
	link_t_ptr ent;
	key64_t k;
	variant_t kh = NULL;
	object_t val = NULL;

	XDL_ASSERT(hkv && hkv->tag == T_OBJ_KV);

	TRY_CATCH;

	if (pobj->hdb)
	{
		val = object_alloc();

		if (!tkb_load(pobj->hdb, key, val))
		{
			raise_user_error(_T("0"), _T("tkb load failed"));
		}

		if (!tkb_clean(pobj->hdb, key))
		{
			raise_user_error(_T("0"), _T("tkb clean failed"));
		}

		variant_hash64(key, &k);
		kh = variant_alloc(VV_LONG);
		variant_set_long(kh, k);

		ent = get_dict_entity(pobj->dict, kh);
		if (ent)
		{
			delete_dict_entity(ent);
		}

		variant_free(kh);
		kh = NULL;
	}
	else
	{
		ent = get_dict_entity(pobj->dict, kh);
		if (ent)
		{
			val = detach_dict_entity_val(ent);
			delete_dict_entity(ent);
		}
		else
		{
			val = NULL;
		}
	}

	END_CATCH;

	return val;

ONERROR:

	if (kh)
		variant_free(kh);

	if (val)
		object_free(val);

	XDL_TRACE_LAST;

	return NULL;
}

bool_t tkv_flush(t_kv_t hkv, t_kb_t hdb)
{
	t_kv_ctx* pobj = (t_kv_ctx*)hkv;
	dict_enum_t de = { 0 };
	link_t_ptr ent,nxt;
	vword_t mask;
	key64_t k;
	key64_t v;
	variant_t kh = NULL;
	object_t vh = NULL;

	const variant_t *pkey;
	const object_t* pval;

	XDL_ASSERT(hkv && hkv->tag == T_OBJ_KV);

	if (pobj->hdb || !hdb)
		return 0;

	TRY_CATCH;

	pobj->hdb = hdb;

	de.dict = pobj->dict;
	de.entity = LINK_FIRST;

	ent = get_dict_next_entity(&de);
	while (ent)
	{
		nxt = get_dict_next_entity(&de);

		mask = get_dict_entity_delta(ent);
		if (mask & TKV_MASK_PERSIST)
		{
			ent = nxt;
			continue;
		}

		pkey = get_dict_entity_key_ptr(ent);
		pval = get_dict_entity_val_ptr(ent);

		if (!tkb_save(pobj->hdb, *pkey, *pval))
		{
			raise_user_error(_T("0"), _T("tkb save failed"));
		}

		variant_hash64(*pkey, &k);
		kh = variant_alloc(VV_LONG);
		variant_set_long(kh, k);

		object_hash64(*pval, &v);
		vh = object_alloc();
		object_set_bytes(vh, (byte_t*)&v, sizeof(key64_t));

		delete_dict_entity(ent);

		ent = write_dict_item(pobj->dict, kh, NULL);
		attach_dict_entity_val(ent, vh);
		vh = NULL;

		variant_free(kh);
		kh = NULL;

		mask = get_dict_entity_delta(ent);
		mask |= TKV_MASK_PERSIST;
		set_dict_entity_delta(ent, mask);

		ent = nxt;
	}

	END_CATCH;

	return 1;

ONERROR:
	
	if (kh)
		variant_free(kh);

	if (vh)
		object_free(vh);

	XDL_TRACE_LAST;

	return 0;
}

#ifdef XDL_SUPPORT_TEST
static int e_count = 0;
static int h_count = 0;
static int v_count = 0;
static int k_count = 0;

static bool_t _check_tdb_node(variant_t key, object_t val, void* pv)
{
	link_t_ptr dict = (link_t_ptr)pv;
	link_t_ptr ent;

	key64_t k;
	key64_t v, s;
	variant_t kh = NULL;
	const object_t *pvh;
	object_t vh = NULL;
	tchar_t str[NUM_LEN + 1];

	variant_to_string(key, str, NUM_LEN);

	variant_hash64(key, &k);
	kh = variant_alloc(VV_LONG);
	variant_set_long(kh, k);

	object_hash64(val, &v);
	vh = object_alloc();
	object_set_bytes(vh, (byte_t*)&v, sizeof(key64_t));

	ent = get_dict_entity(dict, kh);
	if (ent)
	{
		pvh = get_dict_entity_val_ptr(ent);
		if (pvh)
		{
			object_get_bytes(*pvh, (byte_t*)&s, sizeof(key64_t));
			if (v == s)
			{
				e_count++;
				//_tprintf(_T("%s: passed\n"), str);
			}
			else
			{
				h_count++;
				_tprintf(_T("%s: hash not equal\n"), str);
			}
		}
		else
		{
			v_count++;
			_tprintf(_T("%s: value not exist\n"), str);
		}
	}
	else
	{
		k_count++;
		_tprintf(_T("%s: key not exist\n"), str);
	}

	variant_free(kh);

	object_free(vh);

	return 1;
}

void test_tkv()
{
	t_kv_t hkv = tkv_create(NULL);
	
	int i;
	tchar_t kid[NUM_LEN + 1] = { 0 };

	variant_t key = variant_alloc(VV_STRING_UTF8);
	object_t val = object_alloc();
	
	for (i = 0; i < 10000; i++)
	{
		xsprintf(kid, _T("key%d"), i);
		variant_from_string(key, kid, -1);

		object_set_variant(val, key);

		tkv_write(hkv, key, val);
	}

	variant_free(key);

	object_free(val);

	t_kb_t hkb = tkb_create(_T("."), _T("tvtest"), 1);

	tkv_flush(hkv, hkb);

	t_kv_ctx* pobj = (t_kv_ctx*)hkv;

	tkb_enum(hkb, _check_tdb_node, (void*)pobj->dict);

	tkv_destroy(hkv);
	hkv = NULL;

	tkb_destroy(hkb);
	hkb = NULL;

	_tprintf(_T("checked:%d, hash error:%d, value error:%d, key error:%d\n"), e_count, h_count, v_count, k_count);
}

#endif