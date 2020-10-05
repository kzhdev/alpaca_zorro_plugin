#include <windows.h>

#define CLSCTX_INPROC_SERVER	0x1
#define CLSCTX_INPROC_HANDLER	0x2
#define CLSCTX_LOCAL_SERVER		0x4
#define CLSCTX_INPROC_SERVER16	0x8
#define CLSCTX_REMOTE_SERVER	0x10
#define CLSCTX_INPROC_HANDLER16	0x20
#define CLSCTX_INPROC_SERVERX86	0x40
#define CLSCTX_INPROC_HANDLERX86	0x80
#define CLSCTX_ESERVER_HANDLER	0
#define CLSCTX_INPROC           (CLSCTX_INPROC_SERVER|CLSCTX_INPROC_HANDLER)
#define CLSCTX_ALL              (CLSCTX_INPROC_SERVER|CLSCTX_INPROC_HANDLER|CLSCTX_LOCAL_SERVER|CLSCTX_REMOTE_SERVER)
#define CLSCTX_SERVER           (CLSCTX_INPROC_SERVER|CLSCTX_LOCAL_SERVER|CLSCTX_REMOTE_SERVER)
 
HRESULT	WINAPI CoInitialize(long);
void	WINAPI CoUninitialize();
HRESULT	WINAPI CoCreateInstance(GUID* rclsid,void* pUnkOuter,DWORD dwClsContext,IID* riid,void** ppv);
long __stdcall StringFromCLSID(long rclsid,long lplpsz);
long __stdcall CLSIDFromString(long lpsz, long pclsid);
long __stdcall StringFromIID(long rclsid, long lplpsz);
long __stdcall IIDFromString(long lpsz, long lpiid);

void IIDFromStr(char *s,IID *piid)
{
	WORD buf[100];WORD *p=buf;
	while (*s){*p=*s;s++;p++;}*p=0;
	IIDFromString(buf,piid);
}
void StrFromIID(IID *piid,char *s)
{
	WORD *p=NULL;
	StringFromIID(piid,&p);	
	char *ps=s;
	if (p){while(*p){*ps=*p;p++;ps++;}*ps=0;}	
}