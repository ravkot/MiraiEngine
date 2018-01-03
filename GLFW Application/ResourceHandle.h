#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Serializable.h"

namespace GLFW
{
	class Resource : public Serializable
	{
	protected:

		friend class ResourceManager;

	public:
		Resource();
		~Resource();
		virtual std::string getSource();
	};

}

