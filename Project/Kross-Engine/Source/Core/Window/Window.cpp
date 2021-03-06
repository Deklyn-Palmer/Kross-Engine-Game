/*
 *  Author: Deklyn Palmer.
 *  Editors:
 *      - Deklyn Palmer.
 */

#include "Window.h"

#include "../Manager/SceneManager.h"
#include "../Manager/ShaderManager.h"

namespace Kross
{
	Window::~Window()
	{
		delete m_Properties;
		
		if (m_GLFWWindow)
		{
			glfwDestroyWindow(m_GLFWWindow);
		}

		glfwTerminate();
	}

	void Window::OnStart(float r, float g, float b, float a)
	{
		float red, green, blue, alpha;

		/* Clamp the RGBA values from 1.0 to 0.0 */
		red = ((r < 0.0f) ? 0.0f : ((r > 1.0f) ? 1.0f : r));
		green = ((g < 0.0f) ? 0.0f : ((g > 1.0f) ? 1.0f : g));
		blue = ((b < 0.0f) ? 0.0f : ((b > 1.0f) ? 1.0f : b));
		alpha = ((a < 0.0f) ? 0.0f : ((a > 1.0f) ? 1.0f : a));

		/* Set Clear Colours and Refresh the window */
		glClearColor(red, green, blue, alpha);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Window::SetIcon(Texture* texture)
	{
		GLFWimage* icon = KROSS_NEW GLFWimage();
		icon->width = texture->GetWidth();
		icon->height = texture->GetHeight();
		icon->pixels = texture->GetPixelData();

		glfwSetWindowIcon(m_GLFWWindow, 1, icon);
	}

	void Window::OnInitialise()
	{
		/* Assume everything will be okay. */
		m_Initialised = true;

		/* Initialise GLFW */
		if (!glfwInit())
		{
			// Failed to intialise.
			m_Initialised = false;
			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_GLFWWindow = glfwCreateWindow(m_Properties->GetWidth(), m_Properties->GetHeight(), m_Properties->GetTitle().c_str(), NULL, NULL);

		/* Aspect Ratio Locking. */
		glfwSetWindowAspectRatio(m_GLFWWindow, 16, 9);

		if (!m_GLFWWindow)
		{
			// Failed to Create Window.
			m_Initialised = false;
			return;
		}

		/* Set Context */
		glfwMakeContextCurrent(m_GLFWWindow);

		/* Initialise GLEW */
		if (glewInit() != GLEW_OK)
		{
			// Failed to intialise.
			m_Initialised = false;
			return;
		}

		/* Enable Pixel Blending */
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		/* VSync Switch */
		glfwSwapInterval(m_Properties->GetVSync());
	}

	void Window::OnPollEvents()
	{
		/* Gets the Camera. */
		Camera* camera = SceneManager::GetScene()->GetCamera()->GetComponent<Camera>();

		/* Get the Current Width and Height of the Window */
		int Width, Height;
		glfwGetWindowSize(m_GLFWWindow, &Width, &Height);

		/* If the current width or height doesn't match the set width and height */
		if (Width != GetWidth() || Height != GetHeight())
		{
			/* Set Everything to the new Width and Height */
			glViewport(0, 0, Width, Height);
			SetWidth(Width);
			SetHeight(Height);
		}

		/* !FIX THIS! */
		#ifndef KROSS_EDITOR
		//float pPT = Height / (5.0f * BASE_SPRITE_WIDTH_AND_HEIGHT);
		////pPT = glm::round(pPT);
		//pPT = glm::floor(pPT);
		////pPT = glm::ceil(pPT);
		//float cameraHeight = Height / (BASE_SPRITE_WIDTH_AND_HEIGHT * pPT);
		//
		//SceneManager::GetScene()->GetCamera()->GetComponent<Camera>()->SetSize(cameraHeight);
		#endif

		/* Retrieves the Primary Monitor */
		GLFWmonitor* mainMonitor = glfwGetPrimaryMonitor();

		/* Grab the Monitor Size */
		glfwGetMonitorWorkarea(mainMonitor, NULL, NULL, &Width, &Height);

		/* If we haven't changed the Window mode */
		if (!m_Properties->ChangeWindowState())
		{
			/* Change the Window Mode based on the fullscreen flag */
			switch (GetFullscreen())
			{
				case 0:
				{
					/* Rest the Window View Port and Size. */
					glViewport(0, 0, 1280, 720);
					SetWidth(1280);
					SetHeight(720);

					/* Set the window to the middle of the screen */
					glfwSetWindowMonitor(m_GLFWWindow, NULL, (int)(((float)GetWidth() / (float)Width) * GetWidth()) - (int)(GetWidth() / 2), (int)(((float)GetHeight() / (float)Height) * GetHeight()) - (int)(GetHeight() / 2), GetWidth(), GetHeight(), GLFW_DONT_CARE);
					break;
				}
				case 1:
				{
					/* Set the window to the top left of the screen */
					glfwSetWindowMonitor(m_GLFWWindow, mainMonitor, 0, 0, Width, Height, GLFW_DONT_CARE);
					break;
				}
			}
			
			/* Initiate that we have changed the window mode */
			m_Properties->HasChangedWindowState();
		}

		/* Poll the Events */
		glfwSwapBuffers(m_GLFWWindow);
		glfwPollEvents();

		/* VSync Switch */
		glfwSwapInterval(m_Properties->GetVSync());
		
	}

	void Window::OnShutdown()
	{
		delete this;
	}
	const int Window::GetScreenRefreshRate() const
	{
		/* Grab the main Monitor and the Video Mode. */
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);

		/* Return the Refresh Rate. */
		return videoMode->refreshRate;
	}
}