// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#include "framework.h"

// WinRT headers
#include <unknwn.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Data.Pdf.h>
#include <winrt/Windows.Graphics.Imaging.h>

// ATL Headers
#include <atlbase.h>
#include <atlcom.h>
#include <shlwapi.h>

#include <propkey.h>
#include <propsys.h>

// For streams
#include <shcore.h>

#pragma comment(lib, "runtimeobject.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "propsys.lib")
#pragma comment(lib, "shcore.lib")

#endif //PCH_H
