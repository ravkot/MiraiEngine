#include "stdafx.h"
#include "Material.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <rapidxml.hpp>

void GLFW::Material::deserialize(std::string & s)
{
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<>* root;
	std::vector<char> data(s.begin(), s.end());
	data.push_back('\0');
	doc.parse<0>(&data[0]);
	root = doc.first_node("Material");
	if (root == NULL)
	{
#ifdef _DEBUG
		std::cerr << "Material: Cannot deserialize: " << std::endl;
		std::cerr << "\t Error: No 'Material' header" << std::endl;
#endif
		return;
	}

	rapidxml::xml_node<>* uniform = root->first_node("Uniform");
	for (; uniform; uniform = uniform->next_sibling())
	{
		rapidxml::xml_attribute<>* attrName = uniform->first_attribute("name");
		if (attrName == NULL)
		{
#ifdef _DEBUG
			std::cerr << "Material: Cannot deserialize: " << std::endl;
			std::cerr << "\t Error: Missing 'Uniform' attribute" << std::endl;
#endif
		}

		std::string name = std::string(attrName->value());

		if (name == "shininess") 
		{
			this->shininess = (float)std::stof(uniform->value());
			continue;
		}

		glm::vec3 * v = NULL;
		if (name == "ambient")
			v = &this->ambient;
		else if (name == "diffuse")
			v = &this->diffuse;
		else if (name == "specular")
			v = &this->specular;
		else if (name == "texture")
		{
			std::cerr << "TODO: Dodac deserializacje tekstur dla materialow." << std::endl;
		}

		if (v == NULL)
		{
#ifdef _DEBUG
			std::cerr << "Material: Cannot deserialize: " << std::endl;
			std::cerr << "\t Error: Wrong uniform type '"+name+"'" << std::endl;
#endif
		}

		rapidxml::xml_node<>* param = uniform->first_node("Uniform");
		for (; param; param = param->next_sibling())
		{
			rapidxml::xml_attribute<>* attrRGB = param->first_attribute("name");
			if (attrRGB == NULL)
			{
#ifdef _DEBUG
				std::cerr << "Material: Cannot deserialize: " << std::endl;
				std::cerr << "\t Error: Missing 'Uniform' of "+name+" attribute" << std::endl;
#endif
				continue;
			}

			std::string uniAttr = std::string(attrRGB->value());
			if (uniAttr == "r")
				v->r = std::stof(param->value());
			else if (uniAttr == "g")
				v->g = std::stof(param->value());
			else if (uniAttr == "b")
				v->b = std::stof(param->value());
			else
			{
#ifdef _DEBUG
				std::cerr << "Material: Cannot deserialize: " << std::endl;
				std::cerr << "\t Error: Missing 'Uniform' of " + name + " attribute" << std::endl;
				continue;
#endif
			}
		}
		
	}
	
}

GLFW::Material::Material() : GLFW::Resource()
{
	this->shininess = 1.0f;
	this->diffuse = glm::vec3();
	this->ambient = glm::vec3();
	this->specular = glm::vec3();
	this->shader = NULL;
}

GLFW::Material::Material(GLFW::Shader & shad) : GLFW::Resource()
{
	this->shininess = 1.0f;
	this->diffuse = glm::vec3();
	this->ambient = glm::vec3();
	this->specular = glm::vec3();
	this->shader = &shad;
}

GLFW::Material::Material(float shini, glm::vec3 diff, glm::vec3 amb, glm::vec3 spec, GLFW::Shader& shad) : GLFW::Resource()
{
	this->shininess = shini;
	this->diffuse = diff;
	this->ambient = amb;
	this->specular = spec;
	this->shader = &shad;
}


GLFW::Material::~Material()
{
}

void GLFW::Material::use(bool flag)
{
	if (shader == NULL)
	{
#ifdef _DEBUG
		std::cerr << "Material: Cannot use material without shader" << std::endl;
#endif
		glUseProgram(0);
		return;
	}

	if (flag)
	{
		this->shader->use();
		int i = 0;
		for (auto tex : texture)
		{
			this->setUniform("tex" + std::to_string(i), i);
			tex->use();
			i++;
		}
	}
	else
	{
		this->shader->use(false);
		for (int i = texture.size() - 1; i >= 0; i--)
			this->texture[i]->use(false);
	}
}

void GLFW::Material::setUniform(std::string name, bool value)
{
	if (shader == NULL)
	{
#ifdef _DEBUG
		std::cerr << "Material: Cannot set uniform.\n\tError: Shader is NULL" << std::endl;
#endif
		return;
	}

	if (shader->getId() == 0 || shader->getId() == -1)
	{
#ifdef _DEBUG
		std::cerr << "Material: Cannot set uniform.\n\tError: Wrong shader Id" << std::endl;
#endif
		return;
	}

	int location = glGetUniformLocation(shader->getId(), name.c_str());
	glUniform1i(location, (int)value);
}

