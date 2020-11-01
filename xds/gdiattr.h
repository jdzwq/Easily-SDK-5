﻿#ifndef _GDIATTR_H
#define _GDIATTR_H

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
#define GDI_ATTR_STOP_COLOR				_T("stop-color")
#define GDI_ATTR_FILL_OPACITY			_T("fill-opacity")

#define GDI_ATTR_FILL_STYLE_SOLID		_T("solid")
#define GDI_ATTR_FILL_STYLE_HATCH		_T("hatch")
#define GDI_ATTR_FILL_STYLE_PATTERN		_T("pattern")
#define GDI_ATTR_FILL_STYLE_GRADIENT	_T("gradient")
#define GDI_ATTR_FILL_STYLE_OPTIONS		_T("solid~Solid;hatch~Hatch;pattern~Pattern;gradient~Gradient;")

#define GDI_ATTR_GRADIENT				_T("gradient")
#define GDI_ATTR_GRADIENT_HORZ			_T("horz")
#define GDI_ATTR_GRADIENT_VERT			_T("vert")
#define GDI_ATTR_GRADIENT_RADIAL		_T("radial")
#define GDI_ATTR_GRADIENT_OPTIONS		_T("horz~Horz;vert~Vert;radial~Radial;")

#define GDI_ATTR_LAYOUT					_T("layout")
#define GDI_ATTR_LAYOUT_HORZ			_T("horz")
#define GDI_ATTR_LAYOUT_VERT			_T("vert")
#define GDI_ATTR_LAYOUT_CROSS			_T("cross")
#define GDI_ATTR_LAYOUT_OPTIONS			_T("horz~Horz;vert~Vert;cross~Cross;")

#define GDI_ATTR_ORIENT					_T("orient")
#define GDI_ATTR_ORIENT_LEFT			_T("left")
#define GDI_ATTR_ORIENT_RIGHT			_T("right")
#define GDI_ATTR_ORIENT_TOP				_T("top")
#define GDI_ATTR_ORIENT_BOTTOM			_T("bottom")
#define GDI_ATTR_ORIENT_OPTIONS			_T("left~Left;right~Right;top~Top;bottom~Bottom;")

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
#define GDI_ATTR_FONT_SIZE_OPTIONS		_T("5~5px;5.5~5.5px;6.5~6.5px;7.5~7.5px;9~9px;10.5~10.5px;12~12px;14~14px;16~16px;18~18px;24~24px;26~26px;36~36px;42~42px;54~54px;63~63px;72~72px;")
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
#define GDI_ATTR_IMAGE_TYPE_URL		_T("url:")

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

#define GDI_ATTR_RGB_LIGHTRED		_T("RGB(255,182,193)")
#define GDI_ATTR_RGB_RED			_T("RGB(255,0,0)")
#define GDI_ATTR_RGB_SOFTRED		_T("RGB(178,34,34)")
#define GDI_ATTR_RGB_DARKRED		_T("RGB(128,0,0)")

#define GDI_ATTR_RGB_LIGHTPURPLE	_T("RGB(190,209,235)")
#define GDI_ATTR_RGB_PURPLE			_T("RGB(128,0,128)")
#define GDI_ATTR_RGB_SOFTPURPLE		_T("RGB(230,230,250)")
#define GDI_ATTR_RGB_DARKPURPLE		_T("RGB(139,0,139)")

#define GDI_ATTR_RGB_LIGHTGREEN		_T("RGB(102,205,170)")
//#define GDI_ATTR_RGB_LIGHTGREEN		_T("RGB(240,255,240)")
#define GDI_ATTR_RGB_GREEN			_T("RGB(0,128,0)")
#define GDI_ATTR_RGB_SOFTGREEN		_T("RGB(144,238,144)")
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

