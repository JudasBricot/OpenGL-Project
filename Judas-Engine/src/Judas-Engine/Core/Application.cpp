#include "jepch.h"
#include "Application.h"

#include "Core.h"

#include "glm/glm.hpp"
#include <glm/ext/matrix_transform.hpp>

#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"

#include "Input.h"

#include <GLFW/glfw3.h>

namespace Judas_Engine
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		JE_CORE_ASSERT(!s_Instance, "Another application has already be created !")
		s_Instance = this;

		Log::Init();
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();
		Renderer::EnableBackFaceCulling();
		Renderer::EnableDepthTest();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizedEvent>(BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.begin(); it != m_LayerStack.end();)
		{
			(*it)->OnEvent(e);
			it++;
			if (e.handled)
				break;
		}
	}

	bool Application::OnWindowClose(WindowClosedEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizedEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime(); // Platform/Windows
			Timestep timestep = time - m_LastTime;
			m_LastTime = time;

			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
				{
					layer->OnUpdate(timestep);
				}
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}

			m_ImGuiLayer->End();
			m_Window->OnUpdate();
		}
	}
}
