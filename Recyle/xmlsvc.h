// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� XMLSVC_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// XMLSVC_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
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