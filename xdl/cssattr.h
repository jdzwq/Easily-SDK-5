#ifndef CSSATTR_H
#define CSSATTR_H

/********************************************CSS attributes****************************************************/
//css defination
#define CSS_COLOR_OPTIONS			_T("")
#define CSS_GDI_ATTR_RGB_BLACK		_T("0.0.0")
#define CSS_GDI_ATTR_RGB_WHITE		_T("255.255.255")

#define CSS_FONT					_T("font")

#define CSS_FONT_STYLE				_T("font-style")
#define CSS_FONT_STYLE_OPTIONS		_T("0~normal;1~italic;2~oblique;3~underline;4~strikout;")
#define CSS_FONT_STYLE_NORMAL		_T("normal")
#define CSS_FONT_STYLE_ITALIC		_T("italic")
#define CSS_FONT_STYLE_OBLIQUE		_T("oblique")
#define CSS_FONT_STYLE_UNDERLINE	_T("underline")
#define CSS_FONT_STYLE_STRIKOUT		_T("strikout")

#define CSS_FONT_VARIANT			_T("font-variant")
#define CSS_FONT_VARIANT_OPTIONS	_T("0~normal;1~small-caps;")
#define CSS_FONT_VARIANT_NORMAL		_T("normal")
#define CSS_FONT_VARIANT_SMALLCAPS	_T("small-caps")

#define CSS_FONT_WEIGHT				_T("font-weight")
#define CSS_FONT_WEIGHT_OPTIONS		_T("100~100w;200~200w;300~lighter;400~normal;500~bolder;600~600w;700~bold;800~800w;900~black;")
#define CSS_FONT_WEIGHT_100W		_T("100w")
#define CSS_FONT_WEIGHT_200W		_T("200w")
#define CSS_FONT_WEIGHT_LEIGHTER	_T("300w")
#define CSS_FONT_WEIGHT_NORMAL		_T("400w")
#define CSS_FONT_WEIGHT_BOLDER		_T("500w")
#define CSS_FONT_WEIGHT_600W		_T("600w")
#define CSS_FONT_WEIGHT_BOLD		_T("700w")
#define CSS_FONT_WEIGHT_800W		_T("800w")
#define CSS_FONT_WEIGHT_BLACK		_T("900w")

#define CSS_FONT_SIZE				_T("font-size")
#define CSS_FONT_SIZE_OPTIONS		_T("6px~xx-small;8px~x-small;10px~small;12px~medium;16px~large;36px~x-large;48px~xx-large;72px~larger;")
#define CSS_FONT_SIZE_XXSMALL		_T("6px")
#define CSS_FONT_SIZE_XSMALL		_T("8px")
#define CSS_FONT_SIZE_SMALL			_T("10px")
#define CSS_FONT_SIZE_MEDIUM		_T("12px")
#define CSS_FONT_SIZE_LARGE			_T("16px")
#define CSS_FONT_SIZE_XLARGE		_T("36px")
#define CSS_FONT_SIZE_XXLARGE		_T("48px")
#define CSS_FONT_SIZE_LARGER		_T("72px")

#define CSS_FONT_HEIGHT				_T("line-height")
#define CSS_FONT_HEIGHT_NORMAL		_T("normal")

#define CSS_FONT_FAMILY				_T("font-family")
#define CSS_FONT_FAMILY_OPTIONS		_T("ncursive~ncursive;fantasy~fantasy;monospace~monospace;serif~serif;sans-serif~sans-serif;Times New Roman~Times New Roman;")

#define CSS_TEXT					_T("text")

#define CSS_TEXT_INDENT				_T("text-indent")

#define CSS_OVERFLOW				_T("overflow")
#define CSS_OVERFLOW_OPTIONS		_T("hidden~Hidden;visible~Visible")
#define CSS_OVERFLOW_HIDDEN			_T("hidden")
#define CSS_OVERFLOW_VISIBLE		_T("visible")

