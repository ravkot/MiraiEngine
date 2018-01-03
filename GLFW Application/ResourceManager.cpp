#include "stdafx.h"
#include "ResourceHandle.h"
#include "ResourceManager.h"

//template <class T>
//bool GLFW::ResourceManager::load(std::string path, std::string tag)
//{
//	static_assert(std::is_base_of<Resource, T>::value,
//		"ResourceManager: Illegal conversion of resource...");
//	file.open(path, std::ios_base::in);
//	if (!file.is_open())
//	{
//		std::string _class = typeid(this).name();
//		std::cerr << _class + ":\n\t[Error]: Couldn't open '" + path + "' file..." << std::endl;
//		file.close();
//		return false;
//	}
//
//	std::stringstream buffer;
//	buffer << file.rdbuf();
//	std::string contents(buffer.str());
//
//	T* c = new T();
//	Resource* r = c;
//	r->deserialize(contents);
//	if (holder.find(tag) != holder.end())
//	{
//		delete holder[tag];
//		holder.erase(tag);
//	}
//
//	this->holder[tag] = c;
//	file.close();
//
//	return true;
//}
//
//template <typename T>
//T* GLFW::ResourceManager::get(std::string tag)
//{
//	static_assert(std::is_base_of<Resource, T>::value,
//		"ResourceManager: Illegal conversion of resource...");
//	T* obj = (T*)holder[tag];
//	return obj;
//}
