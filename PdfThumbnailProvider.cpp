// PdfThumbnailProvider.cpp : Implementation of the PdfThumbnailProvider class

#include "pch.h"
#include "framework.h"
#include <propkey.h>
#include "PdfThumbnailProvider.h"

std::vector<BYTE> ReadEntireStream(IStream* pStream)
{
    std::vector<BYTE> data;
    if (!pStream) return data;

    LARGE_INTEGER liZero = { 0 };
    pStream->Seek(liZero, STREAM_SEEK_SET, NULL);

    BYTE buffer[4096];
    ULONG bytesRead = 0;
    while (SUCCEEDED(pStream->Read(buffer, sizeof(buffer), &bytesRead)) && bytesRead > 0)
    {
        data.insert(data.end(), buffer, buffer + bytesRead);
    }
    return data;
}

HRESULT PdfThumbnailProvider::LoadFromStream(IStream* pStream, DWORD grfMode)
{
    UNREFERENCED_PARAMETER(grfMode);
    m_stream = pStream;
    return S_OK;
}

void PdfThumbnailProvider::InitializeSearchContent()
{
	CString value = _T("test;content;");
	SetSearchContent(value);
}


void PdfThumbnailProvider::SetSearchContent(CString& value)
{
	// Assigns search content to PKEY_Search_Contents key
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

/* 
    Reference:
    https://web.archive.org/web/20250112211848/https://www.privyetmir.co.uk/blog/fixing-the-atl-ithumbnailprovider-implementation.html

    By default GetThumbnail creates a 1-bit monochrome HDC. This has to be made 32-bit color manually.
    We do this by overriding the GetThumbnail function with our own.
*/
BOOL PdfThumbnailProvider::GetThumbnail(UINT cx, HBITMAP* phbmp, WTS_ALPHATYPE*) {
    HDC hdc = ::GetDC(NULL);
    HDC hDrawDC = CreateCompatibleDC(hdc);
    if (!hDrawDC) { ReleaseDC(NULL, hdc); return FALSE; }

    // Prepare a 32-bit BITMAPINFO
    BITMAPINFO bi = { 0 };
    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth = cx;
    bi.bmiHeader.biHeight = cx;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;

    // Create a 32-bit DIB section
    void* bits = nullptr;
    HBITMAP hBmp = CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, &bits, NULL, 0);
    if (hBmp) 
    {
        HBITMAP hOld = (HBITMAP)SelectObject(hDrawDC, hBmp);
        // Draw into the 32bpp DC
        RECT rct{ 0,0,(LONG)cx,(LONG)cx };
        OnDrawThumbnail(hDrawDC, &rct);
        SelectObject(hDrawDC, hOld);
        *phbmp = hBmp; // return the 32bpp DIB
        ReleaseDC(NULL, hdc);
        DeleteDC(hDrawDC);
        return TRUE;
    }
    DeleteDC(hDrawDC);
    ReleaseDC(NULL, hdc);
    return FALSE;
}

