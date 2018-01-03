#include "stdafx.h"
#include "ResourceHandle.h"

GLFW::Resource::Resource() : Serializable()
{
}

GLFW::Resource::~Resource()
{
}

std::string GLFW::Resource::getSource()
{
	return std::string();
}