#define CSS_TEXT_OVERFLOW			_T("text-overflow")
#define CSS_TEXT_OVERFLOW_OPTIONS	_T("clip~Clip;ellipsis~Ellipsis;")
#define CSS_TEXT_OVERFLOW_CLIP		_T("clip")
#define CSS_TEXT_OVERFLOW_ELLIPSIS	_T("ellipsis")

#define CSS_VERTICAL_ALIGN				_T("vertical-align")
#define CSS_VERTICAL_ALIGN_OPTIONS		_T("0~baseline;1~auto;2~sub;3~super;4~top;5~text-top;6~middle;7~bottom;8~text-bottom;")
#define CSS_VERTICAL_ALIGN_BASELINE		_T("baseline")
#define CSS_VERTICAL_ALIGN_AUTO			_T("auto")
#define CSS_VERTICAL_ALIGN_SUB			_T("sub")
#define CSS_VERTICAL_ALIGN_SUPER		_T("super")
#define CSS_VERTICAL_ALIGN_TOP			_T("top")
#define CSS_VERTICAL_ALIGN_TEXTTOP		_T("text-top")
#define CSS_VERTICAL_ALIGN_MIDDLE		_T("middle")
#define CSS_VERTICAL_ALIGN_BOTTOM		_T("bottom")
#define CSS_VERTICAL_ALIGN_TEXTBOTTOM	_T("text-bottom")

#define CSS_TEXT_ALIGN				_T("text-align")
#define CSS_TEXT_ALIGN_OPTIONS		_T("0~left;1~right;2~center;3~justify;")
#define CSS_TEXT_ALIGN_LEFT			_T("left")
#define CSS_TEXT_ALIGN_RIGHT		_T("right")
#define CSS_TEXT_ALIGN_CENTER		_T("center")
#define CSS_TEXT_ALIGN_JUSTIFY		_T("justify")

#define CSS_TEXT_JUSTIFY				_T("text-justify")
#define CSS_TEXT_JUSTIFY_OPTIONS		_T("0~auto;1~distribute;2~distribute-all-lines;3~distribute-center-last;4~inter-cluster;5~inter-ideograph;6~inter-word;7~kashida;8~newspaper;")
#define CSS_TEXT_JUSTIFY_AUTO			_T("auto")
#define CSS_TEXT_JUSTIFY_DISTRIBUTE		_T("distribute")
#define CSS_TEXT_JUSTTIF_DISTRIBUTEALLLINES		_T("distribute-all-lines")
#define CSS_TEXT_JUSTIFY_DISTRIBUTECENTERLAST	_T("distribute-center-last")
#define CSS_TEXT_JUSTIFY_INTERCLUSTER	_T("inter-clauster")
#define CSS_TEXT_JUSTIFY_INTERIDEOGRAPH	_T("inter-odeograph")
#define CSS_TEXT_JUSTIFY_INTERWORD		_T("inter-word")
#define CSS_TEXT_JUSTIFY_KASHIDA		_T("kashida")
#define CSS_TEXT_JUSTIFY_NEWSPAPER		_T("newspaper")

#define CSS_WORD_BREAK				_T("word-break")
#define CSS_WORD_BREAK_OPTIONS		_T("0~normal;1~break-all;2~keep-all;")
#define CSS_WORD_BREAK_NORMAL		_T("normal")
#define CSS_WORD_BREAK_BREAKALL		_T("break-all")
#define CSS_WORD_BREAK_KEEPALL		_T("keep-all")

#define CSS_WORD_WRAP				_T("word-wrap")
#define CSS_WORD_WRAP_OPTIONS		_T("0~normal;1~break-word;")
#define CSS_WORD_WRAP_NORMAL		_T("normal")
#define CSS_WORD_WRAP_BREAKWORD		_T("brak-word")

#define CSS_WHITE_SPACE				_T("white-space")
#define CSS_WHITE_SPACE_OPTIONS		_T("0~normal;1~pre;2~nowarp;")
#define CSS_WHILTE_SPACE_NORMAL		_T("normal")
#define CSS_WHILTE_SPACE_PRE		_T("pre")
#define CSS_WHILTE_SPACE_NOWRAP		_T("nowrap")

#define CSS_BACKGROUND				_T("background")

