#pragma once
#include <string>
#include "ResourceHandle.h"


namespace GLFW
{
	class Shader : public GLFW::Resource
	{
	protected:
		uint32_t id;
		std::string title;
		std::string source[5];
		bool fCompiled;
		bool compile();
		
		virtual void deserialize(std::string & s);

		bool isCompiled();
		int getId();
		friend class Material;

	public:
		void use(bool flag = true);
		enum Type
		{
			Geometry,
			TCS, // Tessellation Control
			TES, // Tessellation Evaluation
			Vertex,
			Fragment
		};

		Shader();
		~Shader();
		virtual std::string getSource(int id);

		static int CompileShader(Type t, std::string src);
		static void Init();
		static int COMPILATION_FAILURE;

	};
	
}

