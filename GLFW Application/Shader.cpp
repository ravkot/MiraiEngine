#include "stdafx.h"
#include "Shader.h"
#include <Windows.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <rapidxml.hpp>
#include <cstdlib>
#include <iostream>
#include <regex>


int GLFW::Shader::COMPILATION_FAILURE = 0;


bool GLFW::Shader::compile()
{
	uint32_t shaderId[5] = { 0 };
	int success = 0;

	for (int i = 0; i < 5; i++)
	{
		if (source[i].length() == 0)
			continue;
		shaderId[i] = Shader::CompileShader((Shader::Type)i, this->source[i]);
		if (shaderId[i] == Shader::COMPILATION_FAILURE)
		{
#ifdef _DEBUG
			char errors[512];
			glGetProgramInfoLog(this->id, 512, NULL, errors);
			std::string _class = typeid(Shader).name();
			std::cerr << _class + ": Shader compilation failure (id="+std::to_string(i)+")\n\t";
			std::cerr << errors << std::endl;
			continue;
#endif
		}
	}

	for (int i = 0; i < 5; i++)
	{
		if (!shaderId[i])
			continue;
		glAttachShader(this->id, shaderId[i]);
	}
	
	glLinkProgram(this->id);
	glGetProgramiv(this->id, GL_LINK_STATUS, &success);

	if (success)
		return true;

#ifdef _DEBUG
		char errors[512];
		glGetProgramInfoLog(this->id, 512, NULL, errors);
		std::string _class = typeid(Shader).name();
		std::cout << _class + ": Program compilation failure\n\t";
		std::cout << errors << std::endl;
#endif

	return false;
}


void GLFW::Shader::use(bool flag)
{
	if (this->id == -1 || this->id == 0)
		return;

	if(flag)
		glUseProgram(this->id);
	else glUseProgram(0);
}

void GLFW::Shader::deserialize(std::string & s)
{
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<>* root;
	std::vector<char> data(s.begin(), s.end());
	data.push_back('\0');
	doc.parse<0>(&data[0]);

	root = doc.first_node("Shader");
	if (root == NULL)
	{
#ifdef _DEBUG
		std::cerr << "Shader '" << this->title << "' deserialization failure.(root)" << std::endl;
#endif
		return;
	}
	rapidxml::xml_attribute<>* attrTitle = root->first_attribute("title");
	if (attrTitle == NULL)
	{
#ifdef _DEBUG
		std::cerr << "Shader '" << this->title << "' deserialization failure." << std::endl;
#endif
		return;
	}
	
	this->title = attrTitle->value();
	rapidxml::xml_node<>* type = root->first_node("Type");
	if (type == NULL)
	{
#ifdef _DEBUG
		std::cerr << "Shader '" << this->title << "' deserialization failure." << std::endl;
#endif
		return;
	}

	for (; type; type = type->next_sibling())
	{
		rapidxml::xml_attribute<>* attrId = type->first_attribute("id");
		if (attrId == NULL)
		{
#ifdef _DEBUG
			std::cerr << "Shader '" << this->title << "' deserialization failure." << std::endl;
#endif
			return;
		}
		int id = std::stoi(std::string(attrId->value()));
		if (id > Shader::Type::Fragment || id < Shader::Type::Geometry)
		{
#ifdef _DEBUG
			std::cerr << "Shader '" << this->title << "'deserialization failure." << std::endl;
#endif
			return;
		}
		this->source[id] = std::string(type->value());
		
		this->source[id] = std::regex_replace(this->source[id], std::regex("&lt"), "<");
		this->source[id] = std::regex_replace(this->source[id], std::regex("&amp"), "&");
		this->source[id] = std::regex_replace(this->source[id], std::regex("&gt"), ">");
	} // for

	this->compile();
}

bool GLFW::Shader::isCompiled()
{
	return fCompiled;
}

int GLFW::Shader::getId()
{
	return this->id;
}

GLFW::Shader::Shader() : GLFW::Resource()
{
	this->id = glCreateProgram();
	this->fCompiled = false;
	this->title = "Shader:BlankName";
}

GLFW::Shader::~Shader()
{
	glDeleteProgram(this->id);
}

std::string GLFW::Shader::getSource(int id)
{
	return this->source[id];
}

int GLFW::Shader::CompileShader(Type t, std::string src)
{
	uint32_t shaderId = 0;
	GLint length = (uint32_t)src.length();
	if (length == 0)
		return Shader::COMPILATION_FAILURE;
	int type;
	const char* cstrSource = src.c_str();
	int success = 0;

	if (t == Shader::Type::Geometry)
		type = GL_GEOMETRY_SHADER;
	else if (t == Shader::Type::TCS)
		type = GL_TESS_CONTROL_SHADER;
	else if (t == Shader::Type::TES)
		type = GL_TESS_EVALUATION_SHADER;
	else if (t == Shader::Type::Vertex)
		type = GL_VERTEX_SHADER;
	else if (t == Shader::Type::Fragment)
		type = GL_FRAGMENT_SHADER;
	else
		return Shader::COMPILATION_FAILURE;

	shaderId = glCreateShader(type);
	glShaderSource(shaderId, 1, &cstrSource, &length);
	glCompileShader(shaderId);
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
#ifdef _DEBUG
		char errors[512];
		glGetShaderInfoLog(shaderId, 512, NULL, errors);
		std::string _class = typeid(Shader).name();
		std::cout << _class+": Compilation failure "+typeid(t).name()+"::"+std::to_string(t)+"\n\t";
		std::cout << errors << std::endl;
#endif
		glDeleteShader(shaderId);
		return Shader::COMPILATION_FAILURE;
	}

	return shaderId;
}

void GLFW::Shader::Init()
{
	glewInit();
}