#define CSS_BACKGROUND_STYLE		_T("background-style")
#define CSS_BACKGROUND_STYLE_SOLID	_T("solid")
#define CSS_BACKGROUND_STYLE_HATCH	_T("hatch")
#define CSS_BACKGROUND_STYLE_PATTERN	_T("pattern")
#define CSS_BACKGROUND_STYLE_GRADIENT	_T("gradient")
#define CSS_BACKGROUND_STYLE_OPTIONS	_T("solid~solid;hatch~hatch;pattern~pattern;gradient~gradient;")

#define CSS_BACKGROUND_LAYERED			_T("background-layered")
#define CSS_BACKGROUND_LAYERED_HORZ		_T("horz")
#define CSS_BACKGROUND_LAYERED_VERT		_T("vert")
#define CSS_BACKGROUND_LAYERED_CROSS	_T("cross")
#define CSS_BACKGROUND_LAYERED_OPTIONS	_T("horz~horz;vert~vert;cross~cross;")

#define CSS_BACKGROUND_ATTACHMENT			_T("backgrond-attachment")
#define CSS_BACKGROUND_ATTACHMENT_OPTIONS	_T("scroll~scroll;fixed~fixed;")
#define CSS_BACKGROUND_ATTACHMENT_SCROLL	_T("scroll")
#define CSS_BACKGROUND_ATTACHMENT_FIXED		_T("fixed")

#define CSS_BACKGROUND_COLOR				_T("background-color")
#define CSS_BACKGROUND_COLOR_TRANSPARENT	_T("transparent")

#define CSS_BACKGROUND_IMAGE		_T("background-image")
#define CSS_BACKGROUND_IMAGE_NONE	_T("none")
#define CSS_BACKGROUND_IMAGE_URL	_T("url")

#define CSS_BACKGROUND_POSITION		_T("background-position")

#define CSS_BACKGROUND_POSITION_X	_T("background-position-x")
#define CSS_BACKGROUND_POSITION_Y	_T("background-position-y")
#define CSS_BACKGROUND_REPEAT		_T("background-repeat")
#define CSS_BACKGROUND_REPAET_X		_T("repeat-x")
#define CSS_BACKGROUND_REPAET_Y		_T("repeat-y")
#define CSS_BACKGROUND_REPEAT_OPTIONS	_T("repeat~repeat;no-repeat~no-repeat;repeat-x~repeat-x;repeat-y~repeat-y;")

/*style for positioning*/
#define CSS_POSITION				_T("position")
#define CSS_POSITION_OPTIONS		_T("static~static;absolute~absolute;fixed~fixed;relative~relative;")
#define CSS_POSITION_STATIC			_T("static")
#define CSS_POSITION_ABSOLUTE		_T("absolute")
#define CSS_POSITION_FIXED			_T("fixed")
#define CSS_POSITION_RELATIVE		_T("relative")
#define CSS_ZINDEX					_T("z-index")
#define CSS_TOP						_T("top")
#define CSS_BOTTOM					_T("bottom")
#define CSS_LEFT					_T("left")
#define CSS_RIGHT					_T("right")

/*style for dimensions*/
#define CSS_DIMEMSIONS				_T("dimemsions")
#define CSS_DIMEMSIONS_HEIGHT		_T("height")
#define CSS_DIMEMSIONS_WIDTH		_T("width")
#define CSS_DIMEMSIONS_MAXHEIGHT	_T("max-height")
#define CSS_DIMEMSIONS_MINHEIGHT	_T("min-height")
#define CSS_DIMEMSIONS_MAXWIDTH		_T("max-width")
#define CSS_DIMEMSIONS_MINWIDTH		_T("min-width")

