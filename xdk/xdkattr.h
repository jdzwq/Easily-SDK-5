#ifndef _XDKATTR_H
#define _XDKATTR_H

/*********************************************************GDI attributes******************************************************/
#define GDI_ATTR_OPACITY_SOLID		_T("255")
#define GDI_ATTR_OPACITY_SOFT		_T("250")
#define GDI_ATTR_OPACITY_TRANS		_T("128")

#define GDI_ATTR_STROKE_COLOR		_T("stroke-color")
#define GDI_ATTR_STROKE_STYLE		_T("stroke-style")
#define GDI_ATTR_STROKE_WIDTH		_T("stroke-width")
#define GDI_ATTR_STROKE_LINECAP		_T("stroke-linecap")
#define GDI_ATTR_STROKE_TAILCAP		_T("stroke-tailcap")
#define GDI_ATTR_STROKE_LINEJOIN	_T("stroke-linejoin")
#define GDI_ATTR_STROKE_DASHARRAY	_T("stroke-dasharray")
#define GDI_ATTR_STROKE_OPACITY		_T("stroke-opacity")

#define GDI_ATTR_STROKE_STYLE_DOTTED	_T("dotted")
#define GDI_ATTR_STROKE_STYLE_DASHED	_T("dashed")
#define GDI_ATTR_STROKE_STYLE_SOLID		_T("solid")
#define GDI_ATTR_STROKE_STYLE_OPTIONS	_T("solid~Solid;dotted~Dotted;dashed~Dashed;")

#define GDI_ATTR_FILL_STYLE				_T("fill-style")
#define GDI_ATTR_FILL_COLOR				_T("fill-color")
#define GDI_ATTR_FILL_OPACITY			_T("fill-opacity")

#define GDI_ATTR_FILL_STYLE_SOLID		_T("solid")
#define GDI_ATTR_FILL_STYLE_HATCH		_T("hatch")
#define GDI_ATTR_FILL_STYLE_PATTERN		_T("pattern")
#define GDI_ATTR_FILL_STYLE_OPTIONS		_T("solid~Solid;hatch~Hatch;pattern~Pattern;")

#define GDI_ATTR_GRADIENT_TYPE				_T("gradient-type")
#define GDI_ATTR_GRADIENT_TYPE_HORZ			_T("horz")
#define GDI_ATTR_GRADIENT_TYPE_VERT			_T("vert")
#define GDI_ATTR_GRADIENT_TYPE_CROSS		_T("cross")
#define GDI_ATTR_GRADIENT_TYPE_OPTIONS		_T("horz~Horz;vert~Vert;cross~Cross;")

#define GDI_ATTR_LAYOUT					_T("layout")
#define GDI_ATTR_LAYOUT_HORZ			_T("horz")
#define GDI_ATTR_LAYOUT_VERT			_T("vert")
#define GDI_ATTR_LAYOUT_CROSS			_T("cross")
#define GDI_ATTR_LAYOUT_OPTIONS			_T("horz~Horz;vert~Vert;cross~Cross;")

#define GDI_ATTR_FONT_FAMILY		_T("font-family")
#define GDI_ATTR_FONT_STYLE			_T("font-style")
#define GDI_ATTR_FONT_SIZE			_T("font-size")
#define GDI_ATTR_FONT_WEIGHT		_T("font-weight")
#define GDI_ATTR_FONT_COLOR			_T("text-color")
#define GDI_ATTR_TEXT_ALIGN			_T("text-align")
#define GDI_ATTR_LINE_ALIGN			_T("line-align")
#define GDI_ATTR_TEXT_WRAP			_T("text-wrap")

#define GDI_ATTR_FONT_FAMILY_SONGTI	_T("SimSun")
#define GDI_ATTR_FONT_FAMILY_HEITI	_T("SimHei")
#define GDI_ATTR_FONT_FAMILY_KAITI	_T("KaiTi")
#define GDI_ATTR_FONT_FAMILY_YAHEI	_T("Microsoft YaHei")
#define GDI_ATTR_FONT_FAMILY_HELV	_T("Helvetica")
#define GDI_ATTR_FONT_FAMILY_ARIA	_T("Arial")

