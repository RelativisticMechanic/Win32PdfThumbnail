// dllmain.h : Declaration of module class.

class CWin32PdfThumbnailModule : public ATL::CAtlDllModuleT< CWin32PdfThumbnailModule >
{
public :
	DECLARE_LIBID(LIBID_Win32PdfThumbnailLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_WIN32PDFTHUMBNAIL, "{909f7f4f-4e0f-4c70-9fa1-47cf3a814d66}")
};

extern class CWin32PdfThumbnailModule _AtlModule;
