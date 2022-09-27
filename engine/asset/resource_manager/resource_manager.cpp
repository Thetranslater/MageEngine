#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include<asset/resource_manager/resource_manager.h>

namespace Mage {

	bool ResourceManager::loadModelFromFile(const std::string& filename, tinygltf::Model* out_model_data, std::string* err, std::string* warn) {
		return gltf_loader.LoadASCIIFromFile(out_model_data, err, warn, filename);
	}
}