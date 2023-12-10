#pragma once

#include "Judas-Engine/Compute/ComputeShader.h"
#include "Judas-Engine/Platform/OpenGL/OpenGLRenderTexture.h"

#include <glm/glm.hpp>

typedef unsigned int GLenum;

namespace Judas_Engine
{
	class OpenGLComputeShader : public ComputeShader
	{
	public:
		OpenGLComputeShader(const std::string& filepath, Ref<RenderTexture2D> renderTexture);
		OpenGLComputeShader(const std::string& name, const std::string& src, Ref<RenderTexture2D> renderTexture);

		virtual const std::string GetName() override { return m_Name; }

		virtual void Dispatch() const override;

	private:
		std::string ReadFile(const std::string& filepath);
		void Compile(const std::string& src);

	private:
		std::string m_Name;
		unsigned int m_RendererID;	

		Ref<RenderTexture2D> m_RenderTexture;
	};
}