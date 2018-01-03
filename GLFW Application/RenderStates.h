#pragma once
#include <glm/glm.hpp>
#include "Material.h"

namespace GLFW
{
	class RenderStates
	{
	public:
		RenderStates();
		~RenderStates();
		//BlendMode blending;
		glm::mat4 translation;
		Material* material;

		enum Blending
		{
			Zero,
			One,
			SrcColor,
			OneMinusSrcColor,
			DstColor,
			OneMinusDstColor,
			SrcAlpha,
			OneMinusSrcAlpha,
			DstAlpha,
			OneMinusDstAlpha,
			ConstantColor,
			OneMinusConstantColor,
			ConstantAlpha,
			OneMinusConstantAlpha
		};
	};
}


