# Win32PdfThumbnail
An ATL-based PDF Thumbnail provider for the Windows Explorer.

It always annoyed me how I needed to install Adobe PDF Viewer or Sumatra PDF just to get thumbnail views for PDF files in the Windows explorer. So I rolled out my own. It has no dependencies except the standard Windows libraries, but that does introduce a limitation.

## LIMITATIONS

WinRT does not seem to support PDF lazy loading (or I've not been able to figure out), so this thumbnail viewer has a hard limit of 50 MB. For any files above 50 MB, a thumbnail with "PDF TOO LARGE" will be shown. 

## INSTALLATION

In an elevated command prompt / Powershell window type:

```
regsvr32.exe .\Win32PdfThumbnail.dll
```

That should register the DLL with Windows.

