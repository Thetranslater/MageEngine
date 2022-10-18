#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include<asset/resource_manager/resource_manager.h>

#include<asset/resource_manager/asset_type.h>

namespace Mage {

	bool ResourceManager::loadModelFromFile(const std::string& filename, Model* out_model_data, std::string* err, std::string* warn) {
		out_model_data->m_model_filepath = filename;
		tinygltf::Model gltf_model;
		if (!gltf_loader.LoadASCIIFromFile(&gltf_model, err, warn, filename)) {
			return false;
		}
		out_model_data->loadFromgLTF_Model(gltf_model);
		return true;
	}
}