#define GDI_ATTR_GIZMO_ALIGNCENTER	_T("alignCenter")
#define GDI_ATTR_GIZMO_ALIGNFAR		_T("alignFar")
#define GDI_ATTR_GIZMO_ALIGNNEAR		_T("alignNear")
#define GDI_ATTR_GIZMO_APPEND			_T("append")
#define GDI_ATTR_GIZMO_ARRANGECENTER	_T("arrangeCenter")
#define GDI_ATTR_GIZMO_ARRANGELEFT	_T("arrangeLeft")
#define GDI_ATTR_GIZMO_ARRANGERIGHT	_T("arrangeRight")
#define GDI_ATTR_GIZMO_BACKGROUND		_T("background")
#define GDI_ATTR_GIZMO_BEGIN			_T("begin")
#define GDI_ATTR_GIZMO_BOOK			_T("book")
#define GDI_ATTR_GIZMO_BORDER			_T("border")
#define GDI_ATTR_GIZMO_CALENDAR		_T("calendar")
#define GDI_ATTR_GIZMO_CHECK			_T("check")
#define GDI_ATTR_GIZMO_CHECKBOX		_T("checkBox")
#define GDI_ATTR_GIZMO_CHECKED		_T("checked")
#define GDI_ATTR_GIZMO_CLEAR			_T("clear")
#define GDI_ATTR_GIZMO_CLOSE			_T("close")
#define GDI_ATTR_GIZMO_CODE			_T("code")
#define GDI_ATTR_GIZMO_COLLAPSE		_T("collapse")
#define GDI_ATTR_GIZMO_COPY			_T("copy")
#define GDI_ATTR_GIZMO_COUNTER		_T("counter")
#define GDI_ATTR_GIZMO_CUT			_T("cut")
#define GDI_ATTR_GIZMO_DATE			_T("date")
#define GDI_ATTR_GIZMO_DELETE			_T("delete")
#define GDI_ATTR_GIZMO_DENSITY		_T("density")
#define GDI_ATTR_GIZMO_DIAGRAM		_T("diagram")
#define GDI_ATTR_GIZMO_DIALOG			_T("dialog")
#define GDI_ATTR_GIZMO_DOC			_T("doc")
#define GDI_ATTR_GIZMO_DOWN			_T("down")
#define GDI_ATTR_GIZMO_EDIT			_T("edit")
#define GDI_ATTR_GIZMO_END			_T("end")
#define GDI_ATTR_GIZMO_EXECUTE		_T("execute")
#define GDI_ATTR_GIZMO_EXPAND			_T("expand")
#define GDI_ATTR_GIZMO_EXPORT			_T("export")
#define GDI_ATTR_GIZMO_FETCH			_T("fetch")
#define GDI_ATTR_GIZMO_FIND			_T("find")
#define GDI_ATTR_GIZMO_FIRST			_T("first")
#define GDI_ATTR_GIZMO_FIXED			_T("fixed")
#define GDI_ATTR_GIZMO_FRESH			_T("fresh")
#define GDI_ATTR_GIZMO_FOLDER			_T("folder")
#define GDI_ATTR_GIZMO_FONTCOLOR		_T("fontColor")
#define GDI_ATTR_GIZMO_FONTWEIGHT		_T("fontWeight")
#define GDI_ATTR_GIZMO_FONTNAME		_T("fontName")
#define GDI_ATTR_GIZMO_FONTSIZE		_T("fontSize")
#define GDI_ATTR_GIZMO_FONTSTYLE		 _T("fontStyle")
#define GDI_ATTR_GIZMO_FOREGROUND		_T("foreground")
#define GDI_ATTR_GIZMO_GRAPH			_T("graph")
#define GDI_ATTR_GIZMO_GRID			_T("grid")
#define GDI_ATTR_GIZMO_GROUP			_T("group")
#define GDI_ATTR_GIZMO_GUIDER			_T("guider")
#define GDI_ATTR_GIZMO_HELPC			_T("helpc")
#define GDI_ATTR_GIZMO_HELPP			_T("helpp")
#define GDI_ATTR_GIZMO_HERF			_T("herf")
#define GDI_ATTR_GIZMO_HOME			_T("home")
#define GDI_ATTR_GIZMO_IMAGES			_T("images")
#define GDI_ATTR_GIZMO_IMPORT			_T("import")
#define GDI_ATTR_GIZMO_INPUT			_T("input")
#define GDI_ATTR_GIZMO_INSERT			_T("insert")
#define GDI_ATTR_GIZMO_JUDGE			_T("judge")
#define GDI_ATTR_GIZMO_KEYBOX			_T("keyBox")
#define GDI_ATTR_GIZMO_KEYBOXED		_T("keyBoxed")
#define GDI_ATTR_GIZMO_LABEL			_T("label")
#define GDI_ATTR_GIZMO_LAST			_T("last")
#define GDI_ATTR_GIZMO_LIST			_T("list")
#define GDI_ATTR_GIZMO_LOCATION		_T("location")
#define GDI_ATTR_GIZMO_LOCK			_T("lock")
#define GDI_ATTR_GIZMO_LOGO			_T("logo")
#define GDI_ATTR_GIZMO_MAXIMIZE		_T("maximize")
#define GDI_ATTR_GIZMO_MEMO			_T("memo")
#define GDI_ATTR_GIZMO_MINIMIZE		_T("minimize")
#define GDI_ATTR_GIZMO_MINUS			_T("minus")
#define GDI_ATTR_GIZMO_MULTITEXT		_T("multiText")
#define GDI_ATTR_GIZMO_NAVI			_T("navi")
#define GDI_ATTR_GIZMO_NEW			_T("new")
#define GDI_ATTR_GIZMO_NEXT			_T("next")
#define GDI_ATTR_GIZMO_NOTE			_T("note")
#define GDI_ATTR_GIZMO_NUMERIC		_T("numeric")
#define GDI_ATTR_GIZMO_OMIT			_T("omit")
#define GDI_ATTR_GIZMO_OPEN			_T("open")
#define GDI_ATTR_GIZMO_ORDER			_T("order")
#define GDI_ATTR_GIZMO_OUTPUT			_T("output")
#define GDI_ATTR_GIZMO_PANTO			_T("panto")
#define GDI_ATTR_GIZMO_PASTE			_T("paste")
#define GDI_ATTR_GIZMO_PAUSE			_T("pause")
#define GDI_ATTR_GIZMO_PERSON			_T("person")
#define GDI_ATTR_GIZMO_PHOTO			_T("photo")
#define GDI_ATTR_GIZMO_PLUS			_T("plus")
#define GDI_ATTR_GIZMO_PREV			_T("prev")
#define GDI_ATTR_GIZMO_PREVIEW		_T("preview")
#define GDI_ATTR_GIZMO_PRINT			_T("print")
#define GDI_ATTR_GIZMO_PROCESS		_T("process")
#define GDI_ATTR_GIZMO_PROPER			_T("proper")
#define GDI_ATTR_GIZMO_PUSH			_T("push")
#define GDI_ATTR_GIZMO_RADIO			_T("radio")
#define GDI_ATTR_GIZMO_RADIOBOX		_T("radioBox")
#define GDI_ATTR_GIZMO_RADIOED		_T("radioed")
#define GDI_ATTR_GIZMO_REMOVE			_T("remove")
#define GDI_ATTR_GIZMO_REDO			_T("redo")
#define GDI_ATTR_GIZMO_RESTORE		_T("restore")
#define GDI_ATTR_GIZMO_RICH			_T("rich")
#define GDI_ATTR_GIZMO_SAVE			_T("save")
#define GDI_ATTR_GIZMO_SAVEAS			_T("saveas")
#define GDI_ATTR_GIZMO_SCATTER		_T("scatter")
#define GDI_ATTR_GIZMO_SCHEMA			_T("schema")
#define GDI_ATTR_GIZMO_SCREEN			_T("screen")
#define GDI_ATTR_GIZMO_SELECTALL		_T("selectAll")
#define GDI_ATTR_GIZMO_SELECTED		_T("selected")
#define GDI_ATTR_GIZMO_SHAPE			_T("shape")
#define GDI_ATTR_GIZMO_SINGLETEXT		_T("singleText")
#define GDI_ATTR_GIZMO_SIZEHORZ		_T("sizeHorz")
#define GDI_ATTR_GIZMO_SIZEVERT		_T("sizeVert")
#define GDI_ATTR_GIZMO_SLIDE			_T("slide")
#define GDI_ATTR_GIZMO_SPACEHORZ		_T("spaceHorz")
#define GDI_ATTR_GIZMO_SPACEVERT		_T("spaceVert")
#define GDI_ATTR_GIZMO_SPIN			_T("spin")
#define GDI_ATTR_GIZMO_START			_T("start")
#define GDI_ATTR_GIZMO_STATIC			_T("static")
#define GDI_ATTR_GIZMO_STOP			_T("stop")
#define GDI_ATTR_GIZMO_STYLE			_T("style")
#define GDI_ATTR_GIZMO_SUM			_T("sum")
#define GDI_ATTR_GIZMO_TABLE			_T("table")
#define GDI_ATTR_GIZMO_TAG			_T("tag")
#define GDI_ATTR_GIZMO_TIME			_T("time")
#define GDI_ATTR_GIZMO_TOUCH			_T("touch")
#define GDI_ATTR_GIZMO_TOUCHED		_T("touched")
#define GDI_ATTR_GIZMO_TRENDS			_T("trends")
#define GDI_ATTR_GIZMO_UNDO			_T("undo")
#define GDI_ATTR_GIZMO_UNLOCK			_T("unlock")
#define GDI_ATTR_GIZMO_UP				_T("up")
#define GDI_ATTR_GIZMO_UPDATE			_T("update")
#define GDI_ATTR_GIZMO_USER			_T("user")
#define GDI_ATTR_GIZMO_ZOOMIN			_T("zoomIn")
#define GDI_ATTR_GIZMO_ZOOMOUT		_T("zoomOut")

#define GDI_ATTR_GIZMO_RECT			_T("rect")
#define GDI_ATTR_GIZMO_ELLIPSE		_T("ellipse")
#define GDI_ATTR_GIZMO_CROSS			_T("cross")
#define GDI_ATTR_GIZMO_STAR			_T("star")
#define GDI_ATTR_GIZMO_DIAMOND		_T("diamond")

#endif /*XDSATTR_H*/

