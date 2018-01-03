#include "stdafx.h"
#include "Serializable.h"


GLFW::Serializable::Serializable()
{
}


GLFW::Serializable::~Serializable()
{
}

std::string GLFW::Serializable::serialize()
{
	return std::string();
}

void GLFW::Serializable::deserialize(std::string & s)
{
}