#define GDI_ATTR_FONT_FAMILY_OPTIONS	_T("Helvetica~Helvetica;Microsoft YaHei~Microsoft YaHei;")
#define GDI_ATTR_FONT_SIZE_OPTIONS		_T("8~8px;9~9px;10.5~10.5px;12~12px;14~14px;16~16px;18~18px;21~21px;28~28px;36~36px;42~42px;54~54px;")
#define GDI_ATTR_FONT_WEIGHT_OPTIONS	_T("100~Thin;300~Light;400~Normal;500~Medium;700~Bold;900~Black;")
#define GDI_ATTR_FONT_WEIGHT_NORMAL		_T("400")
#define GDI_ATTR_FONT_WEIGHT_BOLD		_T("700")


#define GDI_ATTR_FONT_SIZE_FOOTER			_T("9")
#define GDI_ATTR_FONT_SIZE_SYSTEM			_T("10.5")
#define GDI_ATTR_FONT_SIZE_TEXT				_T("12")
#define GDI_ATTR_FONT_SIZE_TITLE			_T("14")

#define GDI_ATTR_FONT_STYLE_NORMAL		_T("normal")
#define GDI_ATTR_FONT_STYLE_ITALIC		_T("italic")
#define GDI_ATTR_FONT_STYLE_OBLIQUE		_T("oblique")
#define GDI_ATTR_FONT_STYLE_UNDERLINE	_T("underline")
#define GDI_ATTR_FONT_STYLE_STRIKOUT	_T("strikout")
#define GDI_ATTR_FONT_STYLE_OPTIONS		_T("normal~Normal;italic~Italic;oblique~Oblique;underline~Underline;strikout~Strikout;")

#define GDI_ATTR_TEXT_ALIGN_NEAR		_T("near")
#define GDI_ATTR_TEXT_ALIGN_FAR			_T("far")
#define GDI_ATTR_TEXT_ALIGN_CENTER		_T("center")
#define GDI_ATTR_TEXT_ALIGN_OPTIONS		_T("near~Near;far~Far;center~Center;")

#define GDI_ATTR_TEXT_WRAP_WORDBREAK	_T("word-break")
#define GDI_ATTR_TEXT_WRAP_LINEBREAK	_T("line-break")
#define GDI_ATTR_TEXT_WRAP_OPTIONS		_T("word-break~WordBreak;line-break~LineBreak;")

#define GDI_ATTR_LINE_HEIGHT			_T("line-height")

#define DEF_GDI_TEXT_LINE_HEIGHT		_T("1.25")

#define GDI_ATTR_IMAGE					_T("image")
#define GDI_ATTR_IMAGE_STYLE			_T("image-style")
#define GDI_ATTR_IMAGE_SRC				_T("image-src")
#define GDI_ATTR_IMAGE_LOWCOLOR			_T("low-color")
#define GDI_ATTR_IMAGE_HIGHCOLOR		_T("high-color")
#define GDI_ATTR_IMAGE_HEIGHT			_T("image-height")
#define GDI_ATTR_IMAGE_WIDTH			_T("image-width")

#define GDI_ATTR_IMAGE_STYLE_OPTIONS	_T("center~Center;stretch~Stretch;repeat~Repeat;")
#define GDI_ATTR_IMAGE_STYLE_CENTER		_T("center")
#define GDI_ATTR_IMAGE_STYLE_STRETCH	_T("stretch")
#define GDI_ATTR_IMAGE_STYLE_REPEAT		_T("repeat")

#define	GDI_ATTR_IMAGE_TYPE			_T("image-type")
#define GDI_ATTR_IMAGE_HEAD			_T("data:image/")
#define GDI_ATTR_IMAGE_TYPE_JPG		_T("data:image/jpg;base64,")
#define GDI_ATTR_IMAGE_TYPE_PNG		_T("data:image/png;base64,")
#define GDI_ATTR_IMAGE_TYPE_BMP		_T("data:image/bmp;base64,")
#define GDI_ATTR_IMAGE_TYPE_CODE128		_T("data:image/code128;")
#define GDI_ATTR_IMAGE_TYPE_PDF417		_T("data:image/pdf417;")
#define GDI_ATTR_IMAGE_TYPE_QRCODE		_T("data:image/qrcode;")
#define GDI_ATTR_IMAGE_TYPE_THUMB	_T("data::image/thumb;")
#define GDI_ATTR_IMAGE_TYPE_ICON	_T("data::image/icon;")
#define GDI_ATTR_IMAGE_TYPE_COLOR	_T("data::image/color;")