/*style for layout*/
#define CSS_LAYOUT					_T("layout")
#define CSS_CLEAR					_T("clear")
#define CSS_CLEAR_OPTIONS			_T("none~none;left~left;right~right;both~both;")
#define CSS_FLOAT					_T("float")
#define CSS_FLOAT_OPTIONS			_T("none~none;left~left;right~right;")
#define CSS_CLIP					_T("clip")
//#define CSS_OVERFLOW				_T("overflow")
//#define CSS_OVERFLOW_OPTIONS		_T("visible~visible;auto~auto;hidden~hidden;scroll~scroll;")
#define CSS_OVERFLOWX				_T("overflow-x")
#define CSS_OVERFLOWX_OPTIONS		_T("visible~visible;auto~auto;hidden~hidden;scroll~scroll;")
#define CSS_OVERFLOWY				_T("overflow-y")
#define CSS_OVERFLOWY_OPTIONS		_T("visible~visible;auto~auto;hidden~hidden;scroll~scroll;")
#define CSS_DISPLAY					_T("display")
#define CSS_DISPLAY_OPTINS			_T("block~block;none~none;inline~inline;inline-block~inline-block;list-item~list-item;table-header-group~table-header-group;table-footer-group~table-footer-group;")
#define CSS_VISIBILITY				_T("visibility")
#define CSS_VISIBILITY_OPTIONS		_T("inherit~inherit;visible~visible;collapse~collapse;hidden~hidden;")
#define CSS_VISIBILITY_INHERIT		_T("inherit")
#define CSS_VISIBILITY_VISIBLE		_T("visible")
#define CSS_VISIBILITY_COLLAPSE		_T("collapse")
#define CSS_VISIBILITY_HIDDEN		_T("hidden")

/*style for margin*/
#define CSS_MARGIN					_T("margin")
#define CSS_MARGIN_TOP				_T("margin-top")
#define CSS_MARGIN_RIGHT			_T("margin-right")
#define CSS_MARGIN_LEFT				_T("margin-left")
#define CSS_MARGIN_BOTTOM			_T("margin-bottom")

#define CSS_LINE_STYLE_DOTTED		_T("dotted")
#define CSS_LINE_STYLE_DASHED		_T("dashed")
#define CSS_LINE_STYLE_SOLID		_T("solid")
#define CSS_LINE_STYLE_DOUBLE		_T("double")
#define CSS_LINE_STYLE_GROOVE		_T("groove")
#define CSS_LINE_STYLE_RIDGE		_T("ridge")
#define CSS_LINE_STYLE_INSET		_T("inset")
#define CSS_LINE_STYLE_OUTSET		_T("outset")
#define CSS_LINE_STYLE_OPTIONS		_T("none~none;dotted~dotted;dashed~dashed;solid~solid;double~double;groove~groove;ridge~ridge;inset~inset;outset~outset;")
#define CSS_LINE_WIDTH_OPTIONS		_T("meduim~meduim;thin~thin;thick~thick;")

/*style for outline*/
#define CSS_OUTLINE					_T("outline")
#define CSS_OUTLINE_COLOR			_T("outline-color")
#define CSS_OUTLINE_STYLE			_T("outline-style")
#define CSS_OUTLINE_WIDTH			_T("outline-width")

/*style for border*/
#define CSS_BORDER					_T("border")
#define CSS_BORDER_COLOR			_T("border-color")
#define CSS_BORDER_STYLE			_T("border-style")
#define CSS_BORDER_WIDTH			_T("border-width")
#define CSS_BORDER_TOP					_T("border-top")
#define CSS_BORDER_TOP_COLOR			_T("border-top-color")
#define CSS_BORDER_TOP_STYLE			_T("border-top-style")
#define CSS_BORDER_TOP_WIDTH			_T("border-top-width")
#define CSS_BORDER_LEFT					_T("border-left")
#define CSS_BORDER_LEFT_COLOR			_T("border-left-color")
#define CSS_BORDER_LEFT_STYLE			_T("border-left-style")
#define CSS_BORDER_LEFT_WIDTH			_T("border-left-width")
#define CSS_BORDER_RIGHT				_T("border-right")
#define CSS_BORDER_RIGHT_COLOR			_T("border-right-color")
#define CSS_BORDER_RIGHT_STYLE			_T("border-right-style")
#define CSS_BORDER_RIGHT_WIDTH			_T("border-right-width")
#define CSS_BORDER_BOTTOM				_T("border-bottom")
#define CSS_BORDER_BOTTOM_COLOR			_T("border-bottom-color")
#define CSS_BORDER_BOTTOM_STYLE			_T("border-bottom-style")
#define CSS_BORDER_BOTTOM_WIDTH			_T("border-bottom-width")

