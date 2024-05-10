#include "Includes.h"
#include "Camera.h"
#include "GLWindow.h"
#include "Sphere.h"
#include "Cuboid.h"
#include "Square.h"
#include "ShaderManager.h"
#include "TGALoader.h"

DWORD WINAPI Update(LPVOID parameter);
void Init();
void Display();
void Reshape(int width, int height);
void Destroy();

void CenterMouseWithoutOffset(GLWindow* window, Mouse* mouse, int* lasX, int* lastY);
GLuint CreateProgram(LPCTSTR vertexSource, LPCTSTR fragmentSource);

bool running = true;
GLWindow* window;
Mouse* mouse;
Camera camera;
Sphere* sphere;
Sphere* sphere2;
Sphere* sphere3;
Sphere* sphere4;
Cuboid* cuboid1;
Square* floor1;
Square* renderTarget;
Cuboid *light1, *light2, *light3, *light4, *light5, *light6, *light7, *light8;
ShaderManager manager;
GLuint parallax_normal, bloom_first, bloom_lights, gaussian_blur, bloom_combine;
Texture diffuseMap, normalMap, depthMap;
Texture diffuseMap2, normalMap2;
Texture diffuseMap3, normalMap3;
GLuint bloomFBO;
GLuint rboDepth;
GLuint colorBuffers[2];
GLuint pingPongFBOs[2];
GLuint pingPongColorBuffers[2];
glm::mat4 projection;
bool normalMappingEnabled = false;
bool parallaxMappingEnabled = false;
bool bloomEnabled = false;
glm::vec3 cuboidPos = glm::vec3(-4.0f, 2.0f, 0.0f);
glm::vec3 cuboidRot;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	GLWindow window = GLWindow(_T("Assignment 2 - Parallax Occlusion Mapping & Bloom Filter"), 600, 400);
	CreateThread(NULL, 0, Update, &window, 0, NULL);

	window.lpfnInitProc = Init;
	window.lpfnDisplayProc = Display;
	window.lpfnReshapeProc = Reshape;
	return window.ShowDialog();

	running = false;
}