std::unique_ptr<PDFThumbnailProviderThumbnail> PdfThumbnailProvider::GenerateThumbnailWinRT(LPRECT lprcBounds) {
	auto result = std::make_unique<PDFThumbnailProviderThumbnail>();

    std::vector<BYTE> pdf_data = ReadEntireStream(m_stream);
    if (pdf_data.empty())
    {
        return nullptr;
    }

    winrt::Windows::Storage::Streams::DataWriter writer;
    winrt::Windows::Storage::Streams::InMemoryRandomAccessStream winrt_stream;
    writer.WriteBytes(pdf_data);
    winrt::Windows::Storage::Streams::IBuffer buffer = writer.DetachBuffer();
    winrt_stream.WriteAsync(buffer).get();
    winrt_stream.Seek(0);

    winrt::Windows::Data::Pdf::PdfDocument doc = winrt::Windows::Data::Pdf::PdfDocument::LoadFromStreamAsync(winrt_stream).get();
    if (doc.PageCount() == 0)
    {
        return nullptr;
    }

    winrt::Windows::Data::Pdf::PdfPage page = doc.GetPage(0);

    int target_width = lprcBounds->right - lprcBounds->left;
    int target_height = lprcBounds->bottom - lprcBounds->top;

    auto page_size = page.Size();
    float scale = min((float)target_width / page_size.Width, (float)target_height / page_size.Height);

    int render_width = static_cast<int>(page_size.Width * scale);
    int render_height = static_cast<int>(page_size.Height * scale);

    winrt::Windows::Data::Pdf::PdfPageRenderOptions options;
    options.DestinationWidth(static_cast<uint32_t>(render_width));
    options.DestinationHeight(static_cast<uint32_t>(render_height));

    /* Blank background color */
    options.BackgroundColor(
        winrt::Windows::UI::Color{
            255, 255, 255, 255
        });

    winrt::Windows::Storage::Streams::InMemoryRandomAccessStream render_output_stream;
    page.RenderToStreamAsync(render_output_stream, options).get();
    render_output_stream.Seek(0);

    /*
        WinRT PDF renders a bitmap, into raw BGRA8 pixels.
    */
    auto decoder = winrt::Windows::Graphics::Imaging::BitmapDecoder::CreateAsync(render_output_stream).get();
    auto pixel_data = decoder.GetPixelDataAsync(
        winrt::Windows::Graphics::Imaging::BitmapPixelFormat::Bgra8,
        winrt::Windows::Graphics::Imaging::BitmapAlphaMode::Ignore,
        winrt::Windows::Graphics::Imaging::BitmapTransform(),
        winrt::Windows::Graphics::Imaging::ExifOrientationMode::IgnoreExifOrientation,
        winrt::Windows::Graphics::Imaging::ColorManagementMode::ColorManageToSRgb
    ).get();

    auto pixels = pixel_data.DetachPixelData();
    
    result->pixels = std::make_unique<uint8_t[]>(pixels.size());
    memcpy(result->pixels.get(), pixels.data(), pixels.size());

    result->render_width = render_width;
    result->render_height = render_height;
	result->offset_x = (target_width - render_width) / 2;
	result->offset_y = (target_height - render_height) / 2;

    return result;
}

void PdfThumbnailProvider::OnDrawThumbnail(HDC hDrawDC, LPRECT lprcBounds)
{
    if (!m_stream)
    {
        DrawErrorThumbnail(L"NO STREAM", hDrawDC, lprcBounds);
        return;
    }

    STATSTG stat = {};
    
    if (SUCCEEDED(m_stream->Stat(&stat, STATFLAG_NONAME))) 
    {
        if (stat.cbSize.QuadPart > MAX_PDF_FILE_SIZE_MB * 1024 * 1024) 
        {
            DrawErrorThumbnail(L"PDF TOO LARGE", hDrawDC, lprcBounds);
            return;
        }
    }
    else {
        DrawErrorThumbnail(L"CANNOT STAT", hDrawDC, lprcBounds);
        return;
    }
    try
    {
		std::unique_ptr<PDFThumbnailProviderThumbnail> thumbnail = GenerateThumbnailWinRT(lprcBounds);

		if (thumbnail == nullptr) {
			DrawErrorThumbnail(L"???", hDrawDC, lprcBounds);
			return;
		}

        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biSizeImage = thumbnail->render_width * thumbnail->render_height * 4;
        bmi.bmiHeader.biWidth = thumbnail->render_width;
        bmi.bmiHeader.biHeight = -thumbnail->render_height;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        SetStretchBltMode(hDrawDC, COLORONCOLOR);

        StretchDIBits(
            hDrawDC,
            lprcBounds->left + thumbnail->offset_x,
            lprcBounds->top + thumbnail->offset_y,
            thumbnail->render_width,
            thumbnail->render_height,
            0,
            0,
            thumbnail->render_width,
            thumbnail->render_height,
            thumbnail->pixels.get(),
            &bmi,
            DIB_RGB_COLORS,
            SRCCOPY);
    }
    catch (...) {
        DrawErrorThumbnail(L"DOC FAIL", hDrawDC, lprcBounds);
        return;
    }
}

/* 
    Helper function to draw an error thumbnail.
*/
void PdfThumbnailProvider::DrawErrorThumbnail(LPCWSTR message, HDC hDrawDC, LPRECT lprcBounds)
{
    HBRUSH hBrush = CreateSolidBrush(RGB(245, 245, 245));
    FillRect(hDrawDC, lprcBounds, hBrush);
    DeleteObject(hBrush);

    SetBkMode(hDrawDC, TRANSPARENT);
    SetTextColor(hDrawDC, RGB(120, 120, 120));

    HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
    HFONT hOldFont = (HFONT)SelectObject(hDrawDC, hFont);

    DrawText(
        hDrawDC,
        message,
        -1,
        lprcBounds,
        DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

    SelectObject(hDrawDC, hOldFont);
}