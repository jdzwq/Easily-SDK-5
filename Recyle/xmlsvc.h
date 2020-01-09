// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 XMLSVC_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// XMLSVC_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef __cplusplus
extern "C" {
#endif

#define ERR_SUCCEED		L"0"
#define	ERR_NETWORK		L"-1"
#define ERR_DATA		L"-2"

#define CFG_XMLSVC_NODE_SERVICE		L"service"
#define CFG_XMLSVC_ATTR_INTERFACE	L"interface"
#define CFG_XMLSVC_ATTR_PATH		L"path"
#define CFG_XMLSVC_ATTR_TYPE		L"type"
#define CFG_XMLSVC_ATTR_MODULE		L"module"

extern DWORD WINAPI HttpExtensionProc(LPEXTENSION_CONTROL_BLOCK lpECB);
extern BOOL WINAPI GetExtensionVersion(HSE_VERSION_INFO* pVer);
extern BOOL WINAPI TerminateExtension( DWORD dwFlags);

typedef int (WINAPI *SvcProcPtr)(LINKPTR svc);

#ifdef __cplusplus
}
#endif