/***********************************************************************
	Easily xds v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc color document

	@module	xdscolor.c | implement file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
	@devnote 张文权 2018.01 - 2018.12	v5.5
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

#include "xdscolor.h"
#include "gdiattr.h"
#include "xdsstr.h"

typedef struct _color_map_t{
	tchar_t en_color[32];
	tchar_t cn_color[32];
	tchar_t bin_color[16];
	tchar_t rgb_color[24];
}color_map_t;

static color_map_t color_map[] = {
	{ _T("AliceBlue"), _T("爱丽丝蓝"), _T("#F0F8FF"), _T("RGB(240,248,255)") },
	{ _T("AntiqueWhite"), _T("古董白"), _T("#FAEBD7"), _T("RGB(250,235,215)") },
	{ _T("Aqua"), _T("浅绿色"), _T("#00FFFF"), _T("RGB(0,255,255)") },
	{ _T("Aquamarine"), _T("宝石碧绿"), _T("#7FFFD4"), _T("RGB(127,255,212)") },
	{ _T("Azure"), _T("蔚蓝色"), _T("#F0FFFF"), _T("RGB(240,255,255)") },
	{ _T("Beige"), _T("米色"), _T("#F5F5DC"), _T("RGB(245,245,220)") },
	{ _T("Bisque"), _T("陶坯黄"), _T("#FFE4C4"), _T("RGB(255,228,196)") },
	{ _T("Black"), _T("暗淡的灰色"), _T("#000000"), _T("RGB(0,0,0)") },
	{ _T("BlanchedAlmond"), _T("白杏色"), _T("#FFEBCD"), _T("RGB(255,235,205)") },
	{ _T("Blue"), _T("纯蓝"), _T("#0000FF"), _T("RGB(0,0,255)") },
	{ _T("BlueViolet"), _T("蓝紫罗兰"), _T("#8A2BE2"), _T("RGB(138,43,226)") },
	{ _T("Brown"), _T("棕色"), _T("#A52A2A"), _T("RGB(165,42,42)") },
	{ _T("BurlyWood"), _T("硬木色"), _T("#DEB887"), _T("RGB(222,184,135)") },
	{ _T("CadetBlue"), _T("军兰色"), _T("#5F9EA0"), _T("RGB(95,158,160)") },
	{ _T("Chartreuse"), _T("黄绿色"), _T("#7FFF00"), _T("RGB(127,255,0)") },
	{ _T("Chocolate"), _T("巧克力色"), _T("#D2691E"), _T("RGB(210,105,30)") },
	{ _T("Coral"), _T("珊瑚"), _T("#FF7F50"), _T("RGB(255,127,80)") },
	{ _T("CornflowerBlue"), _T("矢车菊蓝"), _T("#6495ED"), _T("RGB(100,149,237)") },
	{ _T("Cornsilk"), _T("玉米丝色"), _T("#FFF8DC"), _T("RGB(255,248,220)") },
	{ _T("Crimson"), _T("深红"), _T("#DC143C"), _T("RGB(220,20,60)") },
	{ _T("Cyan"), _T("青色"), _T("#00FFFF"), _T("RGB(0,255,255)") },
	{ _T("DarkBlue"), _T("暗蓝色"), _T("#00008B"), _T("RGB(0,0,139)") },
	{ _T("DarkCyan"), _T("暗青色"), _T("#008B8B"), _T("RGB(0,139,139)") },
	{ _T("DarkGoldenrod"), _T("暗金菊黄"), _T("#B8860B"), _T("RGB(184,134,11)") },
	{ _T("DarkGray"), _T("深灰色"), _T("#A9A9A9"), _T("RGB(169,169,169)") },
	{ _T("DarkGreen"), _T("暗绿色"), _T("#006400"), _T("RGB(0,100,0)") },
	{ _T("DarkKhaki"), _T("暗黄褐色"), _T("#BDB76B"), _T("RGB(189,183,107)") },
	{ _T("DarkMagenta"), _T("深洋红"), _T("#8B008B"), _T("RGB(139,0,139)") },
	{ _T("DarkOliveGreen"), _T("暗橄榄绿"), _T("#556B2F"), _T("RGB(85,107,47)") },
	{ _T("DarkOrange"), _T("深橙色"), _T("#FF8C00"), _T("RGB(255,140,0)") },
	{ _T("DarkOrchid"), _T("暗兰花紫"), _T("#9932CC"), _T("RGB(153,50,204)") },
	{ _T("DarkRed"), _T("深红色"), _T("#8B0000"), _T("RGB(139,0,0)") },
	{ _T("DarkSalmon"), _T("暗鲑鱼色"), _T("#E9967A"), _T("RGB(233,150,122)") },
	{ _T("DarkSeaGreen"), _T("暗海洋绿"), _T("#8FBC8F"), _T("RGB(143,188,143)") },
	{ _T("DarkSlateBlue"), _T("暗灰蓝色"), _T("#483D8B"), _T("RGB(72,61,139)") },
	{ _T("DarkSlateGray"), _T("暗瓦灰色"), _T("#2F4F4F"), _T("RGB(47,79,79)") },
	{ _T("DarkTurquoise"), _T("暗绿宝石"), _T("#00CED1"), _T("RGB(0,206,209)") },
	{ _T("DarkViolet"), _T("暗紫罗兰"), _T("#9400D3"), _T("RGB(148,0,211)") },
	{ _T("DeepPink"), _T("深粉红"), _T("#FF1493"), _T("RGB(255,20,147)") },
	{ _T("DeepSkyBlue"), _T("深天蓝"), _T("#00BFFF"), _T("RGB(0,191,255)") },
	{ _T("DimGray"), _T("暗淡的灰色"), _T("#696969"), _T("RGB(105,105,105)") },
	{ _T("DodgerBlue"), _T("道奇蓝"), _T("#1E90FF"), _T("RGB(30,144,255)") },
	{ _T("FireBrick"), _T("火砖色"), _T("#B22222"), _T("RGB(178,34,34)") },
	{ _T("FloralWhite"), _T("花白色"), _T("#FFFAF0"), _T("RGB(255,250,240)") },
	{ _T("ForestGreen"), _T("森林绿"), _T("#228B22"), _T("RGB(34,139,34)") },
	{ _T("Fuchsia"), _T("紫红"), _T("#FF00FF"), _T("RGB(255,0,255)") },
	{ _T("Gainsboro"), _T("淡灰色"), _T("#DCDCDC"), _T("RGB(220,220,220)") },
	{ _T("GhostWhite"), _T("幽灵白"), _T("#F8F8FF"), _T("RGB(248,248,255)") },
	{ _T("Gold"), _T("金色"), _T("#FFD700"), _T("RGB(255,215,0)") },
	{ _T("Goldenrod"), _T("金菊黄"), _T("#DAA520"), _T("RGB(218,165,32)") },
	{ _T("Gray"), _T("灰色"), _T("#808080"), _T("RGB(128,128,128)") },
	{ _T("Green"), _T("纯绿"), _T("#008000"), _T("RGB(0,128,0)") },
	{ _T("GreenYellow"), _T("绿黄色"), _T("#ADFF2F"), _T("RGB(173,255,47)") },
	{ _T("Honeydew"), _T("蜜瓜色"), _T("#F0FFF0"), _T("RGB(240,255,240)") },
	{ _T("HotPink"), _T("热情的粉红"), _T("#FF69B4"), _T("RGB(255,105,180)") },
	{ _T("IndianRed"), _T("印度红"), _T("#CD5C5C"), _T("RGB(205,92,92)") },
	{ _T("Indigo"), _T("靛青"), _T("#4B0082"), _T("RGB(75,0,130)") },
	{ _T("Ivory"), _T("象牙色"), _T("#FFFFF0"), _T("RGB(255,255,240)") },
	{ _T("Khaki"), _T("黄褐色"), _T("#F0E68C"), _T("RGB(240,230,140)") },
	{ _T("Lavender"), _T("淡紫色"), _T("#E6E6FA"), _T("RGB(230,230,250)") },
	{ _T("LavenderBlush"), _T("淡紫红"), _T("#FFF0F5"), _T("RGB(255,240,245)") },
	{ _T("LawnGreen"), _T("草绿色"), _T("#7CFC00"), _T("RGB(124,252,0)") },
	{ _T("LemonChiffon"), _T("柠檬绸"), _T("#FFFACD"), _T("RGB(255,250,205)") },
	{ _T("LightBlue"), _T("亮蓝"), _T("#ADD8E6"), _T("RGB(173,216,230)") },
	{ _T("LightCoral"), _T("淡珊瑚色"), _T("#F08080"), _T("RGB(240,128,128)") },
	{ _T("LightCyan"), _T("淡青色"), _T("#E0FFFF"), _T("RGB(224,255,255)") },
	{ _T("LightGoldenrodYellow"), _T("亮菊黄"), _T("#FAFAD2"), _T("RGB(250,250,210)") },
	{ _T("LightGray"), _T("浅灰色"), _T("#D3D3D3"), _T("RGB(211,211,211)") },
	{ _T("LightGreen"), _T("淡绿色"), _T("#90EE90"), _T("RGB(144,238,144)") },
	{ _T("LightPink"), _T("浅粉红"), _T("#FFB6C1"), _T("RGB(255,182,193)") },
	{ _T("LightSalmon"), _T("浅鲑鱼肉色"), _T("#FFA07A"), _T("RGB(255,160,122)") },
	{ _T("LightSeaGreen"), _T("浅海洋绿"), _T("#20B2AA"), _T("RGB(32,178,170)") },
	{ _T("LightSkyBlue"), _T("亮天蓝色"), _T("#87CEFA"), _T("RGB(135,206,250)") },
	{ _T("LightSlateGray"), _T("亮蓝灰"), _T("#778899"), _T("RGB(119,136,153)") },
	{ _T("LightSteelBlue"), _T("亮钢蓝"), _T("#B0C4DE"), _T("RGB(176,196,222)") },
	{ _T("LightYellow"), _T("浅黄色"), _T("#FFFFE0"), _T("RGB(255,255,224)") },
	{ _T("Lime"), _T("闪光绿"), _T("#00FF00"), _T("RGB(0,255,0)") },
	{ _T("LimeGreen"), _T("闪光深绿"), _T("#32CD32"), _T("RGB(50,205,50)") },
	{ _T("Linen"), _T("亚麻布"), _T("#FAF0E6"), _T("RGB(250,240,230)") },
	{ _T("Magenta"), _T("洋红"), _T("#FF00FF"), _T("RGB(255,0,255)") },
	{ _T("Maroon"), _T("栗色"), _T("#800000"), _T("RGB(128,0,0)") },
	{ _T("MediumAquamarine"), _T("中宝石碧绿"), _T("#66CDAA"), _T("RGB(102,205,170)") },
	{ _T("MediumBlue"), _T("中蓝色"), _T("#0000CD"), _T("RGB(0,0,205)") },
	{ _T("MediumOrchid"), _T("中兰花紫"), _T("#BA55D3"), _T("RGB(186,85,211)") },
	{ _T("MediumPurple"), _T("中紫色"), _T("#9370DB"), _T("RGB(147,112,219)") },
	{ _T("MediumSeaGreen"), _T("中海洋绿"), _T("#3CB371"), _T("RGB(60,179,113)") },
	{ _T("MediumSlateBlue"), _T("中暗蓝色"), _T("#7B68EE"), _T("RGB(123,104,238)") },
	{ _T("MediumSpringGreen"), _T("中春绿色"), _T("#00FA9A"), _T("RGB(0,250,154)") },
	{ _T("MediumTurquoise"), _T("中绿宝石"), _T("#48D1CC"), _T("RGB(72,209,204)") },
	{ _T("MediumVioletRed"), _T("中紫罗兰红"), _T("#C71585"), _T("RGB(199,21,133)") },
	{ _T("MidnightBlue"), _T("午夜蓝"), _T("#191970"), _T("RGB(25,25,112)") },
	{ _T("MintCream"), _T("薄荷奶油"), _T("#F5FFFA"), _T("RGB(245,255,250)") },
	{ _T("MistyRose"), _T("浅玫瑰色"), _T("#FFE4E1"), _T("RGB(255,228,225)") },
	{ _T("Moccasin"), _T("鹿皮色"), _T("#FFE4B5"), _T("RGB(255,228,181)") },
	{ _T("NavajoWhite"), _T("纳瓦白"), _T("#FFDEAD"), _T("RGB(255,222,173)") },
	{ _T("Navy"), _T("海军蓝"), _T("#000080"), _T("RGB(0,0,128)") },
	{ _T("OldLace"), _T("老花色"), _T("#FDF5E6"), _T("RGB(253,245,230)") },
	{ _T("Olive"), _T("橄榄"), _T("#808000"), _T("RGB(128,128,0)") },
	{ _T("OliveDrab"), _T("橄榄褐色"), _T("#6B8E23"), _T("RGB(107,142,35)") },
	{ _T("Orange"), _T("橙色"), _T("#FFA500"), _T("RGB(255,165,0)") },
	{ _T("OrangeRed"), _T("橙红色"), _T("#FF4500"), _T("RGB(255,69,0)") },
	{ _T("Orchid"), _T("暗紫色"), _T("#DA70D6"), _T("RGB(218,112,214)") },
	{ _T("PaleGoldenrod"), _T("灰菊黄"), _T("#EEE8AA"), _T("RGB(238,232,170)") },
	{ _T("PaleGreen"), _T("弱绿色"), _T("#98FB98"), _T("RGB(152,251,152)") },
	{ _T("PaleTurquoise"), _T("弱绿宝石"), _T("#AFEEEE"), _T("RGB(175,238,238)") },
	{ _T("PaleVioletRed"), _T("弱紫罗兰红"), _T("#DB7093"), _T("RGB(219,112,147)") },
	{ _T("PapayaWhip"), _T("番木色"), _T("#FFEFD5"), _T("RGB(255,239,213)") },
	{ _T("PeachPuff"), _T("桃肉色"), _T("#FFDAB9"), _T("RGB(255,218,185)") },
	{ _T("Peru"), _T("秘鲁色"), _T("#CD853F"), _T("RGB(205,133,63)") },
	{ _T("Pink"), _T("粉红"), _T("#FFC0CB"), _T("RGB(255,192,203)") },
	{ _T("Plum"), _T("洋李色"), _T("#DDA0DD"), _T("RGB(221,160,221)") },
	{ _T("PowderBlue"), _T("粉蓝色"), _T("#B0E0E6"), _T("RGB(176,224,230)") },
	{ _T("Purple"), _T("紫色"), _T("#800080"), _T("RGB(128,0,128)") },
	{ _T("Red"), _T("纯红"), _T("#FF0000"), _T("RGB(255,0,0)") },
	{ _T("RosyBrown"), _T("玫瑰棕色"), _T("#BC8F8F"), _T("RGB(188,143,143)") },
	{ _T("RoyalBlue"), _T("宝蓝"), _T("#4169E1"), _T("RGB(65,105,225)") },
	{ _T("SaddleBrown"), _T("重褐色"), _T("#8B4513"), _T("RGB(139,69,19)") },
	{ _T("Salmon"), _T("鲑鱼色"), _T("#FA8072"), _T("RGB(250,128,114") },
	{ _T("SandyBrown"), _T("沙棕色"), _T("#F4A460"), _T("RGB(244,164,96)") },
	{ _T("SeaGreen"), _T("海洋绿"), _T("#2E8B57"), _T("RGB(46,139,87)") },
	{ _T("Seashell"), _T("海贝壳"), _T("#FFF5EE"), _T("RGB(255,245,238)") },
	{ _T("Sienna"), _T("黄土赭色"), _T("#A0522D"), _T("RGB(160,82,45)") },
	{ _T("Silver"), _T("银灰色"), _T("#C0C0C0"), _T("RGB(192,192,192)") },
	{ _T("SkyBlue"), _T("天蓝色"), _T("#87CEEB"), _T("RGB(135,206,235)") },
	{ _T("SlateBlue"), _T("石蓝色"), _T("#6A5ACD"), _T("RGB(106,90,205)") },
	{ _T("SlateGray"), _T("石板灰"), _T("#708090"), _T("RGB(112,128,144)") },
	{ _T("Snow"), _T("雪白色"), _T("#FFFAFA"), _T("RGB(255,250,250)") },
	{ _T("SpringGreen"), _T("春绿色"), _T("#00FF7F"), _T("RGB(0,255,127)") },
	{ _T("SteelBlue"), _T("钢蓝"), _T("#4682B4"), _T("RGB(70,130,180)") },
	{ _T("Tan"), _T("茶色"), _T("#D2B48C"), _T("RGB(210,180,140)") },
	{ _T("Teal"), _T("水鸭色"), _T("#008080"), _T("RGB(0,128,128)") },
	{ _T("Thistle"), _T("蓟色"), _T("#D8BFD8"), _T("RGB(216,191,216)") },
	{ _T("Tomato"), _T("番茄红"), _T("#FF6347"), _T("RGB(255,99,71)") },
	{ _T("Turquoise"), _T("绿宝石"), _T("#40E0D0"), _T("RGB(64,224,208)") },
	{ _T("Violet"), _T("紫罗兰"), _T("#EE82EE"), _T("RGB(238,130,238)") },
	{ _T("Wheat"), _T("浅黄色"), _T("#F5DEB3"), _T("RGB(245,222,179)") },
	{ _T("White"), _T("纯白"), _T("#FFFFFF"), _T("RGB(255,255,255)") },
	{ _T("WhiteSmoke"), _T("白烟"), _T("#F5F5F5"), _T("RGB(245,245,245)") },
	{ _T("Yellow"), _T("纯黄"), _T("#FFFF00"), _T("RGB(255,255,0)") },
	{ _T("YellowGreen"), _T("黄绿色"), _T("#9ACD32"), _T("RGB(154,205,50)") }
};


static int _find_color(int i, int j, const tchar_t* cname)
{
	int rt, k;

	if (i == j)
	{
		rt = xsicmp(cname, color_map[i].en_color);
		if (!rt)
			return i;
		else
			return -1;
	}
	else if (i < j)
	{
		k = (i + j) / 2;
		rt = xsicmp(cname, color_map[k].en_color);
		if (!rt)
			return k;
		else if (rt > 0)
			return _find_color(k + 1, j, cname);
		else
			return _find_color(i, k - 1, cname);
	}
	else
	{
		return -1;
	}
}

bool_t find_color(const tchar_t* en_clr, tchar_t* cn_clr, tchar_t* rgb_clr, tchar_t* bin_clr)
{
	int k;

	if (is_null(en_clr))
		return 0;

	k = sizeof(color_map) / sizeof(color_map_t);

	k = _find_color(0, k - 1, en_clr);

	if (k < 0)
		return 0;

	if (cn_clr)
		xscpy(cn_clr, color_map[k].cn_color);
	if (rgb_clr)
		xscpy(rgb_clr, color_map[k].rgb_color);
	if (bin_clr)
		xscpy(bin_clr, color_map[k].bin_color);

	return 1;
}

bool_t next_color(const tchar_t* en, tchar_t* en_clr, tchar_t* cn_clr, tchar_t* rgb_clr, tchar_t* bin_clr)
{
	int i, k;

	if (is_null(en))
	{
		i = 0;
	}
	else
	{
		k = sizeof(color_map) / sizeof(color_map_t);
		i = _find_color(0, k - 1, en) + 1;
		if (i == 0 || i == k)
			return 0;
	}

	if (en_clr)
		xscpy(en_clr, color_map[i].en_color);
	if (cn_clr)
		xscpy(cn_clr, color_map[i].cn_color);
	if (rgb_clr)
		xscpy(rgb_clr, color_map[i].rgb_color);
	if (bin_clr)
		xscpy(bin_clr, color_map[i].bin_color);

	return 1;
}