#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "ResourceHandle.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "Texture2D.h"

namespace GLFW
{
	class Material : GLFW::Resource
	{
	protected:
		virtual void deserialize(std::string& s);
		friend class ResourceManager;

	public:
		Material();
		Material(GLFW::Shader& shad);
		Material(float shini, glm::vec3 diff, glm::vec3 amb, glm::vec3 spec, GLFW::Shader& shad);
		~Material();
		float shininess;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		GLFW::Shader* shader;
		std::vector<GLFW::Texture2D*> texture;
		
		void use(bool flag = true);
		void setUniform(std::string name, bool value);
		void setUniform(std::string name, int value);
		void setUniform(std::string name, float value);
		void setUniform(std::string name, glm::vec2& value);
		void setUniform(std::string name, glm::vec3& value);
		void setUniform(std::string name, glm::vec4& value);
		void setUniform(std::string name, glm::mat2& value);
		void setUniform(std::string name, glm::mat3& value);
		void setUniform(std::string name, glm::mat4& value);
	};
}


