// dicmdump.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#ifdef _OS_WINDOWS
#include <conio.h>
#endif

#include <xdl.h>
#include <dicm.h>
#include <mded.h>

static void usage()
{
	_tprintf(_T("xDump [-dcm|-med] SourceFile(.dcm|.med) [TargetFile(.xml|.txt)]\n"));
}

#ifdef _OS_WINDOWS
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	tchar_t sz_src[PATH_LEN + 1] = { 0 };
	tchar_t sz_tag[INT_LEN + 1] = { 0 };
	tchar_t sz_dst[PATH_LEN + 1] = { 0 };
	tchar_t sz_ext[INT_LEN + 1] = { 0 };

	if (argc < 2)
	{
		usage();
		exit(0);
	}

	xdl_process_init(XDL_APARTMENT_PROCESS);

	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == _T('-'))
			xscpy(sz_tag, (tchar_t*)(argv[i]) + 1);
		else if (is_null(sz_src))
			xscpy(sz_src, argv[i]);
		else if (is_null(sz_dst))
			xscpy(sz_dst, argv[i]);
	}

	stream_t stm_src;
	file_t xf_src = xfile_open(NULL, sz_src, FILE_OPEN_READ);
	if (xf_src)
	{
		if (is_null(sz_tag))
		{
			split_path(sz_src, NULL, NULL, sz_tag);
		}

		stm_src = stream_alloc(xfile_bio(xf_src));

		if (is_null(sz_dst))
		{
			xhand_t std = xcons_alloc();

			if (std)
			{
				stream_t stm_dst = stream_alloc(std);
				stream_set_encode(stm_dst, DEF_MBS);

				if (compare_text(sz_tag, -1, _T("med"), -1, 1) == 0)
					dump_mded(stm_dst, MED_OPERA_FILEHEAD | MED_OPERA_DATASET, stm_src);
				else
					dump_dicm(stm_dst, DICM_OPERA_FILEHEAD | DICM_OPERA_DATASET, stm_src);

				stream_free(stm_dst);
				xcons_free(std);
			}
		}
		else
		{
			split_path(sz_dst, NULL, NULL, sz_ext);

			if (compare_text(sz_ext, -1, _T("xml"), -1, 1) == 0)
			{
				LINKPTR dom = create_dom_doc();

				if (compare_text(sz_tag, -1, _T("med"), -1, 1) == 0)
					decode_mded_doc_from_stream(dom, MED_OPERA_FILEHEAD | MED_OPERA_DATASET, stm_src);
				else
					parse_dicm_doc_from_stream(dom, DICM_OPERA_FILEHEAD | DICM_OPERA_DATASET, stm_src);

				save_dom_doc_to_file(dom, NULL, sz_dst);

				destroy_dom_doc(dom);
			}
			else if (compare_text(sz_ext, -1, _T("txt"), -1, 1) == 0)
			{
				file_t xf_dst = xfile_open(NULL, sz_dst, FILE_OPEN_CREATE);
				if (xf_dst)
				{
					stream_t stm_dst = stream_alloc(xfile_bio(xf_dst));
					stream_set_encode(stm_dst, DEF_MBS);

					if (compare_text(sz_tag, -1, _T("med"), -1, 1) == 0)
						dump_mded(stm_dst, MED_OPERA_FILEHEAD | MED_OPERA_DATASET, stm_src);
					else
						dump_dicm(stm_dst, MED_OPERA_FILEHEAD | MED_OPERA_DATASET, stm_src);

					stream_free(stm_dst);
					xfile_close(xf_dst);
				}
			}
		}

		stream_free(stm_src);

		xfile_close(xf_src);
	}

	xdl_process_uninit();

	return 0;
}