#define GDI_ATTR_REGION				_T("region")
#define GDI_ATTR_REGION_RECT		_T("rect")
#define GDI_ATTR_REGION_ROUND		_T("round")
#define GDI_ATTR_REGION_ELLIPSE		_T("ellipse")

#define GDI_ATTR_RGB_WHITE			_T("RGB(255,255,255)")
#define GDI_ATTR_RGB_SOFTWHITE		_T("RGB(250,250,250)")
#define GDI_ATTR_RGB_LIGHTWHITE		_T("RGB(252,252,252)")
#define GDI_ATTR_RGB_SNOWWHITE		_T("RGB(248,248,248)")
#define GDI_ATTR_RGB_DARKWHITE		_T("RGB(225,225,225)")
#define GDI_ATTR_RGB_CREAMWHITE		_T("RGB(245,255,250)")
#define GDI_ATTR_RGB_HONEYWHITE		_T("RGB(240,255,240)")
#define GDI_ATTR_RGB_SMOKEWHITE		_T("RGB(245,245,245)")
#define GDI_ATTR_RGB_GAINSWHITE		_T("RGB(220,220,220)")
#define GDI_ATTR_RGB_BEIGEWHITE		_T("RGB(245,245,220)")

#define GDI_ATTR_RGB_LIGHTAZURE		_T("RGB(240,255,255)")
#define GDI_ATTR_RGB_AZURE			_T("RGB(224,238,238)")
#define GDI_ATTR_RGB_SOFTAZURE		_T("RGB(193,205,205)")
#define GDI_ATTR_RGB_DARKAZURE		_T("RGB(131,139,139)")

#define GDI_ATTR_RGB_LIGHTIVORY		_T("RGB(255,255,240)")
#define GDI_ATTR_RGB_IVORY			_T("RGB(238,238,224)")
#define GDI_ATTR_RGB_SOFTIVORY		_T("RGB(238,238,224)")
#define GDI_ATTR_RGB_DARKIVORY		_T("RGB(205,205,193)")

#define GDI_ATTR_RGB_GRAY			_T("RGB(169,169,169)")
#define GDI_ATTR_RGB_SOFTGRAY		_T("RGB(192,192,192)")
#define GDI_ATTR_RGB_LIGHTGRAY		_T("RGB(211,211,211)")
#define GDI_ATTR_RGB_DARKGRAY		_T("RGB(128,128,128)")
#define GDI_ATTR_RGB_SLATEGRAY		_T("RGB(112,128,144)")

#define GDI_ATTR_RGB_LIGHTSLATE		_T("RGB(198,226,255)")
#define GDI_ATTR_RGB_SLATE			_T("RGB(185,211,238)")
#define GDI_ATTR_RGB_SOFTSLATE		_T("RGB(159,182,205)")
#define GDI_ATTR_RGB_DARKSLATE		_T("RGB(96,123,139)")

#define GDI_ATTR_RGB_RED			_T("RGB(255,0,0)")
#define GDI_ATTR_RGB_SOFTRED		_T("RGB(178,34,34)")
#define GDI_ATTR_RGB_LIGHTRED		_T("RGB(255,182,193)")
#define GDI_ATTR_RGB_DARKRED		_T("RGB(128,0,0)")

#define GDI_ATTR_RGB_PURPLE			_T("RGB(128,0,128)")
#define GDI_ATTR_RGB_SOFTPURPLE		_T("RGB(230,230,250)")
#define GDI_ATTR_RGB_LIGHTPURPLE	_T("RGB(190,209,235)")
#define GDI_ATTR_RGB_DARKPURPLE		_T("RGB(139,0,139)")

