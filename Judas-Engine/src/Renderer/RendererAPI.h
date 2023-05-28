#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"

namespace Judas_Engine
{
	class RendererAPI
	{
	public:
		enum API { None = 0, OpenGL = 1 };

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		inline static RendererAPI::API GetAPI() { return s_API; }

	private:
		static RendererAPI::API s_API;
	};
}