DWORD WINAPI Update(LPVOID parameter)
{
	float speed = 2.0f;
	float angularSpeed = 20.0f;
	float cuboidAngularSpeed = 1.0f;

	window = reinterpret_cast<GLWindow*>(parameter);
	Keyboard* keyboard = &(window->keyboard);
	mouse = &(window->mouse);
	camera.SetEye(0.0f, 4.0f, 14.0f);
	
	int lastX = 0;
	int lastY = 0;
	bool firstMouse = true;
	bool useOffset = true;
	bool paused = false;
	bool escapeReleased = true;
	bool nReleased = true;
	bool pReleased = true;
	bool bReleased = true;

	while (running)
	{
		//Poll keyboard.
		float speedMultiplier = 3.0f;
		if (keyboard->IsPressed(VK_SHIFT))
		{
			speedMultiplier = 6.0f;
		}

		float distance = speed * speedMultiplier * .0333f;
		float angularDistance = angularSpeed * speedMultiplier * .0333f;
		float cuboidAngularDistance = cuboidAngularSpeed * speedMultiplier * .0333f;

		//Movement controls
		if (keyboard->IsPressed('W'))
		{
			camera.MoveForward(distance);
		}
		if (keyboard->IsPressed('A'))
		{
			camera.StrafeLeft(distance);
		}
		if (keyboard->IsPressed('S'))
		{
			camera.MoveBackward(distance);
		}
		if (keyboard->IsPressed('D'))
		{
			camera.StrafeRight(distance);
		}
		
		//Rotate view.
		if (keyboard->IsPressed(VK_LEFT))
		{
			camera.RotateYaw(-angularDistance);
		}
		if (keyboard->IsPressed(VK_RIGHT))
		{
			camera.RotateYaw(angularDistance);
		}
		if (keyboard->IsPressed(VK_DOWN))
		{
			camera.RotatePitch(-angularDistance);
		}
		if (keyboard->IsPressed(VK_UP))
		{
			camera.RotatePitch(angularDistance);
		}

		if (keyboard->IsPressed('R'))
		{
			//Reset to defaults.
			camera.SetEye(0.0f, 4.0f, 14.0f);
			camera.SetRotation(-90.0f, 0.0f, 0.0f);
			mouse->zoom = 0;
			cuboidPos = glm::vec3(-4.0f, 2.0f, 0.0f);
			cuboidRot = glm::vec3();
		}
		if (keyboard->IsPressed(VK_ESCAPE))
		{
			if (escapeReleased)
			{
				paused = !paused;
				escapeReleased = false;
			}
		}
		else //ESC Released
		{
			escapeReleased = true;
		}

		//Show or hide cursor.
		if (paused)
		{
			window->SetCursorVisible(true);
		}
		else
		{
			window->SetCursorVisible(false);
		}

		//The logic below is necessary for repositioning the cursor without moving the view.
		//This is done so that the cursor can be moved indefinitely (by the user) to rotate the view.
		if (firstMouse)
		{
			CenterMouseWithoutOffset(window, mouse, &lastX, &lastY);
			firstMouse = false;
		}
		
		float offsetX = mouse->x - lastX;
		float offsetY = lastY - mouse->y;

		lastX = mouse->x;
		lastY = mouse->y;

		//If the window is currently the active window, then reset the cursor position.
		if (window->IsActive() && !paused)
		{
			CenterMouseWithoutOffset(window, mouse, &lastX, &lastY);
		}

		camera.RotateYaw(offsetX * 0.3f);
		camera.RotatePitch(offsetY * 0.3f);

		if (keyboard->IsPressed('N'))
		{
			if (nReleased)
			{
				normalMappingEnabled = !normalMappingEnabled;
				nReleased = false;
			}
		}
		else
		{
			nReleased = true;
		}

		if (keyboard->IsPressed('P'))
		{
			if (pReleased)
			{
				parallaxMappingEnabled = !parallaxMappingEnabled;
				pReleased = false;
			}
		}
		else
		{
			pReleased = true;
		}

		if (keyboard->IsPressed('B'))
		{
			if (bReleased)
			{
				bloomEnabled = !bloomEnabled;
				bReleased = false;
			}
		}
		else
		{
			bReleased = true;
		}

		if (keyboard->IsPressed('I'))
		{
			cuboidPos.z -= distance;
		}
		if (keyboard->IsPressed('J'))
		{
			cuboidPos.x -= distance;
		}
		if (keyboard->IsPressed('K'))
		{
			cuboidPos.z += distance;
		}
		if (keyboard->IsPressed('L'))
		{
			cuboidPos.x += distance;
		}
		if (keyboard->IsPressed('U'))
		{
			cuboidPos.y -= distance;
		}
		if (keyboard->IsPressed('O'))
		{
			cuboidPos.y += distance;
		}

		if (keyboard->IsPressed(VK_NUMPAD4))
		{
			cuboidRot.y -= cuboidAngularDistance;
		}
		if (keyboard->IsPressed(VK_NUMPAD6))
		{
			cuboidRot.y += cuboidAngularDistance;
		}
		if (keyboard->IsPressed(VK_NUMPAD2))
		{
			cuboidRot.x += cuboidAngularDistance;
		}
		if (keyboard->IsPressed(VK_NUMPAD8))
		{
			cuboidRot.x -= cuboidAngularDistance;
		}

		Sleep(33);
	}

	return 0;
}

void CenterMouseWithoutOffset(GLWindow* window, Mouse* mouse, int* lastX, int* lastY)
{
	//Mouse mouse to the center of the window.
	SetCursorPos(window->x + window->width / 2, window->y + window->height / 2);

	//Compute the new mouse coordinates in client-space.
	POINT p = POINT();
	p.x = window->x + window->width / 2;
	p.y = window->y + window->height / 2;
	ScreenToClient(window->hWnd, &p);

	//Apply the new mouse coordinates in client-space and mark them as the previous coordinates.
	mouse->x = p.x;
	mouse->y = p.y;
	*lastX = mouse->x;
	*lastY = mouse->y;
}

