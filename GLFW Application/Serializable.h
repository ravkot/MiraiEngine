#pragma once
#include <string>

namespace GLFW
{
	class Serializable
	{
	public:
		Serializable();
		~Serializable();

	protected:
		virtual std::string serialize();
		virtual void deserialize(std::string & s);
	};
}
