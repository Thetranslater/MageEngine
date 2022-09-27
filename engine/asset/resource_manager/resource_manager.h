#pragma once
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include<engine_core/platform/file_system.h>
#include<tinygITF/tiny_gltf.h>

namespace Mage {
	//gltf loader
	class ResourceManager {
	public:
		ResourceManager() :gltf_loader() {}
		~ResourceManager() = default;

		bool loadModelFromFile(const std::string& filename, tinygltf::Model* out_model_data, std::string* err, std::string* warn);
	private:
		tinygltf::TinyGLTF gltf_loader;
	};
}