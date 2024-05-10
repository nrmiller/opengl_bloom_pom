#pragma once
#include <Windows.h>
#include "Keyboard.h"
#include "Mouse.h"

typedef void (*InitProc)();
typedef void (*ReshapeProc)(int width, int height);
typedef void (*DisplayProc)();
typedef void (*DestroyProc)();

class GLWindow
{
public:
	GLWindow(LPCTSTR title, int width, int height);
	~GLWindow();

	LPCTSTR title;
	int x;
	int y;
	int width;
	int height;
	InitProc lpfnInitProc;
	ReshapeProc lpfnReshapeProc;
	DisplayProc lpfnDisplayProc;
	DestroyProc lpfnDestroyProc;
	Keyboard keyboard;
	Mouse mouse;

	int ShowDialog();
	bool IsActive();
	void SetCursorVisible(bool visible);
	void Close();
	HWND hWnd;
private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		//Obtain the user data for the HWND that uses this singleton WndProc.
		//If the user data refers to an instance of GLWindow, then use the instance-scope WndProc
		//instead.  Otherwise, use the default WndProc.
		//This convolution is necessary since it is not possible to directly assign a class function
		//as the WndProc when creating a window.  Only free functions or static functions are permitted.
		GLWindow* glWindow = reinterpret_cast<GLWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		if (glWindow)
		{
			return glWindow->InstanceWndProc(hWnd, msg, wParam, lParam);
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT CALLBACK InstanceWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
protected:
	HDC hdc;
	HGLRC hglrc;
	bool cursorVisible = true;
};