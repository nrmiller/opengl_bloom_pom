#include "DebugHelper.h"
#include <tchar.h>
#include <strsafe.h>

DebugHelper::DebugHelper()
{
}


DebugHelper::~DebugHelper()
{
}

void DebugHelper::PrintPixelFormatDescriptor(PIXELFORMATDESCRIPTOR* pfd, int pixelFormat)
{
	OutputDebugString(_T("==================================\n"));
	OutputDebugString(_T("Pixel Format Description:\n"));

	TCHAR line[100];
	StringCchPrintf(line, 100, _T("Pixel Format Index: %d\n"), pixelFormat);
	OutputDebugString(line);
	StringCchPrintf(line, 100, _T("Size: %d\n"), pfd->nSize);
	OutputDebugString(line);
	StringCchPrintf(line, 100, _T("Version: %d\n"), pfd->nVersion);
	OutputDebugString(line);
	StringCchPrintf(line, 100, _T("Flags: 0x%08X\n"), pfd->dwFlags);
	OutputDebugString(line);
	StringCchPrintf(line, 100, _T("Pixel Type: %d\n"), pfd->iPixelType);
	OutputDebugString(line);
	StringCchPrintf(line, 100, _T("Color Bits: %d\n"), pfd->cColorBits);
	OutputDebugString(line);
	StringCchPrintf(line, 100, _T("Red Bits: %d, Shift: %d\n"), pfd->cRedBits, pfd->cRedShift);
	OutputDebugString(line);
	StringCchPrintf(line, 100, _T("Green Bits: %d, Shift: %d\n"), pfd->cGreenBits, pfd->cGreenShift);
	OutputDebugString(line);
	StringCchPrintf(line, 100, _T("Blue Bits: %d, Shift: %d\n"), pfd->cBlueBits, pfd->cBlueShift);
	OutputDebugString(line);
	StringCchPrintf(line, 100, _T("Alpha Bits: %d, Shift: %d\n"), pfd->cAlphaBits, pfd->cAlphaShift);
	OutputDebugString(line);
	StringCchPrintf(line, 100, _T("Accum Bits: %d, RGBA Bits(%d, %d, %d, %d)\n"), pfd->cAccumBits, pfd->cAccumRedBits, pfd->cAccumGreenBits, pfd->cAccumBlueBits, pfd->cAccumAlphaBits);
	OutputDebugString(line);
	StringCchPrintf(line, 100, _T("Depth Bits: %d\n"), pfd->cDepthBits);
	OutputDebugString(line);
	StringCchPrintf(line, 100, _T("Stencil Bits: %d\n"), pfd->cStencilBits);
	OutputDebugString(line);
	StringCchPrintf(line, 100, _T("Auxilary Buffers: %d\n"), pfd->cAuxBuffers);
	OutputDebugString(line);
	StringCchPrintf(line, 100, _T("Layer Type: 0x%08X\n"), pfd->iLayerType);
	OutputDebugString(line);
	StringCchPrintf(line, 100, _T("bReserved: %d\n"), pfd->bReserved);
	OutputDebugString(line);
	StringCchPrintf(line, 100, _T("Layer Mask: 0x%08X\n"), pfd->dwLayerMask);
	OutputDebugString(line);
	StringCchPrintf(line, 100, _T("Visible Mask: 0x%08X\n"), pfd->dwVisibleMask);
	OutputDebugString(line);
	StringCchPrintf(line, 100, _T("Damage Mask: 0x%08X\n"), pfd->dwDamageMask);
	OutputDebugString(line);
	OutputDebugString(_T("==================================\n"));
}

void DebugHelper::PrintFormat(LPCTSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);

	TCHAR output[512] = {};
	_vstprintf(output, 511, lpszFormat, args);

	OutputDebugString(output);
	
	va_end(args);
}
