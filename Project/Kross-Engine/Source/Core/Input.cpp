/*
 *  Author: Deklyn Palmer.
 *  Editors:
 *      - Deklyn Palmer.
 */

#include "Input.h"

namespace Kross
{
	Input* Input::m_Instance = nullptr;

	void Input::OnCreate()
	{
		if (!m_Instance)
		{
			m_Instance = KROSS_NEW Input();
		}
	}

	void Input::OnUpdate()
	{
		for (int i = 0; i < MaxControllerSlots(); i++)
		{
			bool connected = ControllerConnected(i);

			m_Instance->m_ControllerIDsConnected[i] = connected;

			if (connected)
			{
				/* Ask for what it's state currently is. */
				glfwGetGamepadState(i, m_Instance->m_GamepadStates[i]);
			}
		}
	}

	void Input::OnDestoy()
	{
		if (m_Instance)
		{
			m_Instance->m_Window = nullptr;
			delete m_Instance;
		}
	}

	void Input::SetWindow(Window* window)
	{
		m_Instance->m_Window = window;

		glfwSetScrollCallback(window->GetGLFWWindow(), ScrollCallback);
	};

	float Input::GetAxis(Axis axis)
	{
		/* Obtain the GLFW Window. */
		GLFWwindow* window = m_Instance->m_Window->GetGLFWWindow();

		/* Search which axis. */
		switch (axis)
		{
			/* If none has been specified. */
			case Axis::None:
			{
				// Return Zero. */
				return 0.0f;
			}
			/* The keys (A or <-) or (D or ->) */
			case Axis::KeyboardHorizontal:
			{
				/* If there is a window. */
				if (m_Instance->m_Window)
				{
					/* Grab the Left and Right Values. */
					float rightValue = glfwGetKey(window, (int)Key::D) + glfwGetKey(window, (int)Key::RightArrow);
					float leftValue = glfwGetKey(window, (int)Key::A) + glfwGetKey(window, (int)Key::LeftArrow);

					/* Return 1 being fully Right and -1 being fully Left. */
					return (float)(glm::sign(rightValue) - glm::sign(leftValue));
				}

				return 0.0f;
			}
			/* The keys (W or /\) or (S or \/) */
			case Axis::KeyboardVertical:
			{
				/* If there is a window. */
				if (m_Instance->m_Window)
				{
					/* Grab the Up and Down Values. */
					float upValue = (float)glfwGetKey(window, (int)Key::W) + (float)glfwGetKey(window, (int)Key::UpArrow);
					float downValue = (float)glfwGetKey(window, (int)Key::S) + (float)glfwGetKey(window, (int)Key::DownArrow);

					/* Return 1 being fully Up and -1 being fully Down. */
					return (float)(glm::sign(upValue) - glm::sign(downValue));
				}

				return 0.0f;
			}
			/* If something random has been specified. */
			default:
			{
				/* Return Zero. */
				return 0.0f;
			} 
		}
	}

	bool Input::GetKeyDown(Key key)
	{
		/* Obtain the GLFW Window. */
		GLFWwindow* window = m_Instance->m_Window->GetGLFWWindow();

		int result = glfwGetKey(window, (int)key);
		m_Instance->m_KeyStateCache[key] = result;

		/* Get whether or not the Key is down. */
		return (bool)result;
	}

	bool Input::GetKeyPressed(Key key)
	{
		// Obtain the GLFW Window. */
		GLFWwindow* window = m_Instance->m_Window->GetGLFWWindow();

		/* Get the Key status. */
		int keyPress = glfwGetKey(window, (int)key);

		/* Assume the Key is not pressed. */
		bool result = false;

		/* Check if it's last state was released and its pressed. */
		if (m_Instance->m_KeyStateCache[key] == GLFW_RELEASE && keyPress == GLFW_PRESS)
		{
			result = true; /* We have pressed the Key. */
		}

		/* Update the Cache. */
		m_Instance->m_KeyStateCache[key] = keyPress;
		 
		/* Return the Result. */
		return result;
	}