void Init()
{
	glEnable(GL_DEPTH_TEST);

	sphere = new Sphere(0.1f, 10, 10);
	sphere2 = new Sphere(2.0f, 20, 20);
	sphere3 = new Sphere(4.0f, 30, 30);
	sphere4 = new Sphere(1.0f, 20, 20);
	cuboid1 = new Cuboid(4.0f, 4.0f, 4.0f);
	floor1 = new Square(20.0f, 20.0f);
	light1 = new Cuboid(1.0f, 1.0f, 1.0f, glm::vec3(1.0f, 0.76f, 0.39f));
	light2 = new Cuboid(1.0f, 1.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	light3 = new Cuboid(1.0f, 1.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	light4 = new Cuboid(1.0f, 1.0f, 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	light5 = new Cuboid(0.65f, 0.65f, 0.65f, glm::vec3(0.8f, 0.85f, 0.3f));
	light6 = new Cuboid(0.65f, 0.65f, 0.65f, glm::vec3(0.8f, 0.85f, 0.3f));
	light7 = new Cuboid(0.65f, 0.65f, 0.65f, glm::vec3(0.8f, 0.85f, 0.3f));
	light8 = new Cuboid(0.65f, 0.65f, 0.65f, glm::vec3(0.8f, 0.85f, 0.3f));

	parallax_normal = CreateProgram(_T("parallax_normal.vert"), _T("parallax_normal.frag"));
	bloom_first = CreateProgram(_T("bloom_first.vert"), _T("bloom_first.frag"));
	bloom_lights = CreateProgram(_T("bloom_lights.vert"), _T("bloom_lights.frag"));
	gaussian_blur = CreateProgram(_T("gaussian_blur.vert"), _T("gaussian_blur.frag"));
	bloom_combine = CreateProgram(_T("bloom_combine.vert"), _T("bloom_combine.frag"));
	
	diffuseMap3 = TGALoader::Load(string("StoneBrick.tga"));
	normalMap3 = TGALoader::Load(string("StoneBrick_NormalMap.tga"));
	diffuseMap2 = TGALoader::Load(string("brickwall.tga"));
	normalMap2 = TGALoader::Load(string("brickwall_normal.tga"));
	diffuseMap = TGALoader::Load(string("bricks2.tga"));
	normalMap = TGALoader::Load(string("bricks2_normals.tga"));
	depthMap = TGALoader::Load(string("bricks2_depth.tga"));

	glGenFramebuffers(1, &bloomFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO);
	glGenTextures(2, colorBuffers);
	for (int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 600, 400, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
	}
	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	//Attach depth buffer.
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 600, 400);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	//Create ping-pong FBO's to perform gaussian-blurring.
	glGenFramebuffers(2, pingPongFBOs);
	glGenTextures(2, pingPongColorBuffers);
	//Attach color buffers (1 buffer per ping-pong FBO)
	for (int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingPongFBOs[i]);
		glBindTexture(GL_TEXTURE_2D, pingPongColorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 600, 400, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingPongColorBuffers[i], 0);
	}
}

GLuint CreateProgram(LPCTSTR vertexSource, LPCTSTR fragmentSource)
{
	GLuint program = 0;
	GLuint vertexShader = manager.CompileShader(vertexSource, GL_VERTEX_SHADER);
	if (vertexShader == 0) { OutputDebugString(_T("Error compiling vertex shader!\n")); }
	else { OutputDebugString(_T("Vertex shader compiled successfully!\n")); }

	GLuint fragmentShader = manager.CompileShader(fragmentSource, GL_FRAGMENT_SHADER);
	if (fragmentShader == 0) { OutputDebugString(_T("Error compiling fragment shader!\n")); }
	else { OutputDebugString(_T("Fragment shader compiled successfully!\n")); }

	program = manager.LinkShader(vertexShader, fragmentShader);
	if (program <= 0) { OutputDebugString(_T("Error linking shader program!\n")); }
	else { OutputDebugString(_T("Shader program linked successfully!\n")); }

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

void Display()
{
	glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO);
	//Resize color buffer and depth buffer storages.
	for (int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window->width, window->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	}
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window->width, window->height);
	
	glClearColor(0.4f, 0.6f, 1.0f, 1.0f);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (mouse != NULL && window != NULL)
	{
		float fov = 60.0f + (mouse->zoom * 5.0f);
		projection = glm::perspective(glm::radians(fov), (float)window->width / (float)window->height, 0.1f, 100.0f);
	}

	glm::mat4 view = camera.GetViewTransform();
	glm::mat4 model;

	//-----------------------------------------------------------------------
	// Render Cube with Normal Mapping and Parallax Occlusion Mapping.
	// 1 light is included (in the shader).
	//-----------------------------------------------------------------------
	glUseProgram(parallax_normal);

	glUniform3fv(glGetUniformLocation(parallax_normal, "viewPosition"), 1, glm::value_ptr(camera.GetPosition()));

	GLuint modelLocation = glGetUniformLocation(parallax_normal, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	GLuint viewLocation = glGetUniformLocation(parallax_normal, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	GLuint projectionLocation = glGetUniformLocation(parallax_normal, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	model = glm::mat4();
	model = glm::translate(model, cuboidPos);
	model = glm::rotate(model, cuboidRot.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, cuboidRot.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap.id);
	glUniform1i(glGetUniformLocation(parallax_normal, "diffuseMap"), 0); //Uniform is at texture unit 0.
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalMap.id);
	glUniform1i(glGetUniformLocation(parallax_normal, "normalMap"), 1); //Uniform is at texture unit 1.
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, depthMap.id);
	glUniform1i(glGetUniformLocation(parallax_normal, "depthMap"), 2); //Uniform is at texture unit 2.

	glUniform1i(glGetUniformLocation(parallax_normal, "normalMappingEnabled"), normalMappingEnabled);
	glUniform1i(glGetUniformLocation(parallax_normal, "parallaxMappingEnabled"), parallaxMappingEnabled);

	cuboid1->Draw();

	//-----------------------------------------------------------------------
	// This is the Bloom Scene
	// Render floor and more bricks with Normal Mapping and 8 lights (in the shaders).
	//-----------------------------------------------------------------------
	glUseProgram(bloom_first);

	glUniform3fv(glGetUniformLocation(bloom_first, "viewPosition"), 1, glm::value_ptr(camera.GetPosition()));

	modelLocation = glGetUniformLocation(bloom_first, "model");
	model = glm::mat4();;
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

	viewLocation = glGetUniformLocation(bloom_first, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	projectionLocation = glGetUniformLocation(bloom_first, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap2.id);
	glUniform1i(glGetUniformLocation(bloom_first, "diffuseMap"), 0); //Uniform is at texture unit 0.

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalMap2.id);
	glUniform1i(glGetUniformLocation(bloom_first, "normalMap"), 1); //Uniform is at texture unit 1.

	glUniform1i(glGetUniformLocation(bloom_first, "normalMappingEnabled"), normalMappingEnabled);

	floor1->Draw();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-8.0f, 2.0f, -8.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap.id);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalMap.id);
	cuboid1->Draw();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(8.0f, 2.0f, -8.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap2.id);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalMap2.id);
	cuboid1->Draw();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(8.0f, 2.0f, 8.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap3.id);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalMap3.id);
	cuboid1->Draw();

	//-----------------------------------------------------------------------
	// This is the Bloom Scene
	// Render cubes to represent the 8 lights.
	//-----------------------------------------------------------------------
	glUseProgram(bloom_lights);
	glUniform3fv(glGetUniformLocation(bloom_lights, "viewPosition"), 1, glm::value_ptr(camera.GetPosition()));

	modelLocation = glGetUniformLocation(bloom_lights, "model");
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-2.0f, 4.0f, 4.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	viewLocation = glGetUniformLocation(bloom_lights, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	projectionLocation = glGetUniformLocation(bloom_lights, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	light1->Draw();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(5.0f, 3.0f, 5.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	light2->Draw();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-5.0f, 3.0f, -5.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	light3->Draw();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(5.0f, 3.0f, -5.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	light4->Draw();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(10.0f, 5.0f, 10.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	light5->Draw();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-10.0f, 5.0f, -10.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	light6->Draw();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(10.0f, 5.0f, -10.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	light7->Draw();

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-10.0f, 5.0f, 10.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	light8->Draw();

	//Take results of bloom_first & bloom_lights, and blur them using the ping-pong FBO's.
	glUseProgram(gaussian_blur);
	glm::mat4 orthoProjection = glm::ortho(0.0f, (float)window->width, 0.0f, (float)window->height, 0.0f, 1.0f);
	glm::mat4 orthoView = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(gaussian_blur, "projection"), 1, GL_FALSE, glm::value_ptr(orthoProjection));
	glUniformMatrix4fv(glGetUniformLocation(gaussian_blur, "view"), 1, GL_FALSE, glm::value_ptr(orthoView));

	//Resize ping-pong colorbuffers dynamically.
	for (int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingPongFBOs[i]);
		glBindTexture(GL_TEXTURE_2D, pingPongColorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window->width, window->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	}
	bool horizontal = true;
	int gaussianBlurPasses = 5;
	for (int i = 0; i < gaussianBlurPasses * 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingPongFBOs[horizontal]);
		glClear(GL_COLOR_BUFFER_BIT);
		glUniform1i(glGetUniformLocation(gaussian_blur, "horizontal"), horizontal);

		glActiveTexture(GL_TEXTURE0);
		//On first interation, perform gaussian blur on the bright colors
		//then render into the horizontal framebuffer.
		//Every other iteration should take from the other framebuffer's colorbuffer.
		glBindTexture(GL_TEXTURE_2D, (i == 0) ? colorBuffers[1] : pingPongColorBuffers[!horizontal]);
		glUniform1i(glGetUniformLocation(gaussian_blur, "image"), 0); //Uniform is at texture unit 0.

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(window->width / 2.0f, window->height / 2.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(gaussian_blur, "model"), 1, GL_FALSE, glm::value_ptr(model));
		renderTarget->Draw();
		horizontal = !horizontal;
	}

	//Draw scene and blurred-bloom FBO's to default framebuffer.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(bloom_combine);
	orthoProjection = glm::ortho(0.0f, (float)window->width, 0.0f, (float)window->height, 0.0f, 1.0f);
	orthoView = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(bloom_combine, "projection"), 1, GL_FALSE, glm::value_ptr(orthoProjection));
	glUniformMatrix4fv(glGetUniformLocation(bloom_combine, "view"), 1, GL_FALSE, glm::value_ptr(orthoView));

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(window->width / 2.0f, window->height / 2.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(bloom_combine, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
	glUniform1i(glGetUniformLocation(bloom_combine, "scene"), 0); //Uniform is at texture unit 0.
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pingPongColorBuffers[0]);
	glUniform1i(glGetUniformLocation(bloom_combine, "bloomBlur"), 1); //Uniform is at texture unit 1.
	glUniform1i(glGetUniformLocation(bloom_combine, "bloomEnabled"), bloomEnabled); //Uniform is at texture unit 1.
	renderTarget->Draw();

	glFlush();

	//Check for any errors.
	GLuint error = glGetError();
	if (error != GL_NO_ERROR)
	{
		//DebugHelper::PrintFormat(_T("Found Error: %d\n"), error);
	}
}

void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	projection = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 100.0f);

	//Resize render target so FBO's are properly drawn.
	if (renderTarget != NULL)
	{
		delete renderTarget;
	}
	renderTarget = new Square(window->width, window->height);
}

void Destroy()
{
	delete sphere;
	delete sphere2;
	delete sphere3;
	delete sphere4;
	delete cuboid1;
	delete floor1;
	delete renderTarget;
	delete light1;
	delete light2;
	delete light3;
	delete light4;
	delete light5;
	delete light6;
	delete light7;
	delete light8;
}