#define GDI_ATTR_RGB_GREEN			_T("RGB(0,128,0)")
#define GDI_ATTR_RGB_SOFTGREEN		_T("RGB(240,255,240)")
//#define GDI_ATTR_RGB_LIGHTGREEN		_T("RGB(144,238,144)")
#define GDI_ATTR_RGB_LIGHTGREEN		_T("RGB(102,205,170)")
#define GDI_ATTR_RGB_DARKGREEN		_T("RGB(34,139,34)")

#define GDI_ATTR_RGB_BLUE			_T("RGB(0,0,255)")
//#define GDI_ATTR_RGB_LIGHTBLUE		_T("RGB(95,158,160)")
#define GDI_ATTR_RGB_LIGHTBLUE		_T("RGB(176,196,222)")
#define GDI_ATTR_RGB_SOFTBLUE		_T("RGB(70,130,180)")
#define GDI_ATTR_RGB_DARKBLUE		_T("RGB(0,0,139)")

#define GDI_ATTR_RGB_CYAN			_T("RGB(0,255,255)")
#define GDI_ATTR_RGB_SOFTCYAN		_T("RGB(0,128,128)")
#define GDI_ATTR_RGB_LIGHTCYAN		_T("RGB(244,255,255)")
#define GDI_ATTR_RGB_DARKCYAN		_T("RGB(47,79,79)")

#define GDI_ATTR_RGB_YELLOW			_T("RGB(255,255,0)")
//#define GDI_ATTR_RGB_SOFTYELLOW		_T("RGB(255,165,0)")
#define GDI_ATTR_RGB_SOFTYELLOW		_T("RGB(245,222,179)")
#define GDI_ATTR_RGB_LIGHTYELLOW	_T("RGB(255,215,0)")
#define GDI_ATTR_RGB_DARKYELLOW		_T("RGB(160,82,45)")
#define GDI_ATTR_RGB_PALEYELLOW		_T("RGB(238,232,170)")

#define GDI_ATTR_RGB_ORANGE			_T("RGB(250,127,0)")

#define GDI_ATTR_RGB_BLACK			_T("RGB(0,0,0)")
#define GDI_ATTR_RGB_SLATEBLACK		_T("RGB(47,79,79)")
#define GDI_ATTR_RGB_GREYBLACK		_T("RGB(79,79,79)")
#define GDI_ATTR_RGB_SOFTBLACK		_T("RGB(98,98,98)")
#define GDI_ATTR_RGB_LIGHTBLACK		_T("RGB(105,105,105)")
#define GDI_ATTR_RGB_HARDBLACK		_T("RGB(60,60,60)")
#define GDI_ATTR_RGB_DARKBLACK		_T("RGB(36,36,36)")
#define GDI_ATTR_RGB_MONOBLACK		_T("RGB(54,54,54)")

#define GDI_ATTR_RGB_OPTIONS		_T("RGB(255,255,255)~White;RGB(250,250,250)~GhostWhite;RGB(248,248,255)~SliverWhite;RGB(200,200,200)~SliverGray;RGB(169,169,169)~Gray;RGB(192,192,192)~SoftGray;RGB(128,128,128)~DarkGray;RGB(255,0,0)~Red;RGB(178,34,34)~BrickRed;RGB(255,182,193)~LightRed;RGB(128,0,0)~PurpleRed;RGB(128,0,128)~Purple;RGB(230,230,250)~SoftPurple;RGB(190,209,235)~LightPurple;RGB(139,0,139)~DarkPurple;RGB(0,128,0)~Green;RGB(240,255,240)~PaleGreen;RGB(102,205,170)~LightGreen;RGB(34,139,34)~DarkGreen;RGB(0,0,255)~Blue;RGB(70,130,180)~SoftBlue;RGB(176,196,222)~SkyBlue;RGB(0,0,139)~SeaBlue;RGB(0,255,255)~Cyan;RGB(0,128,128)~TealCyan;RGB(47,79,79)~StoneCyan;RGB(244,255,255)~SoftCyan;RGB(255,255,0)~Yellow;RGB(255,215,0)~GoldenYellow;RGB(218,165,32)~SoftYellow;RGB(160,82,45)~DarkYellow;RGB(0,0,0)~Black;RGB(36,36,36)~DarkBlack;RGB(105,105,105)~LightBlack;RGB(98,98,98)~ClassicsBlack;")


#endif /*XDKATTR_H*/

