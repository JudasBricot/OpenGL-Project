#include <JudasEngine.h>

#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer/Shader.h"

#include "ImGui/imgui.h"

#ifdef GLM_ENABLE_EXPERIMENTAL
#else
	#define GLM_ENABLE_EXPERIMENTAL
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp>

// temp
#include "glm/ext.hpp"


class ExampleLayer : public Judas_Engine::Layer
{
public:
	ExampleLayer()
		: Layer("MyLayer"), m_CameraController(1280.0f / 720.0f, true)
	{
		/*m_VertexArray.reset(Judas_Engine::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};
		Judas_Engine::Ref<Judas_Engine::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(Judas_Engine::VertexBuffer::Create(vertices, sizeof(vertices)));

		Judas_Engine::BufferLayout layout = {
			{ Judas_Engine::ShaderDataType::Float3, "a_Position"},
			{ Judas_Engine::ShaderDataType::Float4, "a_Color"},
		};
		m_VertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		position = glm::mat4x4(1.0f); // Set identity
		//position = glm::translate(position, glm::vec3(0.0f, 0.0f, 0.0f));

		unsigned int indices[3] = {
			0, 1, 2
		};

		Judas_Engine::Ref<Judas_Engine::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(Judas_Engine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);*/

		m_BlueShaderVertexArray.reset(Judas_Engine::VertexArray::Create());

		float vertices2[5 * 4] = {
			-0.75f, -0.75f, 0.0f, 0.0f, 0.0f,
			 0.75f, -0.75f, 0.0f, 1.0f, 0.0f,
			 0.75f,  0.75f, 0.0f, 1.0f, 1.0f,
			-0.75f,  0.75f, 0.0f, 0.0f, 1.0f
		};

		Judas_Engine::Ref<Judas_Engine::VertexBuffer> vb;
		vb.reset(Judas_Engine::VertexBuffer::Create(vertices2, sizeof(vertices2)));

		Judas_Engine::BufferLayout layout2 = {
			{ Judas_Engine::ShaderDataType::Float3, "a_Position"},
			{ Judas_Engine::ShaderDataType::Float2, "a_TexCoords"}
		};
		vb->SetLayout(layout2);
		m_BlueShaderVertexArray->AddVertexBuffer(vb);

		unsigned int indices2[6] = {
			0, 1, 2,
			0, 2, 3
		};

		Judas_Engine::Ref<Judas_Engine::IndexBuffer> ib;
		ib.reset(Judas_Engine::IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t)));

		m_BlueShaderVertexArray->SetIndexBuffer(ib);
		bluePosition = glm::mat4x4(1.0f); // Set identity
		//bluePosition = glm::translate(position, glm::vec3(0.0f, 0.0f, 0.0f));

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoords;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			uniform vec3 u_Color;

			out vec2 v_TexCoords;

			void main()
			{
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
				v_TexCoords = a_TexCoords;
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			in vec2 v_TexCoords;		
			layout(location = 0) out vec4 color;

			void main()
			{
				color = vec4(v_TexCoords, 0.0, 1.0);
			}
		)";

		m_Shader = Judas_Engine::Shader::Create("VertexColorTriangle", vertexSrc, fragmentSrc);

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			uniform vec3 u_Color;

			out vec3 v_Color;

			out vec3 v_Position;

			void main()
			{
				v_Color = u_Color;
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			in vec3 v_Position;	
			in vec3 v_Color;		
			layout(location = 0) out vec4 color;

			void main()
			{
				color = vec4(v_Color, 1.0);
			}
		)";

		m_BlueShader = Judas_Engine::Shader::Create("BlueShader", blueShaderVertexSrc, blueShaderFragmentSrc);

		auto texture = m_ShaderLibrary.Load("Texture", "src/Assets/shaders/texture.glsl");

		m_Texture = Judas_Engine::Texture2D::Create("src/Assets/textures/texture.png");
		m_ChernoLogoTexture = Judas_Engine::Texture2D::Create("src/Assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(texture)->Bind();
		std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(texture)->UploadUniformInt("u_Texture", 0);
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("SquareColor", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnUpdate(Judas_Engine::Timestep ts) override
	{
		m_CameraController.OnUpdate(ts);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		Judas_Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Judas_Engine::RenderCommand::Clear();

		Judas_Engine::Renderer::BeginScene(m_CameraController.GetCamera());

		std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(m_BlueShader)->Bind();
		std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(m_BlueShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				glm::vec3 pos(i * 0.2f, j * 0.2f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				
				Judas_Engine::Renderer::Submit(m_BlueShader, m_BlueShaderVertexArray, transform);
			}
		}
		auto texture = m_ShaderLibrary.Get("Texture");
		m_Texture->Bind(0);
		Judas_Engine::Renderer::Submit(texture, m_BlueShaderVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_ChernoLogoTexture->Bind(0);
		Judas_Engine::Renderer::Submit(texture, m_BlueShaderVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Judas_Engine::Renderer::EndScene();
	}

	void OnEvent(Judas_Engine::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
private:
	Judas_Engine::ShaderLibrary m_ShaderLibrary;
	Judas_Engine::Ref<Judas_Engine::Texture2D> m_Texture, m_ChernoLogoTexture;

	Judas_Engine::Ref<Judas_Engine::Shader> m_Shader;
	Judas_Engine::Ref<Judas_Engine::VertexArray> m_VertexArray;
	glm::mat4x4 position;

	Judas_Engine::Ref<Judas_Engine::Shader> m_BlueShader, m_TextureShader;
	Judas_Engine::Ref<Judas_Engine::VertexArray> m_BlueShaderVertexArray;
	glm::mat4x4 bluePosition;

	Judas_Engine::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.1f, 0.8f };
};

class GameApplication : public Judas_Engine::Application
{
public:
	GameApplication()
	{
		PushLayer(new ExampleLayer());

		/*m_CameraLayer = new CameraLayer(&m_Camera);
		PushLayer(m_CameraLayer);*/
	}
	~GameApplication()
	{

	}
	private:
};

Judas_Engine::Application* Judas_Engine::CreateApplication()
{
	return new GameApplication();
}