	bool Input::GetKeyReleased(Key key)
	{
		/* Obtain the GLFW Window. */
		GLFWwindow* window = m_Instance->m_Window->GetGLFWWindow();

		/* Get the Key status. */
		int keyRelease = glfwGetKey(window, (int)key);

		/* Assume the Key is not Released. */
		bool result = false;

		/* Check if it's last state was pressed and its released. */
		if (m_Instance->m_KeyStateCache[key] == GLFW_PRESS && keyRelease == GLFW_RELEASE)
		{
			result = true; /* We have released the Key. */
		}

		/* Update the Cache. */
		m_Instance->m_KeyStateCache[key] = keyRelease;

		/* Return the Result. */
		return result;
	}

	bool Input::GetMouseButtonDown(Mouse mouse)
	{
		/* Obtain the GLFW Window. */
		GLFWwindow* window = m_Instance->m_Window->GetGLFWWindow();

		int result = glfwGetMouseButton(window, (int)mouse);
		m_Instance->m_MouseStateCache[mouse] = result;

		return (bool)result; /* Get if the Mouse is down. */
	}

	bool Input::GetMouseButtonPressed(Mouse mouse)
	{
		/* Obtain the GLFW Window. */
		GLFWwindow* window = m_Instance->m_Window->GetGLFWWindow();

		/* Get the Mouse status. */
		int mousePress = glfwGetMouseButton(window, (int)mouse);

		/* Assume the Mouse Button is not Pressed. */
		bool result = false;

		/* Check if it's last state was released and its pressed. */
		if (m_Instance->m_MouseStateCache[mouse] == GLFW_RELEASE && mousePress == GLFW_PRESS)
		{
			result = true; /* We have pressed the Mouse Button. */
		}

		/* Update the Cache. */
		m_Instance->m_MouseStateCache[mouse] = mousePress;

		/* Return the Result. */
		return result;
	}

	bool Input::GetMouseButtonReleased(Mouse mouse)
	{
		/* Obtain the GLFW Window. */
		GLFWwindow* window = m_Instance->m_Window->GetGLFWWindow();

		/* Get the Mouse status. */
		int mouseRelease = glfwGetMouseButton(window, (int)mouse);

		/* Assume the Mouse Button is not Released. */
		bool result = false;

		/* Check if it's last state was pressed and its released. */
		if (m_Instance->m_MouseStateCache[mouse] == GLFW_PRESS && mouseRelease == GLFW_RELEASE)
		{
			result = true; /* We have released the Mouse Button. */
		}

		/* Update the Cache. */
		m_Instance->m_MouseStateCache[mouse] = mouseRelease;

		/* Return the Result. */
		return result;
	}

	inline float Input::GetMouseScroll()
	{
		return m_Instance->m_Scroll;
	}

	Vector2 Input::GetMousePosition()
	{
		/* Quick Variables for getting the mouse position. */
		double x, y;

		/* Obtain the GLFW Window. */
		GLFWwindow* window = m_Instance->m_Window->GetGLFWWindow();

		/* Grab the Mouse position in WINDOW PIXEL SPACE. */
		glfwGetCursorPos(window, &x, &y);

		/* Return the Position. */
		return Vector2((float)x, (float)y);
	}

