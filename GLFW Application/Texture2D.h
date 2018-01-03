#pragma once
#include "ResourceHandle.h"
#include "ResourceManager.h"

namespace GLFW
{
	class Texture2D : GLFW::Resource
	{
	public:
		Texture2D();
		~Texture2D();
		static const int TEXTURE_DISABLED;
		enum WrapMode
		{
			Repeat = 0x2901,
			MirroredRepeat = 0x8370,
			ClampToEdge = 0x812F,
			ClampToBorder = 0x812D
		};

		enum FilteringMode
		{
			Linear = 0x2601,
			Nearest = 0x2600,
			MipmapNN = 0x2700, // GL_NEAREST_MIPMAP_NEAREST
			MipmapLN = 0x2701, // GL_LINEAR_MIPMAP_NEAREST
			MipmapNL = 0x2702,
			MipmapLL = 0x2703
		};

		void use(bool flag = true);

	protected:
		friend class ResourceManager;
		int id;
		int activationId;
		int width;
		int height;
		int channels;
		unsigned char* pixels;

		
		static unsigned int numberOfUsed;

		virtual void deserialize(std::string & s);
		void invertY();
	};
}
