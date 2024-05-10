#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/wglew.h>

#include <Windows.h>
#include <Windowsx.h>
#include <tchar.h>
#include <strsafe.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>

#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef _DEBUG
#include "DebugHelper.h"
#endif