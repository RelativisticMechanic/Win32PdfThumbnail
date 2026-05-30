/* 
	The magic happens in this class. It is a CAtlDocumentImpl. Generated
	by Visual Studio's ATL wizard with "Thumbnail" checked.
*/

#pragma once

#include <atlhandlerimpl.h>

// TODO: Fix this. There is a way to get lazy loading
// working with WinRT, but there are issues regarding
// Microsoft's COM architecture doing that with a STA
// (single threaded apartment).

// But it seems like WinRT does not support lazy loading
// PDFs. So this might be needed to be kept.

// Do not render thumbnails of files more than 50 MB for now.
#define MAX_PDF_FILE_SIZE_MB 50

using namespace ATL;

class PdfThumbnailProvider : public CAtlDocumentImpl
{
public:
	PdfThumbnailProvider(void)
	{
	}

	virtual ~PdfThumbnailProvider(void)
	{
	}

	virtual HRESULT LoadFromStream(IStream* pStream, DWORD grfMode);
	virtual void InitializeSearchContent();

	/* Need to override this function. */
	BOOL GetThumbnail(UINT cx, HBITMAP* phbmp, WTS_ALPHATYPE*) override;

protected:
	void SetSearchContent(CString& value);
	virtual void OnDrawThumbnail(HDC hDrawDC, LPRECT lprcBounds);

private:
	CComPtr<IStream> m_stream;
	void DrawErrorThumbnail(LPCWSTR message, HDC hDrawDC, LPRECT lprcBounds);
};
