#ifndef _HTTPATTR_H
#define _HTTPATTR_H

#define HTTP_HEADER_AUTHORIZATION			_T("Authorization")
#define HTTP_HEADER_AUTHORIZATION_BASIC			_T("Basic")
#define HTTP_HEADER_AUTHORIZATION_XDS			_T("XDS")
#define HTTP_HEADER_AUTHORIZATION_OSS			_T("OSS")
#define HTTP_HEADER_ACCEPT					_T("Accept")
#define HTTP_HEADER_ACCEPTENCODING			_T("Accept-Encoding")
#define HTTP_HEADER_ACCEPTCHARSET			_T("Accept-Charset")
#define HTTP_HEADER_ACCEPTRANGES			_T("Accept-Ranges")
#define HTTP_HEADER_ACCEPTRANGES_BYTES		_T("bytes")
#define HTTP_HEADER_ACCEPTRANGES_NONE		_T("none")
#define HTTP_HEADER_EXPECT					_T("Expect")
#define HTTP_HEADER_EXPECT_CONTINUE			_T("100-continue")
#define HTTP_HEADER_CONTENTENCODING			_T("Content-Encoding")
#define HTTP_HEADER_CONTENTENCODING_GZIP		_T("gzip")
#define HTTP_HEADER_CONTENTENCODING_DEFLATE		_T("deflate")
#define HTTP_HEADER_CONTENTLENGTH			_T("Content-Length")
#define HTTP_HEADER_CONTENTTYPE				_T("Content-Type")

#define HTTP_HEADER_CONTENTTYPE_IMAGEJPG			_T("image/jpeg")
#define HTTP_HEADER_CONTENTTYPE_IMAGEPNG			_T("image/png")
#define HTTP_HEADER_CONTENTTYPE_IMAGEGIF			_T("image/gif")
#define HTTP_HEADER_CONTENTTYPE_IMAGETIFF			_T("image/tiff")
#define HTTP_HEADER_CONTENTTYPE_IMAGEBMP			_T("image/bmp")
#define HTTP_HEADER_CONTENTTYPE_IMAGESVG			_T("image/svg+xml")
#define HTTP_HEADER_CONTENTTYPE_VIDEOMPG			_T("video/mpeg")
#define HTTP_HEADER_CONTENTTYPE_VIDEOAVI			_T("video/avi")
#define HTTP_HEADER_CONTENTTYPE_AUDIOMP3			_T("audio/mp3")
#define HTTP_HEADER_CONTENTTYPE_APPPDF				_T("application/pdf")
#define HTTP_HEADER_CONTENTTYPE_APPXML				_T("application/xml")
#define HTTP_HEADER_CONTENTTYPE_APPSOAP				_T("application/soap+xml")
#define HTTP_HEADER_CONTENTTYPE_APPJSON				_T("application/json")
#define HTTP_HEADER_CONTENTTYPE_APPJSON_GB2312		_T("application/json; charset=gb2312;")
#define HTTP_HEADER_CONTENTTYPE_APPJSON_UTF8		_T("application/json; charset=utf-8;")
#define HTTP_HEADER_CONTENTTYPE_APPPDF				_T("application/pdf")
#define HTTP_HEADER_CONTENTTYPE_JAVASCRIPT			_T("text/javascript")
#define HTTP_HEADER_CONTENTTYPE_TEXTCSS				_T("text/css")
#define HTTP_HEADER_CONTENTTYPE_TEXTXML				_T("text/xml")
#define HTTP_HEADER_CONTENTTYPE_TEXTXML_GB2312		_T("text/xml; charset=gb2312;")
#define HTTP_HEADER_CONTENTTYPE_TEXTXML_UTF8		_T("text/xml; charset=utf-8;")
#define HTTP_HEADER_CONTENTTYPE_TEXTHTML			_T("text/html")
#define HTTP_HEADER_CONTENTTYPE_TEXTPLAIN			_T("text/plain")
#define HTTP_HEADER_CONTENTTYPE_TEXTPLAIN_GB2312	_T("text/plain; charset=gb2312;")
#define HTTP_HEADER_CONTENTTYPE_TEXTPLAIN_UTF8		_T("text/plain; charset=utf-8;")

#define HTTP_HEADER_CONTENTTYPE_ENTITY_CHARSET		_T("charset")
#define HTTP_HEADER_CONTENTRANGE			_T("Content-Range")
#define HTTP_HEADER_RANGE					_T("Range")
#define HTTP_HEADER_TRANSFERENCODING		_T("Transfer-Encoding")
#define HTTP_HEADER_TRANSFERENCODING_CHUNKED	_T("chunked")
#define HTTP_HEADER_CONTENTMD5				_T("Content-MD5")
#define HTTP_HEADER_DATE					_T("Date")
#define HTTP_HEADER_HOST					_T("Host")
#define HTTP_HEADER_SERVER					_T("Server")
#define HTTP_HEADER_SERVER_XSERVICE				_T("xService")
#define HTTP_HEADER_ETAG					_T("ETag")
#define HTTP_HEADER_LASTMODIFIED			_T("Last-Modified")
#define HTTP_HEADER_IFMODIFIEDSINCE			_T("If-Modified-Since")
#define HTTP_HEADER_IFMATCH					_T("If-Match")
#define HTTP_HEADER_IFNONEMATCH				_T("If-None-Match")
#define HTTP_HEADER_CONNECTION				_T("Connection")
#define HTTP_HEADER_CONNECTION_CLOSE			_T("Close")
#define HTTP_HEADER_CONNECTION_KEEPALIVE		_T("Keep-Alive")
#define HTTP_HEADER_SOAPACTION				_T("SOAPAction")
#define HTTP_HEADER_LOCATION				_T("Location")
#define HTTP_HEADER_CACHECONTROL			_T("Cache-Control")
#define HTTP_HEADER_CACHECONTROL_NOSTORE		_T("no-store")
#define HTTP_HEADER_COOKIE					_T("Cookie")
#define HTTP_HEADER_TRACE					_T("Trace")

