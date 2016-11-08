// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "include\mhook\mhook.h"
#include "Xpsobjectmodel.h"
#include "XpsObjectModel_1.h"
#include "Documenttarget.h"
#include "XpsPrint.h"
#include "string"
#include "Shlwapi.h"
#include <winternl.h>
#include <documenttarget.h>

typedef NTSTATUS (WINAPI *PNT_LdrLoadDll)
(
	PWCHAR, PULONG, PUNICODE_STRING, PHANDLE
);

typedef NTSTATUS(WINAPI *PNT_LdrUnloadDll)
(
	HANDLE
);

typedef HRESULT (WINAPI *PNT_DllGetClassObject)
(
	_In_  REFCLSID rclsid,
	_In_  REFIID   riid,
	_Out_ LPVOID   *ppv
);


PNT_LdrLoadDll OriginalLdrLoadDll = nullptr;
PNT_LdrUnloadDll OriginalLdrUnloadDll = nullptr;
PNT_DllGetClassObject OriginalDllGetClassObject = nullptr;

class XpsOMObjectFactoryProxy : public IXpsOMObjectFactory
{
public:
	XpsOMObjectFactoryProxy() : p_factory(nullptr)
	{
	}

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		if (p_factory)
		{
			auto res = p_factory->QueryInterface(riid, ppvObject);
			return res;
		}

		return S_FALSE;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef(void)
	{
		if (p_factory)
		{
			auto res = p_factory->AddRef();
			return res;
		}

		return S_FALSE;
	}

