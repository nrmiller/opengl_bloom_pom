#include "GLWindow.h"
#include "Includes.h"

using namespace std;

GLWindow::GLWindow(LPCTSTR title, int width, int height)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	this->title = title;
	this->width = width;
	this->height = height;

	//Create Class.
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = NULL;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = title;
	wcex.hIconSm = NULL;

	//Register Class.
	if (!RegisterClassEx(&wcex))
	{
		const int errSize = 27 + 256 + 1;
		TCHAR errMessage[errSize];
		StringCchPrintf(errMessage, errSize, _T("Failed to register class: %s."), title);
		MessageBox(NULL, errMessage, _T("GLWindow Error"), MB_OK | MB_ICONERROR);
	}

	//Create Window.
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	hWnd = CreateWindow(title, title, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		screenWidth / 2 - width / 2,
		screenHeight / 2 - height / 2,
		width, height,
		NULL,
		NULL,
		hInstance,
		NULL);

	//Make sure window was created successfully.
	if (!hWnd)
	{
		const int errSize = 40 + 256 + 1;
		TCHAR errMessage[errSize];
		StringCchPrintf(errMessage, errSize, _T("Failed to create window for the class: %s."), title);
		MessageBox(NULL, errMessage, _T("Window Error"), MB_OK | MB_ICONERROR);
	}


	//Set this class as extra user data for the HWND.
	//That will allow the window to avoid a singleton WndProc.
	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	//Create a pixel format descriptor with the desired settings.
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,			//Color bits.
		0, 0, 0, 0, 0, 0,
		8,			//Alpha bits.
		0,
		0,			//Accumulation bits.
		0, 0, 0, 0,	//Accumulation RGBA bits.
		32,			//Depth bits.
		0,			//Stencil bits.
		0,			//Number of auxiliary buffers.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	//Get the device context associated with the window.
	hdc = GetDC(hWnd);

	//Find the closest match for the desired pixel format
	//supported by the graphics hardware.
	int pixelFormat = ChoosePixelFormat(hdc, &pfd);

	//Use the best fitting pixel format.
	if (!SetPixelFormat(hdc, pixelFormat, &pfd))
	{
		MessageBox(NULL, _T("Failed to set pixel format."), _T("Window Error"), MB_OK | MB_ICONERROR);
	}

	//Create a dummy OpenGL Rendering Context.
	hglrc = wglCreateContext(hdc);
	if (!hglrc)
	{
		MessageBox(NULL, _T("Failed to create OpenGL Context"), _T("Window Error"), MB_OK | MB_ICONERROR);
	}

	//Make the dummy OpenGL Rendering Context current.
	if (!wglMakeCurrent(hdc, hglrc))
	{
		MessageBox(NULL, _T("Failed to make OpenGL Context current."), _T("Window Error"), MB_OK | MB_ICONERROR);
	}

	//Once the dummy context has been created, we can locate the extension which allows us to specify
	//more precise context creation attributes.  That is, we can now access the extension which allows us
	//to choose which version of OpenGL we wish to use.
	//We will use GLEW to obtain pointers to these extensions (as well as others).
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		MessageBox(NULL, _T("Failed to initialize GLEW."), _T("GLEW Error"), MB_OK | MB_ICONERROR);
	}

	//If the desired extension is available, we will use it.
	if (wglewIsSupported("WGL_ARB_create_context"))
	{
		//Stop using the dummy context and delete it.
		wglMakeCurrent(hdc, NULL);
		wglDeleteContext(hglrc);

		//Create new context and specify version & core profile mode.
		int attributes[] = 
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0 //Denotes end of attribute list.
		};
		hglrc = wglCreateContextAttribsARB(hdc, NULL, attributes);
		wglMakeCurrent(hdc, hglrc);

		//Check if OpenGL 3.3. was successfully applied.
		//That way, we can show an error message if necessary.
		string version = "";
		if (glGetString(GL_VERSION) != 0)
		{
			version = string((char*)glGetString(GL_VERSION));
		}
		int i1 = version.find('.', 0);
		int major = 0;
		int minor = 0;
		if (i1 != -1)
		{
			//Look for minor version if a major version is present.
			major = stoi(version.substr(0, i1));
			int i2 = version.find('.', i1 + 1);
			if (i2 == -1)
			{
				i2 = version.find(' ', i1 + 1);
				if (i2 != -1)
				{
					minor = stoi(version.substr(i1 + 1, i2));
				}
			}
			else
			{
				minor = stoi(version.substr(i1 + 1, i2));
			}
		}
		if (!(major >= 3 && minor >= 3))
		{
			char dest[300];
			StringCchPrintfA(dest, 300,
				"You must have OpenGL Version 3.3 or higher on your system to run this program."
				"Your system details are below:\n"
				"Vendor: %s\n"
				"Renderer: %s\n"
				"GL Version: %s\n"
				"GLSL Version: %s\n",
				(LPCSTR)glGetString(GL_VENDOR),
				(LPCSTR)glGetString(GL_RENDERER),
				(LPCSTR)glGetString(GL_VERSION),
				(LPCSTR)glGetString(GL_SHADING_LANGUAGE_VERSION));
			MessageBoxA(NULL, dest, "System Requirements Error", MB_OK | MB_ICONERROR);
			ExitProcess(-1);
		}
		else
		{
			char dest[256];
			StringCchPrintfA(dest, 220,
				"Your system details are below:\n"
				"Vendor: %s\n"
				"Renderer: %s\n"
				"GL Version: %s\n"
				"GLSL Version: %s\n",
				(LPCSTR)glGetString(GL_VENDOR),
				(LPCSTR)glGetString(GL_RENDERER),
				(LPCSTR)glGetString(GL_VERSION),
				(LPCSTR)glGetString(GL_SHADING_LANGUAGE_VERSION));
			OutputDebugStringA(dest);
		}
	}
	else
	{
		MessageBox(NULL, _T("WGL_ARB_create_context extensions is not available."), _T("GLEW Error"), MB_OK | MB_ICONERROR);
		ExitProcess(-1);
	}
}