//#define HTTP_HEADER_XDSACTION				_T("xds-action")


#define HTTP_VERSION			_T("1.1")

#define HTTP_CODE_100			_T("100")
#define HTTP_CODE_100_TEXT		_T("Continue")

#define HTTP_CODE_101			_T("101")
#define HTTP_CODE_101_TEXT		_T("Switching Protocols")

#define HTTP_CODE_200			_T("200")
#define HTTP_CODE_200_TEXT		_T("OK")

#define HTTP_CODE_201			_T("201")
#define HTTP_CODE_201_TEXT		_T("Created")

#define HTTP_CODE_202			_T("202")
#define HTTP_CODE_202_TEXT		_T("Accepted")

#define HTTP_CODE_300			_T("300")
#define HTTP_CODE_300_TEXT		_T("Multiple Choices")

#define HTTP_CODE_301			_T("301")
#define HTTP_CODE_301_TEXT		_T("Moved Permanently")

#define HTTP_CODE_302			_T("302")
#define HTTP_CODE_302_TEXT		_T("Moved Temporarily")

#define HTTP_CODE_303			_T("303")
#define HTTP_CODE_303_TEXT		_T("See Other")

#define HTTP_CODE_304			_T("304")
#define HTTP_CODE_304_TEXT		_T("Not Modified")

#define HTTP_CODE_305			_T("305")
#define HTTP_CODE_305_TEXT		_T("Use Proxy")

#define HTTP_CODE_400			_T("400")
#define HTTP_CODE_400_TEXT		_T("Bad Request")

#define HTTP_CODE_401			_T("401")
#define HTTP_CODE_401_TEXT		_T("Unauthorized")

#define HTTP_CODE_402			_T("402")
#define HTTP_CODE_402_TEXT		_T("Payment Required")

#define HTTP_CODE_403			_T("403")
#define HTTP_CODE_403_TEXT		_T("Forbidden")

#define HTTP_CODE_404			_T("404")
#define HTTP_CODE_404_TEXT		_T("Not Found")

#define HTTP_CODE_405			_T("405")
#define HTTP_CODE_405_TEXT		_T("Method Not Allowed")

#define HTTP_CODE_406			_T("406")
#define HTTP_CODE_406_TEXT		_T("Not Acceptable")

#define HTTP_CODE_407			_T("407")
#define HTTP_CODE_407_TEXT		_T("Proxy Authentication")

#define HTTP_CODE_408			_T("408")
#define HTTP_CODE_408_TEXT		_T("Request Timed-Out")

#define HTTP_CODE_409			_T("409")
#define HTTP_CODE_409_TEXT		_T("Conflict")

#define HTTP_CODE_410			_T("410")
#define HTTP_CODE_410_TEXT		_T("Gone")

#define HTTP_CODE_411			_T("411")
#define HTTP_CODE_411_TEXT		_T("Length Required")

#define HTTP_CODE_412			_T("412")
#define HTTP_CODE_412_TEXT		_T("Precondition Failed")

#define HTTP_CODE_413			_T("413")
#define HTTP_CODE_413_TEXT		_T("Request Entity Too Large")

#define HTTP_CODE_414			_T("414")
#define HTTP_CODE_414_TEXT		_T("Request-URI Too Large")

#define HTTP_CODE_415			_T("415")
#define HTTP_CODE_415_TEXT		_T("Unsupported Media Type")

#define HTTP_CODE_417			_T("417")
#define HTTP_CODE_417_TEXT		_T("Expectation Failed")

#define HTTP_CODE_500			_T("500")
#define HTTP_CODE_500_TEXT		_T("Internal Server Error")

#define HTTP_CODE_501			_T("501")
#define HTTP_CODE_501_TEXT		_T("Not Implemented")

#define HTTP_CODE_502			_T("502")
#define HTTP_CODE_502_TEXT		_T("Bad Gateway")

#define HTTP_CODE_503			_T("503")
#define HTTP_CODE_503_TEXT		_T("Server Unavailable")

#define HTTP_CODE_504			_T("504")
#define HTTP_CODE_504_TEXT		_T("Gateway Timed-Out")

#define HTTP_CODE_505			_T("505")
#define HTTP_CODE_505_TEXT		_T("HTTP Version not support")

#define HTTP_FAULT				_T("fault")
#define HTTP_FAULT_CODE			_T("fault-code")
#define HTTP_FAULT_STRING		_T("fault-string")

#define HTTP_METHOD_GET			_T("GET")
#define HTTP_METHOD_PUT			_T("PUT")
#define HTTP_METHOD_POST		_T("POST")
#define HTTP_METHOD_HEAD		_T("HEAD")
#define HTTP_METHOD_DELETE		_T("DELETE")
#define HTTP_METHOD_LIST		_T("LIST")
#define HTTP_METHOD_GRANT		_T("GRANT")
	


#endif /*HTTPATTR_H*/