	virtual ULONG STDMETHODCALLTYPE Release(void)
	{
		if (p_factory)
		{
			auto res = p_factory->Release();
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreatePackage(
		/* [retval][out] */ IXpsOMPackage **package)
	{
		if (p_factory)
		{
			auto res = p_factory->CreatePackage(package);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreatePackageFromFile(
		/* [string][in] */ LPCWSTR filename,
		/* [in] */ BOOL reuseObjects,
		/* [retval][out] */ IXpsOMPackage **package)
	{
		if (p_factory)
		{
			auto res = p_factory->CreatePackageFromFile(filename, reuseObjects, package);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreatePackageFromStream(
		/* [in] */ IStream *stream,
		/* [in] */ BOOL reuseObjects,
		/* [retval][out] */ IXpsOMPackage **package)
	{
		if (p_factory)
		{
			auto res = p_factory->CreatePackageFromStream(stream, reuseObjects, package);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateStoryFragmentsResource(
		/* [in] */ IStream *acquiredStream,
		/* [in] */ IOpcPartUri *partUri,
		/* [retval][out] */ IXpsOMStoryFragmentsResource **storyFragmentsResource)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateStoryFragmentsResource(acquiredStream, partUri, storyFragmentsResource);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateDocumentStructureResource(
		/* [in] */ IStream *acquiredStream,
		/* [in] */ IOpcPartUri *partUri,
		/* [retval][out] */ IXpsOMDocumentStructureResource **documentStructureResource)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateDocumentStructureResource(acquiredStream, partUri, documentStructureResource);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateSignatureBlockResource(
		/* [in] */ IStream *acquiredStream,
		/* [in] */ IOpcPartUri *partUri,
		/* [retval][out] */ IXpsOMSignatureBlockResource **signatureBlockResource)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateSignatureBlockResource(acquiredStream, partUri, signatureBlockResource);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateRemoteDictionaryResource(
		/* [in] */ IXpsOMDictionary *dictionary,
		/* [in] */ IOpcPartUri *partUri,
		/* [retval][out] */ IXpsOMRemoteDictionaryResource **remoteDictionaryResource)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateRemoteDictionaryResource(dictionary, partUri, remoteDictionaryResource);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateRemoteDictionaryResourceFromStream(
		/* [in] */ IStream *dictionaryMarkupStream,
		/* [in] */ IOpcPartUri *dictionaryPartUri,
		/* [in] */ IXpsOMPartResources *resources,
		/* [retval][out] */ IXpsOMRemoteDictionaryResource **dictionaryResource)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateRemoteDictionaryResourceFromStream(dictionaryMarkupStream, dictionaryPartUri, resources, dictionaryResource);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreatePartResources(
		/* [retval][out] */ IXpsOMPartResources **partResources)
	{
		if (p_factory)
		{
			auto res = p_factory->CreatePartResources(partResources);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateDocumentSequence(
		/* [in] */ IOpcPartUri *partUri,
		/* [retval][out] */ IXpsOMDocumentSequence **documentSequence)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateDocumentSequence(partUri, documentSequence);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateDocument(
		/* [in] */ IOpcPartUri *partUri,
		/* [retval][out] */ IXpsOMDocument **document)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateDocument(partUri, document);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreatePageReference(
		/* [in] */ const XPS_SIZE *advisoryPageDimensions,
		/* [retval][out] */ IXpsOMPageReference **pageReference)
	{
		if (p_factory)
		{
			auto res = p_factory->CreatePageReference(advisoryPageDimensions, pageReference);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreatePage(
		/* [in] */ const XPS_SIZE *pageDimensions,
		/* [string][in] */ LPCWSTR language,
		/* [in] */ IOpcPartUri *partUri,
		/* [retval][out] */ IXpsOMPage **page)
	{
		if (p_factory)
		{
			auto res = p_factory->CreatePage(pageDimensions, language, partUri, page);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreatePageFromStream(
		/* [in] */ IStream *pageMarkupStream,
		/* [in] */ IOpcPartUri *partUri,
		/* [in] */ IXpsOMPartResources *resources,
		/* [in] */ BOOL reuseObjects,
		/* [retval][out] */ IXpsOMPage **page)
	{
		if (p_factory)
		{
			auto res = p_factory->CreatePageFromStream(pageMarkupStream, partUri, resources, reuseObjects, page);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateCanvas(
		/* [retval][out] */ IXpsOMCanvas **canvas)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateCanvas(canvas);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateGlyphs(
		/* [in] */ IXpsOMFontResource *fontResource,
		/* [retval][out] */ IXpsOMGlyphs **glyphs)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateGlyphs(fontResource, glyphs);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreatePath(
		/* [retval][out] */ IXpsOMPath **path)
	{
		if (p_factory)
		{
			auto res = p_factory->CreatePath(path);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateGeometry(
		/* [retval][out] */ IXpsOMGeometry **geometry)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateGeometry(geometry);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateGeometryFigure(
		/* [in] */ const XPS_POINT *startPoint,
		/* [retval][out] */ IXpsOMGeometryFigure **figure)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateGeometryFigure(startPoint, figure);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateMatrixTransform(
		/* [in] */ const XPS_MATRIX *matrix,
		/* [retval][out] */ IXpsOMMatrixTransform **transform)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateMatrixTransform(matrix, transform);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateSolidColorBrush(
		/* [in] */ const XPS_COLOR *color,
		/* [in] */ IXpsOMColorProfileResource *colorProfile,
		/* [retval][out] */ IXpsOMSolidColorBrush **solidColorBrush)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateSolidColorBrush(color, colorProfile, solidColorBrush);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateColorProfileResource(
		/* [in] */ IStream *acquiredStream,
		/* [in] */ IOpcPartUri *partUri,
		/* [retval][out] */ IXpsOMColorProfileResource **colorProfileResource)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateColorProfileResource(acquiredStream, partUri, colorProfileResource);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateImageBrush(
		/* [in] */ IXpsOMImageResource *image,
		/* [in] */ const XPS_RECT *viewBox,
		/* [in] */ const XPS_RECT *viewPort,
		/* [retval][out] */ IXpsOMImageBrush **imageBrush)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateImageBrush(image, viewBox, viewPort, imageBrush);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateVisualBrush(
		/* [in] */ const XPS_RECT *viewBox,
		/* [in] */ const XPS_RECT *viewPort,
		/* [retval][out] */ IXpsOMVisualBrush **visualBrush)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateVisualBrush(viewBox, viewPort, visualBrush);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateImageResource(
		/* [in] */ IStream *acquiredStream,
		/* [in] */ XPS_IMAGE_TYPE contentType,
		/* [in] */ IOpcPartUri *partUri,
		/* [retval][out] */ IXpsOMImageResource **imageResource)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateImageResource(acquiredStream, contentType, partUri, imageResource);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreatePrintTicketResource(
		/* [in] */ IStream *acquiredStream,
		/* [in] */ IOpcPartUri *partUri,
		/* [retval][out] */ IXpsOMPrintTicketResource **printTicketResource)
	{
		if (p_factory)
		{
			auto res = p_factory->CreatePrintTicketResource(acquiredStream, partUri, printTicketResource);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateFontResource(
		/* [in] */ IStream *acquiredStream,
		/* [in] */ XPS_FONT_EMBEDDING fontEmbedding,
		/* [in] */ IOpcPartUri *partUri,
		/* [in] */ BOOL isObfSourceStream,
		/* [retval][out] */ IXpsOMFontResource **fontResource)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateFontResource(acquiredStream, fontEmbedding, partUri, isObfSourceStream, fontResource);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateGradientStop(
		/* [in] */ const XPS_COLOR *color,
		/* [in] */ IXpsOMColorProfileResource *colorProfile,
		/* [in] */ FLOAT offset,
		/* [retval][out] */ IXpsOMGradientStop **gradientStop)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateGradientStop(color, colorProfile, offset, gradientStop);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateLinearGradientBrush(
		/* [in] */ IXpsOMGradientStop *gradStop1,
		/* [in] */ IXpsOMGradientStop *gradStop2,
		/* [in] */ const XPS_POINT *startPoint,
		/* [in] */ const XPS_POINT *endPoint,
		/* [retval][out] */ IXpsOMLinearGradientBrush **linearGradientBrush)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateLinearGradientBrush(gradStop1, gradStop2, startPoint, endPoint, linearGradientBrush);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateRadialGradientBrush(
		/* [in] */ IXpsOMGradientStop *gradStop1,
		/* [in] */ IXpsOMGradientStop *gradStop2,
		/* [in] */ const XPS_POINT *centerPoint,
		/* [in] */ const XPS_POINT *gradientOrigin,
		/* [in] */ const XPS_SIZE *radiiSizes,
		/* [retval][out] */ IXpsOMRadialGradientBrush **radialGradientBrush)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateRadialGradientBrush(gradStop1, gradStop2, centerPoint, gradientOrigin, radiiSizes, radialGradientBrush);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateCoreProperties(
		/* [in] */ IOpcPartUri *partUri,
		/* [retval][out] */ IXpsOMCoreProperties **coreProperties)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateCoreProperties(partUri, coreProperties);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateDictionary(
		/* [retval][out] */ IXpsOMDictionary **dictionary)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateDictionary(dictionary);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreatePartUriCollection(
		/* [retval][out] */ IXpsOMPartUriCollection **partUriCollection)
	{
		if (p_factory)
		{
			auto res = p_factory->CreatePartUriCollection(partUriCollection);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreatePackageWriterOnFile(
		/* [string][in] */ LPCWSTR fileName,
		/* [unique][in] */ LPSECURITY_ATTRIBUTES securityAttributes,
		/* [in] */ DWORD flagsAndAttributes,
		/* [in] */ BOOL optimizeMarkupSize,
		/* [in] */ XPS_INTERLEAVING interleaving,
		/* [in] */ IOpcPartUri *documentSequencePartName,
		/* [in] */ IXpsOMCoreProperties *coreProperties,
		/* [in] */ IXpsOMImageResource *packageThumbnail,
		/* [in] */ IXpsOMPrintTicketResource *documentSequencePrintTicket,
		/* [in] */ IOpcPartUri *discardControlPartName,
		/* [retval][out] */ IXpsOMPackageWriter **packageWriter)
	{
		if (p_factory)
		{
			auto res = p_factory->CreatePackageWriterOnFile(fileName, securityAttributes, flagsAndAttributes, optimizeMarkupSize, interleaving, documentSequencePartName,
				coreProperties, packageThumbnail, documentSequencePrintTicket, discardControlPartName, packageWriter);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreatePackageWriterOnStream(
		/* [in] */ ISequentialStream *outputStream,
		/* [in] */ BOOL optimizeMarkupSize,
		/* [in] */ XPS_INTERLEAVING interleaving,
		/* [in] */ IOpcPartUri *documentSequencePartName,
		/* [in] */ IXpsOMCoreProperties *coreProperties,
		/* [in] */ IXpsOMImageResource *packageThumbnail,
		/* [in] */ IXpsOMPrintTicketResource *documentSequencePrintTicket,
		/* [in] */ IOpcPartUri *discardControlPartName,
		/* [retval][out] */ IXpsOMPackageWriter **packageWriter)
	{
		if (p_factory)
		{
			auto res = p_factory->CreatePackageWriterOnStream(outputStream, optimizeMarkupSize, interleaving, documentSequencePartName, coreProperties, packageThumbnail,
				documentSequencePrintTicket, discardControlPartName, packageWriter);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreatePartUri(
		/* [string][in] */ LPCWSTR uri,
		/* [retval][out] */ IOpcPartUri **partUri)
	{
		if (p_factory)
		{
			auto res = p_factory->CreatePartUri(uri, partUri);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateReadOnlyStreamOnFile(
		/* [string][in] */ LPCWSTR filename,
		/* [retval][out] */ IStream **stream)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateReadOnlyStreamOnFile(filename, stream);
			return res;
		}

		return S_FALSE;
	}

	void SetFactory(IXpsOMObjectFactory* factory)
	{
		p_factory = factory;
	}
private:
	IXpsOMObjectFactory* p_factory;
};

XpsOMObjectFactoryProxy g_xpsfactory;

class XpsOMPackageWriterProxy : public IXpsOMPackageWriter
{
public:
	XpsOMPackageWriterProxy() : p_xpspackagewriter(nullptr)
	{
	}

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		if (p_xpspackagewriter)
		{
			auto res = p_xpspackagewriter->QueryInterface(riid, ppvObject);
			p_myxpspackagewriter->QueryInterface(riid, ppvObject);
			return res;
		}

		return S_FALSE;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef(void)
	{
		if (p_xpspackagewriter)
		{
			auto res = p_xpspackagewriter->AddRef();
			p_myxpspackagewriter->AddRef();
			return res;
		}

		return S_FALSE;
	}

	virtual ULONG STDMETHODCALLTYPE Release(void)
	{
		if (p_xpspackagewriter)
		{
			auto res = p_xpspackagewriter->Release();
			p_myxpspackagewriter->Release();
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE StartNewDocument(
		/* [in] */ __RPC__in_opt IOpcPartUri *documentPartName,
		/* [in] */ __RPC__in_opt IXpsOMPrintTicketResource *documentPrintTicket,
		/* [in] */ __RPC__in_opt IXpsOMDocumentStructureResource *documentStructure,
		/* [in] */ __RPC__in_opt IXpsOMSignatureBlockResourceCollection *signatureBlockResources,
		/* [in] */ __RPC__in_opt IXpsOMPartUriCollection *restrictedFonts)
	{
		if (p_xpspackagewriter)
		{
			auto res = p_xpspackagewriter->StartNewDocument(documentPartName, documentPrintTicket, documentStructure, signatureBlockResources, restrictedFonts);
			p_myxpspackagewriter->StartNewDocument(documentPartName, documentPrintTicket, documentStructure, signatureBlockResources, restrictedFonts);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE AddPage(
		/* [in] */ __RPC__in_opt IXpsOMPage *page,
		/* [in] */ __RPC__in const XPS_SIZE *advisoryPageDimensions,
		/* [in] */ __RPC__in_opt IXpsOMPartUriCollection *discardableResourceParts,
		/* [in] */ __RPC__in_opt IXpsOMStoryFragmentsResource *storyFragments,
		/* [in] */ __RPC__in_opt IXpsOMPrintTicketResource *pagePrintTicket,
		/* [in] */ __RPC__in_opt IXpsOMImageResource *pageThumbnail)
	{
		if (p_xpspackagewriter)
		{
			auto res = p_xpspackagewriter->AddPage(page, advisoryPageDimensions, discardableResourceParts, storyFragments, pagePrintTicket, pageThumbnail);
			p_myxpspackagewriter->AddPage(page, advisoryPageDimensions, discardableResourceParts, storyFragments, pagePrintTicket, pageThumbnail);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE AddResource(
		/* [in] */ __RPC__in_opt IXpsOMResource *resource)
	{
		if (p_xpspackagewriter)
		{
			auto res = p_xpspackagewriter->AddResource(resource);
			//p_myxpspackagewriter->AddResource(resource);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE Close(void)
	{
		if (p_xpspackagewriter)
		{
			//IXpsOMPackage* package = nullptr;
			//g_xpsfactory.CreatePackage(&package);
			//package->WriteToFile(L"D:\\testxps.xps", NULL, FILE_ATTRIBUTE_NORMAL, FALSE);
			//package->Release();

			auto res = p_xpspackagewriter->Close();
			p_myxpspackagewriter->Close();
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE IsClosed(
		/* [retval][out] */ __RPC__out BOOL *isClosed)
	{
		if (p_xpspackagewriter)
		{
			auto res = p_xpspackagewriter->IsClosed(isClosed);
			//p_myxpspackagewriter->IsClosed(isClosed);
			return res;
		}

		return S_FALSE;
	}

	void SetXpsPackageWriter(IXpsOMPackageWriter* xpspackagewriter)
	{
		p_xpspackagewriter = xpspackagewriter;
	}
	void SetXpsMyPackageWriter(IXpsOMPackageWriter* xpspackagewriter)
	{
		p_myxpspackagewriter = xpspackagewriter;
	}
private:
	IXpsOMPackageWriter* p_xpspackagewriter;
	IXpsOMPackageWriter* p_myxpspackagewriter;
};

XpsOMPackageWriterProxy g_xpspackagewriter;

class XpsDPT : public IXpsDocumentPackageTarget
{
public:
	XpsDPT() : p_xpsdpt(nullptr)
	{
	}

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		if (p_xpsdpt)
		{
			auto res = p_xpsdpt->QueryInterface(riid, ppvObject);
			return res;
		}

		return S_FALSE;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef(void)
	{
		if (p_xpsdpt)
		{
			auto res = p_xpsdpt->AddRef();
			return res;
		}

		return S_FALSE;
	}

	virtual ULONG STDMETHODCALLTYPE Release(void)
	{
		if (p_xpsdpt)
		{
			auto res = p_xpsdpt->Release();
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE GetXpsOMPackageWriter(
		/* [in] */ IOpcPartUri *documentSequencePartName,
		/* [in] */ IOpcPartUri *discardControlPartName,
		/* [retval][out] */ IXpsOMPackageWriter **packageWriter)
	{
		if (p_xpsdpt)
		{
			auto res = p_xpsdpt->GetXpsOMPackageWriter(documentSequencePartName, discardControlPartName, packageWriter);
						
			IXpsOMPackageWriter* mypackageWriter = NULL;
			g_xpsfactory.CreatePackageWriterOnFile(L"D:\\testxps.xps",
				NULL,
				FILE_ATTRIBUTE_NORMAL,
				FALSE,
				XPS_INTERLEAVING_OFF,
				documentSequencePartName,
				NULL,
				NULL,
				NULL,
				discardControlPartName,
				&mypackageWriter);

			g_xpspackagewriter.SetXpsPackageWriter(*packageWriter);
			g_xpspackagewriter.SetXpsMyPackageWriter(mypackageWriter);
			*packageWriter = &g_xpspackagewriter;

			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE GetXpsOMFactory(
		/* [retval][out] */ IXpsOMObjectFactory **xpsFactory)
	{
		if (p_xpsdpt)
		{
			auto res = p_xpsdpt->GetXpsOMFactory(xpsFactory);

			g_xpsfactory.SetFactory(*xpsFactory);
			*xpsFactory = &g_xpsfactory;

			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE GetXpsType(
		/* [retval][out] */ XPS_DOCUMENT_TYPE *documentType)
	{
		if (p_xpsdpt)
		{
			auto res = p_xpsdpt->GetXpsType(documentType);
			return res;
		}

		return S_FALSE;
	}

	void SetXpsDPT(IXpsDocumentPackageTarget* xpsdpt)
	{
		p_xpsdpt = xpsdpt;
	}
private:
	IXpsDocumentPackageTarget* p_xpsdpt;
};

XpsDPT g_xpsdpt;

class PDPT : public IPrintDocumentPackageTarget
{
public:
	PDPT() : p_pdpt(nullptr)
	{
	}

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		if (p_pdpt)
		{
			auto res = p_pdpt->QueryInterface(riid, ppvObject);
			return res;
		}

		return S_FALSE;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef(void)
	{
		if (p_pdpt)
		{
			auto res = p_pdpt->AddRef();
			return res;
		}

		return S_FALSE;
	}

	virtual ULONG STDMETHODCALLTYPE Release(void)
	{
		if (p_pdpt)
		{
			auto res = p_pdpt->Release();
			return res;
		}

		return S_FALSE;
	}

	virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetPackageTargetTypes(
		/* [out] */ __RPC__out UINT32 *targetCount,
		/* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*targetCount) GUID **targetTypes)
	{
		if (p_pdpt)
		{
			auto res = p_pdpt->GetPackageTargetTypes(targetCount, targetTypes);
			return res;
		}

		return S_FALSE;
	}

	virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetPackageTarget(
		/* [in] */ __RPC__in REFGUID guidTargetType,
		/* [in] */ __RPC__in REFIID riid,
		/* [iid_is][out] */ __RPC__deref_out_opt void **ppvTarget)
	{
		if (p_pdpt)
		{
			auto res = p_pdpt->GetPackageTarget(guidTargetType, riid, ppvTarget);

			g_xpsdpt.SetXpsDPT(reinterpret_cast<IXpsDocumentPackageTarget*>(*ppvTarget));
			*ppvTarget = &g_xpsdpt;

			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE Cancel(void)
	{
		if (p_pdpt)
		{
			auto res = p_pdpt->Cancel();
			return res;
		}

		return S_FALSE;
	}

	void SetPDPT(IPrintDocumentPackageTarget* pdpt)
	{
		p_pdpt = pdpt;
	}
private:
	IPrintDocumentPackageTarget* p_pdpt;
};


PDPT g_pdpt;


class PDPTF : public IPrintDocumentPackageTargetFactory
{
public:
	PDPTF() : p_pdptf(nullptr)
	{
	}

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		if (p_pdptf)
		{
			auto res = p_pdptf->QueryInterface(riid, ppvObject);

			if (riid == __uuidof(IPrintDocumentPackageTargetFactory))
			{
				*ppvObject = this;
			}

			return res;
		}

		return S_FALSE;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef(void)
	{
		if (p_pdptf)
		{
			auto res = p_pdptf->AddRef();
			return res;
		}

		return S_FALSE;
	}

	virtual ULONG STDMETHODCALLTYPE Release(void)
	{
		if (p_pdptf)
		{
			auto res = p_pdptf->Release();
			return res;
		}

		return S_FALSE;
	}

	virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateDocumentPackageTargetForPrintJob(
		/* [string][in] */ __RPC__in_string LPCWSTR printerName,
		/* [string][in] */ __RPC__in_string LPCWSTR jobName,
		/* [in] */ __RPC__in_opt IStream *jobOutputStream,
		/* [in] */ __RPC__in_opt IStream *jobPrintTicketStream,
		/* [out] */ __RPC__deref_out_opt IPrintDocumentPackageTarget **docPackageTarget)
	{
		if (p_pdptf)
		{
			auto res = p_pdptf->CreateDocumentPackageTargetForPrintJob(printerName, jobName, jobOutputStream, jobPrintTicketStream, docPackageTarget);

			g_pdpt.SetPDPT(reinterpret_cast<IPrintDocumentPackageTarget*>(*docPackageTarget));
			*docPackageTarget = &g_pdpt;

			return res;
		}

		return S_FALSE;
	}

	void SetPDPTF(IPrintDocumentPackageTargetFactory* pdptf)
	{
		p_pdptf = pdptf;
	}
private:
	IPrintDocumentPackageTargetFactory* p_pdptf;
};

PDPTF g_pdptf;

class ClassFactory : public IClassFactory
{
public:
	ClassFactory() : p_factory(nullptr)
	{
	}

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		if (p_factory)
		{
			auto res = p_factory->QueryInterface(riid, ppvObject);
			return res;
		}

		return S_FALSE;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef(void)
	{
		if (p_factory)
		{
			auto res = p_factory->AddRef();
			return res;
		}

		return S_FALSE;
	}

	virtual ULONG STDMETHODCALLTYPE Release(void)
	{
		if (p_factory)
		{
			auto res = p_factory->Release();
			return res;
		}

		return S_FALSE;
	}

	virtual /* [local] */ HRESULT STDMETHODCALLTYPE CreateInstance(
		/* [annotation][unique][in] */
		_In_opt_  IUnknown *pUnkOuter,
		/* [annotation][in] */
		_In_  REFIID riid,
		/* [annotation][iid_is][out] */
		_COM_Outptr_  void **ppvObject)
	{
		if (p_factory)
		{
			auto res = p_factory->CreateInstance(pUnkOuter, riid, ppvObject);

			if (riid == __uuidof(IPrintDocumentPackageTargetFactory))
			{
				g_pdptf.SetPDPTF(reinterpret_cast<IPrintDocumentPackageTargetFactory*>(*ppvObject));

				*ppvObject = &g_pdptf;
			}

			return res;
		}

		return S_FALSE;
	}

	virtual /* [local] */ HRESULT STDMETHODCALLTYPE LockServer(
		/* [in] */ BOOL fLock)
	{
		if (p_factory)
		{
			auto res = p_factory->LockServer(fLock);
			return res;
		}

		return S_FALSE;
	}

	void SetFactory(IClassFactory* factory)
	{
		p_factory = factory;
	}

private:
	IClassFactory* p_factory;
};

class FactoryBuffer : public IPSFactoryBuffer
{
public:
	FactoryBuffer() : p_buffer(nullptr)
	{
	}

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		if (p_buffer)
		{
			auto res = p_buffer->QueryInterface(riid, ppvObject);
			return res;
		}

		return S_FALSE;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef(void)
	{
		if (p_buffer)
		{
			auto res = p_buffer->AddRef();
			return res;
		}

		return S_FALSE;
	}

	virtual ULONG STDMETHODCALLTYPE Release(void)
	{
		if (p_buffer)
		{
			auto res = p_buffer->Release();
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateProxy(
		/* [annotation][in] */
		_In_  IUnknown *pUnkOuter,
		/* [annotation][in] */
		_In_  REFIID riid,
		/* [annotation][out] */
		_Outptr_  IRpcProxyBuffer **ppProxy,
		/* [annotation][out] */
		_Outptr_  void **ppv)
	{
		if (p_buffer)
		{
			auto res = p_buffer->CreateProxy(pUnkOuter, riid, ppProxy, ppv);
			return res;
		}

		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateStub(
		/* [annotation][in] */
		_In_  REFIID riid,
		/* [annotation][unique][in] */
		_In_opt_  IUnknown *pUnkServer,
		/* [annotation][out] */
		_Outptr_  IRpcStubBuffer **ppStub)
	{
		if (p_buffer)
		{
			auto res = p_buffer->CreateStub(riid, pUnkServer, ppStub);
			return res;
		}

		return S_FALSE;
	}

	void SetFactoryBuffer(IPSFactoryBuffer* buffer)
	{
		p_buffer = buffer;
	}

private:
	IPSFactoryBuffer* p_buffer;
};

ClassFactory g_factory;
FactoryBuffer g_buffer;

HRESULT WINAPI HookedDllGetClassObject
(
	_In_  REFCLSID rclsid,
	_In_  REFIID   riid,
	_Out_ LPVOID   *ppv
)
{
	if (OriginalDllGetClassObject)
	{
		auto res = OriginalDllGetClassObject(rclsid, riid, ppv);

		if (riid == IID_IClassFactory)
		{
			g_factory.SetFactory(reinterpret_cast<IClassFactory*>(*ppv));

			*ppv = &g_factory;
		}
		else if (riid == IID_IPSFactoryBuffer)
		{
			g_buffer.SetFactoryBuffer(reinterpret_cast<IPSFactoryBuffer*>(*ppv));

			*ppv = &g_buffer;
		}
	}

	return S_FALSE;
}

NTSTATUS WINAPI HookedLdrLoadDll
(
	PWCHAR PathToFile, PULONG Flags, PUNICODE_STRING ModuleFileName, PHANDLE ModuleHandle
)
{
	if (OriginalLdrLoadDll)
	{
		auto res = OriginalLdrLoadDll(PathToFile, Flags, ModuleFileName, ModuleHandle);

		if (res == 0 && ModuleFileName->Buffer && ModuleFileName->Length > 0)
		{
				std::wstring full_path(ModuleFileName->Buffer, ModuleFileName->Length);

				const wchar_t* dllNameW = ::PathFindFileNameW(full_path.c_str());
				if (dllNameW)
				{
					if (StrStrIW(dllNameW, L"XpsDocumentTargetPrint"))
					{
						auto dllNameA = std::string(dllNameW, dllNameW + wcslen(dllNameW));

						HMODULE module = ::GetModuleHandle(L"XpsDocumentTargetPrint");
						OriginalDllGetClassObject = reinterpret_cast<PNT_DllGetClassObject>(GetProcAddress(module, "DllGetClassObject"));
						if (OriginalDllGetClassObject)
						{
							Mhook_SetHook(reinterpret_cast<PVOID*>(&OriginalDllGetClassObject), HookedDllGetClassObject);
						}
					}
				}
		}

		return res;
	}

	return NULL;
}

NTSTATUS WINAPI HookedLdrUnloadDll
(
	HANDLE ModuleHandle
)
{
	if (OriginalLdrLoadDll)
	{
		//auto res = OriginalLdrUnloadDll(ModuleHandle);

		auto res = S_OK;
		return res;
	}

	return NULL;
}


void GetFunctions()
{
	HMODULE hspool = ::GetModuleHandle(L"ntdll");

	if (hspool)
	{
		OriginalLdrLoadDll = reinterpret_cast<PNT_LdrLoadDll>(GetProcAddress(hspool, "LdrLoadDll"));
		if (OriginalLdrLoadDll)
		{
			Mhook_SetHook(reinterpret_cast<PVOID*>(&OriginalLdrLoadDll), HookedLdrLoadDll);
		}

		OriginalLdrUnloadDll = reinterpret_cast<PNT_LdrUnloadDll>(GetProcAddress(hspool, "LdrUnloadDll"));
		if (OriginalLdrUnloadDll)
		{
			Mhook_SetHook(reinterpret_cast<PVOID*>(&OriginalLdrUnloadDll), HookedLdrUnloadDll);
		}
	}
}


LRESULT GetMsgProc(
	_In_ int    code,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	return CallNextHookEx(NULL, code, wParam, lParam);
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		//TCHAR name[1024] = { 0 };
		//DWORD chc = GetModuleFileName(hModule, name, 1024);
		//LoadLibraryW(name);
		//HMODULE module;
		//GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_PIN, name, &module);

		GetFunctions();
		break;
	}
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		if (OriginalLdrLoadDll)
		{
			Mhook_Unhook(reinterpret_cast<PVOID*>(&OriginalLdrLoadDll));
			OriginalLdrLoadDll = nullptr;
		}
		if (OriginalDllGetClassObject)
		{
			Mhook_Unhook(reinterpret_cast<PVOID*>(&OriginalDllGetClassObject));
			OriginalDllGetClassObject = nullptr;
		}
	}
	return TRUE;
}

