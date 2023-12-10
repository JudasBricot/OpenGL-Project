#pragma once

#include <JudasEngine.h>

#include <imgui/imgui.h>

class PrimitiveTestLayer : public Judas_Engine::Layer
{
public:
	PrimitiveTestLayer()
		: Layer("MyLayer") {}

	virtual void OnAttach()
	{
		m_Shader = Judas_Engine::Shader::Create("src/Assets/shaders/colors.glsl");
		m_Mesh = Judas_Engine::Mesh::CreatePrimitive(Judas_Engine::Plane);
		m_Mesh->SetShader(m_Shader);
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");

		ImGui::End();
	}

	void OnUpdate(Judas_Engine::Timestep ts) override
	{
		m_Mesh->Submit();
	}
private:
	Judas_Engine::Ref<Judas_Engine::Shader> m_Shader;
	Judas_Engine::Ref <Judas_Engine::Mesh> m_Mesh;
};