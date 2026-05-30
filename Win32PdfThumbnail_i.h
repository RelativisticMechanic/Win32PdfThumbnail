

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 08:44:07 2038
 */
/* Compiler settings for Win32PdfThumbnail.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0628 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __Win32PdfThumbnail_i_h__
#define __Win32PdfThumbnail_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if defined(_CONTROL_FLOW_GUARD_XFG)
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */ 

#ifndef __IThumbnail_FWD_DEFINED__
#define __IThumbnail_FWD_DEFINED__
typedef interface IThumbnail IThumbnail;

#endif 	/* __IThumbnail_FWD_DEFINED__ */


#ifndef __Thumbnail_FWD_DEFINED__
#define __Thumbnail_FWD_DEFINED__

#ifdef __cplusplus
typedef class Thumbnail Thumbnail;
#else
typedef struct Thumbnail Thumbnail;
#endif /* __cplusplus */

#endif 	/* __Thumbnail_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "thumbcache.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IThumbnail_INTERFACE_DEFINED__
#define __IThumbnail_INTERFACE_DEFINED__

/* interface IThumbnail */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IThumbnail;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("d79d95e3-6e58-4294-8df2-076fa13c3f4e")
    IThumbnail : public IUnknown
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct IThumbnailVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IThumbnail * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IThumbnail * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IThumbnail * This);
        
        END_INTERFACE
    } IThumbnailVtbl;

    interface IThumbnail
    {
        CONST_VTBL struct IThumbnailVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IThumbnail_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IThumbnail_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IThumbnail_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IThumbnail_INTERFACE_DEFINED__ */



#ifndef __Win32PdfThumbnailLib_LIBRARY_DEFINED__
#define __Win32PdfThumbnailLib_LIBRARY_DEFINED__

/* library Win32PdfThumbnailLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_Win32PdfThumbnailLib;

EXTERN_C const CLSID CLSID_Thumbnail;

#ifdef __cplusplus

class DECLSPEC_UUID("2b2a023a-228b-42a7-93f9-00ab61d6cdf5")
Thumbnail;
#endif
#endif /* __Win32PdfThumbnailLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


