#include "Input.h"

void Input::Initialize(GLFWwindow* const window_) noexcept
{
    glfwSetKeyCallback(window_, [](GLFWwindow*, int key_, int scancode_, int action_, int mods_) 
	{ 
		OnKeyInteracted(key_, scancode_, action_, mods_);
	});
	glfwSetMouseButtonCallback(window_, [](GLFWwindow*, int button_, int action_, int mods_) 
	{ 
		OnMouseButtonInteracted(button_, action_, mods_); 
	});
	glfwSetCursorPosCallback(window_, [](GLFWwindow*, double x_, double y_) 
	{ 
		OnMouseMoved(x_,  y_); 
	});
	glfwSetScrollCallback(window_, [](GLFWwindow*, double x_, double y_) 
	{ 
		OnMouseScrolled(x_, y_); 
	});
}

void Input::Update() noexcept
{
    lastKeyStates		  = nowKeyStates;
    lastMouseButtonStates = nowMouseButtonStates;
    lastMousePosition     = nowMousePosition;
    lastScrollOffset      = nowScrollOffset;
}

void Input::OnKeyInteracted(int key_, 
							int scancode_, 
							int action_, 
							int mods_) noexcept
{
    if (key_ < 0 || key_ >= GLFW_KEY_LAST)
	{
		return;
    }

    switch (action_)
    {
		case GLFW_PRESS:
		{
            nowKeyStates[key_] = true;
			break;
        }
		case GLFW_RELEASE:
		{
            nowKeyStates[key_] = false;
			break;
        }
        case GLFW_REPEAT: 
			[[fallthrough]];
        default:
        {
            break;
        }
    }
}

void Input::OnMouseButtonInteracted(int button_, 
									int action_, 
									int mods_) noexcept
{
	if (button_ < 0 || button_ >= GLFW_MOUSE_BUTTON_LAST)
	{
		return;
	}

	switch (action_)
	{
		case GLFW_PRESS:
		{
            nowMouseButtonStates[button_] = true;
			break;
		}
		case GLFW_RELEASE:
		{
            nowMouseButtonStates[button_] = false;
			break;
		}
        case GLFW_REPEAT:
            [[fallthrough]];
		default:
		{
			break;
		}
    }
}

void Input::OnMouseMoved(double x_, 
						 double y_) noexcept
{
	nowMousePosition = glm::vec2(static_cast<float>(x_), 
								 static_cast<float>(y_));
}

void Input::OnMouseScrolled(double x_, 
 						    double y_) noexcept
{
    nowScrollOffset = static_cast<float>(y_);
}

std::array<bool, Input::MAX_KEYS> Input::lastKeyStates = {false};

std::array<bool, Input::MAX_KEYS> Input::nowKeyStates = {false};

std::array<bool, Input::MAX_MOUSE_BUTTONS> Input::lastMouseButtonStates = {false};

std::array<bool, Input::MAX_MOUSE_BUTTONS> Input::nowMouseButtonStates = {false};

glm::vec2 Input::lastMousePosition = glm::vec2(0.0f);

glm::vec2 Input::nowMousePosition = glm::vec2(0.0f);

float Input::lastScrollOffset = 0.0f;

float Input::nowScrollOffset = 0.0f;