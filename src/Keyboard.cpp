#include "Keyboard.h"
#include "Includes.h"

Keyboard::Keyboard()
{
	memset(keys, 0, 256);
}

Keyboard::~Keyboard()
{

}

bool Keyboard::IsPressed(int vkCode)
{
	return keys[vkCode];
}

void Keyboard::SetPressed(int vkCode, bool pressed)
{
	keys[vkCode] = pressed;
}

void Keyboard::Print()
{
	OutputDebugString(_T("Checking Keys.\n"));
	for (int i = 0; i < 256; i++)
	{
		if (keys[i])
		{
			TCHAR format[256];
			StringCchPrintf(format, 256, _T("Key= 0x04X.\n"), keys[i]);
			OutputDebugString(format);
		}
	}
}