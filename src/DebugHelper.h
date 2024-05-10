#pragma once
#include <Windows.h>

class DebugHelper
{
public:
	DebugHelper();
	~DebugHelper();
	static void DebugHelper::PrintPixelFormatDescriptor(PIXELFORMATDESCRIPTOR* pfd, int pixelFormat);
	static void DebugHelper::PrintFormat(LPCTSTR lpszFormat, ...);
};

