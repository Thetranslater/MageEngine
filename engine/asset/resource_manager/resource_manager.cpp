#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include<asset/resource_manager/asset_type.h>
#include<asset/resource_manager/resource_manager.h>


#include<engine_core/platform/file_system.h>

namespace Mage {

	FSArguments global_fsargs{};

	bool ResourceManager::_custmization_readwholefile(std::vector<unsigned char>* _out_data, std::string* _err, const std::string& _filepath, void* _user_arg) {
		FSArguments* _user_args = static_cast<FSArguments*>(_user_arg);
		if (_user_args->_is_real_load) {
			using namespace std;
			
			filesystem::path filepath(_filepath);
			ifstream file(filepath, ios::binary);
			if (!file.is_open()) {
				*_err += " Can't open file: " + _filepath + "\n";
				return false;
			}
			file.seekg(0, ios::end);
			_out_data->resize(file.tellg());
			if (_out_data->empty()) {
				*_err += " Empty file! \n";
				return false;
			}
			file.seekg(0, ios::beg);
			file.read(reinterpret_cast<char*>(_out_data->data()), _out_data->size());
			return true;
		}
		return false;
	}

	template<>
	bool ResourceManager::loadAsset<Model>(const std::string& filename, Model* out_model_data, std::string* err, std::string* warn, bool is_real_load) {
		FSArguments* pu = static_cast<FSArguments*>(fs.user_data);
		pu->_is_real_load = is_real_load;

		tinygltf::Model tiny_model;
		bool ret = gltf_loader.LoadASCIIFromFile(&tiny_model, err, warn, filename);
		out_model_data->loadFromgLTF_Model(tiny_model, filename);
		return ret;
	}

	template<>
	bool ResourceManager::loadAsset<Buffer>(const std::string& filename, Buffer* out_model_data, std::string* err, std::string* warn, bool is_real_load) {
		FSArguments* pu = static_cast<FSArguments*>(fs.user_data);
		pu->_is_real_load = is_real_load;

		bool ret = tinygltf::LoadExternalFile(&(out_model_data->m_data), err, warn, FileSystem::getFileName(filename), FileSystem::getParentPath(filename), false, 0, false, &fs);
		out_model_data->m_uri = filename;
		return ret;
	}
}