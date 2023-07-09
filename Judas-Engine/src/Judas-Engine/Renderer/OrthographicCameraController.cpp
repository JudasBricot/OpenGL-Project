#include "jepch.h"
#include "OrthographicCameraController.h"

#include "Core/Core.h"

#include "Core/Input.h"
#include "Core/KeyCodes.h"

namespace Judas_Engine
{
    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation = false)
        : m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
    {

    }

    void OrthographicCameraController::OnUpdate(Timestep ts)
    {
        if (Input::IsKeyPressed(JE_KEY_W))
            m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
        else if (Input::IsKeyPressed(JE_KEY_S))
            m_CameraPosition.y += m_CameraTranslationSpeed * ts;
        if (Input::IsKeyPressed(JE_KEY_A))
            m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
        else if (Input::IsKeyPressed(JE_KEY_D))
            m_CameraPosition.x += m_CameraTranslationSpeed * ts;

        if (m_Rotation)
        {
            if (Input::IsKeyPressed(JE_KEY_Q))
                m_Rotation -= m_CameraRotationSpeed * ts;
            else if (Input::IsKeyPressed(JE_KEY_E))
                m_Rotation += m_CameraRotationSpeed * ts;
        }

        m_Camera.SetPosition(m_CameraPosition);
    }

    void OrthographicCameraController::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizedEvent>(BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
    {
        m_ZoomLevel -= e.GetYOffset();
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

        return true;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizedEvent& e)
    {
        m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

        return true;
    }
}