/*style for paddings*/
#define CSS_PADDING					_T("padding")
#define CSS_PADDING_LEFT			_T("padding-left")
#define CSS_PADDING_RIGHT			_T("padding-right")
#define CSS_PADDING_TOP				_T("padding-top")
#define CSS_PADDING_BOTTOM			_T("padding-bottom")

/*style for list*/
#define CSS_LIST_STYLE				_T("list-style")
#define CSS_LIST_STYLE_IMAGE		_T("list-style-image")
#define CSS_LIST_STYLE_POSITION		_T("list-style-position")
#define CSS_LIST_STYLE_TYPE			_T("list-style-type")
#define CSS_LIST_STYLE_TYPE_OPTIONS	_T("disc~disc;circle~circle;square~square;decimal~decimal;lower-roman~lower-roman;upper-roman;lower-alpha~upper-alpha;none~none;")

/*style for table*/
#define CSS_TABLE					_T("table")
#define CSS_BORDER_COLLAPSE			_T("border-collapse")
#define CSS_BORDER_SPACING			_T("border-spacing")
#define CSS_CAPTION_SIDE			_T("caption-side")
#define CSS_EMPTY_CELLS				_T("empty-cells")
#define CSS_EMPTY_CELLS_OPTIONS		_T("show~show;hide~hide;")
#define CSS_TABLE_LAYOUT			_T("table-layout")
#define CSS_TABLEWD_LAYOUT_OPTIONS	_T("auto~auto;fixed~fixed;")

/*style for scrollbar*/
#define CSS_SCROLLBAR					_T("scrollbar")
#define CSS_SCROLLBAR_3DLIGHT_COLOR		_T("scrollbar-3dlight-color")
#define CSS_SCROLLBAR_HIGHLIGHT_COLOR	_T("scrollbar-highlight-color")
#define CSS_SCROLLBAR_FACE_COLOR		_T("scrollbar-face-color")
#define CSS_SCROLLBAR_ARROW_COLOR		_T("scrollbar-arrow-color")
#define CSS_SCROLLBAR_SHADOW_COLOR		_T("scrollbar-shadow-color")
#define CSS_SCROLLBAR_DARKSHADOW_COLOR	_T("scrollbar-darkshadow-color")
#define CSS_SCROLLBAR_BASE_COLOR		_T("scrollbar-base-color")
#define CSS_SCROLLBAR_TRACK_COLOR		_T("scrollbar-track-color")

/*style for cusor*/
#define CSS_CURSOR					_T("cursor")
#define CSS_CURSOR_OPTIONS			_T("auto~auto;all-scroll~all-scroll;col-resize~col-resize;crosshair~crosshair;default~default;hand~hand;move~move;help~help;no-drop~no-drop;not-allowed~not-allowed;pointer~pointer;progress~progress;row-resize~row-resize;text~text;vertical-text~vertical-text;wait~wait;")

#define CSS_BEHAVIOR				_T("behavior")
#define CSS_FILTER					_T("filter")
#define CSS_ZOOM					_T("zoom")

#define CSS_LENGTH_PX				_T("px")
#define CSS_LENGTH_EM				_T("em")
#define CSS_LENGTH_EX				_T("ex")
#define CSS_LENGTH_PT				_T("pt")
#define CSS_LENGTH_PC				_T("pc")
#define CSS_LENGTH_IN				_T("in")
#define CSS_LENGTH_MM				_T("mm")
#define CSS_LENGTH_CM				_T("cm")

#define CSS_COLOR_RGB				_T("rgb")

#define CSS_ANGLE_DEG				_T("deg")
#define CSS_ANGLE_GRAD				_T("grad")
#define CSS_ANGLE_RAD				_T("rad")

#define CSS_TIME_S					_T("s")
#define CSS_TIME_MS					_T("ms")

#define CSS_FREQ_HZ					_T("Hz")
#define CSS_FREQ_KHZ				_T("kHz")


#endif

