#include<asset/resource_manager/asset_type.h>
#include<asset/resource_manager/resource_manager.h>


#include<engine_core/platform/file_system.h>

namespace Mage {

	bool ResourceManager::loadModelFromFile(const std::string& filename, Model* out_model_data, std::string* err, std::string* warn) {
		out_model_data->m_model_filepath = filename;
		tinygltf::Model gltf_model;
		if (!gltf_loader.LoadASCIIFromFile(&gltf_model, err, warn, filename)) {
			return false;
		}
		MAGE_TRY
		out_model_data->loadFromgLTF_Model(gltf_model, filename);
		MAGE_CATCH
		return true;
	}

	bool ResourceManager::loadBufferFromFile(const std::string& filename, Buffer* out_buffer_data, std::string* err, std::string* warn) {
	
	}
}