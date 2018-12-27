#pragma once

typedef struct _SPLITOR{
	link_t_ptr split;

	short org_x, org_y;
	link_t_ptr org_item;
}SPLITOR,*LPSPLITOR;

LRESULT CALLBACK EasilySplitorProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