	float Input::GetControllerAxis(int controllerID, Controller axis, float deadZone)
	{
		/* Ask if the Controller is Connected. */
		if (ControllerConnected(controllerID))
		{
			/* Search the Axis Specified. */
			switch (axis)
			{
				/* Left Stick Horizontal. */
				case Controller::LeftStickHorizontal:
				{
					/* If the Stick is moving outside of the Dead Zone return the value of the stick. */
					if (m_Instance->m_GamepadStates[controllerID]->axes[(int)Controller::LeftStickHorizontal] >= deadZone || m_Instance->m_GamepadStates[controllerID]->axes[(int)Controller::LeftStickHorizontal] <= -deadZone)
					{
						return m_Instance->m_GamepadStates[controllerID]->axes[(int)Controller::LeftStickHorizontal];
					}
					return 0.0f;
				}
				/* Left Stick Vertical. */
				case Controller::LeftStickVertical:
				{
					/* If the Stick is moving outside of the Dead Zone return the value of the stick. */
					if (m_Instance->m_GamepadStates[controllerID]->axes[(int)Controller::LeftStickVertical] >= deadZone || m_Instance->m_GamepadStates[controllerID]->axes[(int)Controller::LeftStickVertical] <= -deadZone)
					{
						return -m_Instance->m_GamepadStates[controllerID]->axes[(int)Controller::LeftStickVertical];
					}
					return 0.0f;
				}
				/* Right Stick Horizontal. */
				case Controller::RightStickHorizontal:
				{
					/* If the Stick is moving outside of the Dead Zone return the value of the stick. */
					if (m_Instance->m_GamepadStates[controllerID]->axes[(int)Controller::RightStickHorizontal] >= deadZone || m_Instance->m_GamepadStates[controllerID]->axes[(int)Controller::RightStickHorizontal] <= -deadZone)
					{
						return m_Instance->m_GamepadStates[controllerID]->axes[(int)Controller::RightStickHorizontal];
					}
					return 0.0f;
				}
				/* Right Stick Vertical. */
				case Controller::RightStickVertical:
				{
					/* If the Stick is moving outside of the Dead Zone return the value of the stick. */
					if (m_Instance->m_GamepadStates[controllerID]->axes[(int)Controller::RightStickVertical] >= deadZone || m_Instance->m_GamepadStates[controllerID]->axes[(int)Controller::RightStickVertical] <= -deadZone)
					{
						return -m_Instance->m_GamepadStates[controllerID]->axes[(int)Controller::RightStickVertical];
					}
					return 0.0f;
				}
				/* Left Trigger. */
				case Controller::LeftTrigger:
				{
					/* Get how far pushing in the trigger is. */
					return m_Instance->m_GamepadStates[controllerID]->axes[(int)Controller::LeftTrigger];
				}
				/* Right Trigger. */
				case Controller::RightTrigger:
				{
					/* Get how far pushing in the trigger is. */
					return m_Instance->m_GamepadStates[controllerID]->axes[(int)Controller::RightTrigger];
				}
				/* Nothing. */
				default:
				{
					/* Return Zero. */
					return 0.0f;
				}
			}
		}

		return 0.0f;
	}

	bool Input::GetControllerButtonDown(int controllerID, Controller button)
	{
		/* Check if the button is down or not. */
		return (bool)m_Instance->m_GamepadStates[controllerID]->buttons[(int)button];
	}

	bool Input::GetControllerButtonPressed(int controllerID, Controller button)
	{
		/* Get the Controller Button status. */
		int buttonPress = m_Instance->m_GamepadStates[controllerID]->buttons[(int)button];

		/* Assume the Controller Button is not pressed. */
		bool result = false;

		/* Check if it's last state was released and its pressed. */
		if (m_Instance->m_ControllerStateCache[button] == GLFW_RELEASE && buttonPress == GLFW_PRESS)
		{
			result = true; /* We have pressed the Controller Button. */
		}

		/* Update the Cache. */
		m_Instance->m_ControllerStateCache[button] = buttonPress;

		/* Return the Result. */
		return result;
	}

	bool Input::GetControllerButtonReleased(int controllerID, Controller button)
	{
		/* Get the Controller Button status. */
		int buttonRelease = m_Instance->m_GamepadStates[controllerID]->buttons[(int)button];

		/* Assume the Controller Button is not Released. */
		bool result = false;

		/* Check if it's last state was Pressed and its Released. */
		if (m_Instance->m_ControllerStateCache[button] == GLFW_PRESS && buttonRelease == GLFW_RELEASE)
		{
			result = true; /* We have Released the Controller Button. */
		}

		/* Update the Cache. */
		m_Instance->m_ControllerStateCache[button] = buttonRelease;

		/* Return the Result. */
		return result;
	}

	int Input::GetAvalibleController()
	{
		/* Run through all Controller Slots. */
		for (int i = 0; i < MaxControllerSlots(); ++i)
		{
			/* Check if the Controler is Connected. */
			if (ControllerConnected(i))
			{
				return i; /* Return the Slot ID. */
			}
		}

		/* Return NULL if none are connected. */
		return -1;
	}

	void ScrollCallback(GLFWwindow* window, double x, double y)
	{
		Input::SetScrollValue((float)y);
	};
}