void GLFW::Material::setUniform(std::string name, int value)
{
	if (shader == NULL)
	{
#ifdef _DEBUG
		std::cerr << "Material: Cannot set uniform.\n\tError: Shader is NULL" << std::endl;
#endif
		return;
	}

	if (shader->getId() == 0 || shader->getId() == -1)
	{
#ifdef _DEBUG
		std::cerr << "Material: Cannot set uniform.\n\tError: Wrong shader Id" << std::endl;
#endif
		return;
	}

	int location = glGetUniformLocation(shader->getId(), name.c_str());
	glUniform1i(location, value);
}

void GLFW::Material::setUniform(std::string name, float value)
{
	if (shader == NULL)
	{
#ifdef _DEBUG
		std::cerr << "Material: Cannot set uniform.\n\tError: Shader is NULL" << std::endl;
#endif
		return;
	}

	if (shader->getId() == 0 || shader->getId() == -1)
	{
#ifdef _DEBUG
		std::cerr << "Material: Cannot set uniform.\n\tError: Wrong shader Id" << std::endl;
#endif
		return;
	}

	int location = glGetUniformLocation(shader->getId(), name.c_str());
	glUniform1f(location, value);
}

void GLFW::Material::setUniform(std::string name, glm::vec2 & value)
{
	if (shader == NULL)
	{
#ifdef _DEBUG
		std::cerr << "Material: Cannot set uniform.\n\tError: Shader is NULL" << std::endl;
#endif
		return;
	}

	if (shader->getId() == 0 || shader->getId() == -1)
	{
#ifdef _DEBUG
		std::cerr << "Material: Cannot set uniform.\n\tError: Wrong shader Id" << std::endl;
#endif
		return;
	}

	int location = glGetUniformLocation(shader->getId(), name.c_str());
	glUniform2fv(location, 1, &value[0]);
}

void GLFW::Material::setUniform(std::string name, glm::vec3 & value)
{
	if (shader == NULL)
	{
#ifdef _DEBUG
		std::cerr << "Material: Cannot set uniform.\n\tError: Shader is NULL" << std::endl;
#endif
		return;
	}

	if (shader->getId() == 0 || shader->getId() == -1)
	{
#ifdef _DEBUG
		std::cerr << "Material: Cannot set uniform.\n\tError: Wrong shader Id" << std::endl;
#endif
		return;
	}

	int location = glGetUniformLocation(shader->getId(), name.c_str());
	glUniform3fv(location, 1, &value[0]);
}

void GLFW::Material::setUniform(std::string name, glm::vec4 & value)
{
	if (shader == NULL)
	{
#ifdef _DEBUG
		std::cerr << "Material: Cannot set uniform.\n\tError: Shader is NULL" << std::endl;
#endif
		return;
	}

	if (shader->getId() == 0 || shader->getId() == -1)
	{
#ifdef _DEBUG
		std::cerr << "Material: Cannot set uniform.\n\tError: Wrong shader Id" << std::endl;
#endif
		return;
	}

	int location = glGetUniformLocation(shader->getId(), name.c_str());
	glUniform4fv(location, 1, &value[0]);
}

void GLFW::Material::setUniform(std::string name, glm::mat2 & value)
{
	if (shader == NULL)
	{
#ifdef _DEBUG
		std::cerr << "Material: Cannot set uniform.\n\tError: Shader is NULL" << std::endl;
#endif
		return;
	}

	if (shader->getId() == 0 || shader->getId() == -1)
	{
#ifdef _DEBUG
		std::cerr << "Material: Cannot set uniform.\n\tError: Wrong shader Id" << std::endl;
#endif
		return;
	}

	int location = glGetUniformLocation(shader->getId(), name.c_str());
	glUniformMatrix2fv(location, 1, GL_FALSE, &value[0][0]);
}

void GLFW::Material::setUniform(std::string name, glm::mat3 & value)
{
	if (shader == NULL)
	{
#ifdef _DEBUG
		std::cerr << "Material: Cannot set uniform.\n\tError: Shader is NULL" << std::endl;
#endif
		return;
	}

	if (shader->getId() == 0 || shader->getId() == -1)
	{
#ifdef _DEBUG
		std::cerr << "Material: Cannot set uniform.\n\tError: Wrong shader Id" << std::endl;
#endif
		return;
	}

	int location = glGetUniformLocation(shader->getId(), name.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
}

void GLFW::Material::setUniform(std::string name, glm::mat4 & value)
{
	if (shader == NULL)
	{
#ifdef _DEBUG
		std::cerr << "Material: Cannot set uniform.\n\tError: Shader is NULL" << std::endl;
#endif
		return;
	}

	if (shader->getId() == 0 || shader->getId() == -1)
	{
#ifdef _DEBUG
		std::cerr << "Material: Cannot set uniform.\n\tError: Wrong shader Id" << std::endl;
#endif
		return;
	}

	int location = glGetUniformLocation(shader->getId(), name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}