GLWindow::~GLWindow()
{

}

int GLWindow::ShowDialog()
{
	//Perform any OpenGL initialization.
	if (lpfnInitProc)
	{
		lpfnInitProc();
	}

	//Restores the window and activates it.
	ShowWindow(hWnd, SW_SHOWNORMAL);

	//Start message loop.
	MSG msg;
	BOOL running = true;
	while (running)
	{
		//Check for any windows messages.
		//If none exist, then redraw the OpenGL scene.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				running = false;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			//Render the scene.
			if (lpfnDisplayProc)
			{
				lpfnDisplayProc();
			}

			SwapBuffers(hdc);
		}
	}

	return (int)msg.wParam;
}

bool GLWindow::IsActive()
{
	return (hWnd == GetForegroundWindow());
}

void GLWindow::Close()
{
	//Peform any OpenGL cleanup.
	if (lpfnDestroyProc)
	{
		lpfnDestroyProc();
	}

	//Stop using the OpenGL Rendering Context and delete it.
	wglMakeCurrent(hdc, NULL);
	wglDeleteContext(hglrc);
	
	ReleaseDC(hWnd, hdc);

	PostQuitMessage(0);
}

LRESULT CALLBACK GLWindow::InstanceWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WINDOWPOS* pos;
	switch (msg)
	{
	case WM_WINDOWPOSCHANGED:
		pos = (WINDOWPOS*)lParam;
		x = pos->x;
		y = pos->y;
		width = pos->cx;
		height = pos->cy;
		if (height == 0)
		{
			//Don't allow height to be zero since
			//aspect ratio would be undefined.
			width = 1;
			height = 1;
		}
		break;
	case WM_SIZE:
		width = LOWORD(lParam);
		height = HIWORD(lParam);
		if (height == 0)
		{
			//Don't allow height to be zero since
			//aspect ratio would be undefined.
			width = 1;
			height = 1;
		}

		if (lpfnReshapeProc)
		{
			//Pass the new width and height to the reshape procedure.
			lpfnReshapeProc(width, height);
		}
		break;
	case WM_SIZING:
		//Render the scene.
		//This prevents the screen from having a solid-color background while resizing.
		if (lpfnDisplayProc)
		{
			lpfnDisplayProc();
		}

		SwapBuffers(hdc);
		break;
	case WM_KEYDOWN:
		keyboard.SetPressed((int)wParam, true);
		break;
	case WM_KEYUP:
		keyboard.SetPressed((int)wParam, false);
		break;
	case WM_SETCURSOR:
		if (cursorVisible)
		{
			SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		}
		else
		{
			SetCursor(LoadCursor(NULL, NULL));
		}
		return true;
	case WM_MOUSEMOVE:
		mouse.x = GET_X_LPARAM(lParam);
		mouse.y = GET_Y_LPARAM(lParam);
		break;
	case WM_MOUSEWHEEL:
		//DebugHelper::PrintFormat(_T("Wheel: %d\n"), (short)HIWORD(wParam));
		mouse.zoom -= ((short)HIWORD(wParam)) / WHEEL_DELTA;
		
		if (mouse.zoom < -6)
			mouse.zoom = -6;
		if (mouse.zoom > 6)
			mouse.zoom = 6;

		break;
	case WM_DESTROY:
		Close();
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void GLWindow::SetCursorVisible(bool visible)
{
	cursorVisible = visible;
}
