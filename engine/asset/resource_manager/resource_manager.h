#pragma once
#include<asset/resource_manager/resource_header.h>

namespace Mage {
	class Model;
	class Buffer;
	//gltf loader
	class ResourceManager {
	public:
		ResourceManager() :gltf_loader() {}
		~ResourceManager() = default;

		bool loadModelFromFile(const std::string& filename, Model* out_model_data, std::string* err, std::string* warn);
		bool loadBufferFromFile(const std::string& filename, Buffer* out_buffer_data, std::string* err, std::string* warn);
	private:
		tinygltf::TinyGLTF gltf_loader;
	};
}