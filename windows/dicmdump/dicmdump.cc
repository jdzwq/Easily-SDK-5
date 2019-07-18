// dicmdump.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <tchar.h>
#include <conio.h>

#include <xdl.h>
#include <dicm.h>

int _tmain(int argc, _TCHAR* argv[])
{
	tchar_t sz_src[PATH_LEN] = { 0 };
	tchar_t sz_dst[PATH_LEN] = { 0 };
	tchar_t sz_ext[INT_LEN] = { 0 };

	xdl_process_init(XDL_APARTMENT_PROCESS);

	if (argc > 1)
	{
		xscpy(sz_src, argv[1]);
	}

	if (argc > 2)
	{
		xscpy(sz_dst, argv[2]);
	}

	if (is_null(sz_src) && is_null(sz_dst))
	{
		_tprintf(_T("dicmdump DICMFilePath [TargetFilePath(.bmp|.jpg|.png|.xml|.txt)]\n"));
	}
	else if (is_null(sz_dst))
	{
		XHANDLE sxf = xfile_open(NULL, sz_src, FILE_OPEN_READ);
		if (sxf)
		{
			XOBJECT stm = stream_alloc(sxf);

			XHANDLE std = xcons_stdout();

			XOBJECT dtm = stream_alloc(std);
			stream_set_encode(dtm, _GB2312);

			dump_dicm(dtm, stm);

			stream_free(stm);
			stream_free(dtm);
			xfile_close(sxf);
		}
	}
	else
	{
		XHANDLE sxf = xfile_open(NULL, sz_src, FILE_OPEN_READ);
		if (sxf)
		{
			XOBJECT stm = stream_alloc(sxf);

			split_path(sz_dst, NULL, NULL, sz_ext);
			if (compare_text(sz_ext, -1, _T("xml"), -1, 1) == 0)
			{
				LINKPTR dom = create_dom_doc();
				parse_dicm_doc_from_stream(dom, stm);

				save_dom_doc_to_file(dom, NULL, sz_dst);
				destroy_dom_doc(dom);
			}
			else if (compare_text(sz_ext, -1, _T("txt"), -1, 1) == 0)
			{
				xhand_t dxf = xfile_open(NULL, sz_dst, FILE_OPEN_CREATE);
				if (dxf)
				{
					XOBJECT dtm = stream_alloc(dxf);
					stream_set_encode(dtm, _GB2312);

					dump_dicm(dtm, stm);

					stream_free(dtm);
					xfile_close(dxf);
				}
			}

			stream_free(stm);
			xfile_close(sxf);
		}
	}

	xdl_process_uninit();

	return 0;
}
