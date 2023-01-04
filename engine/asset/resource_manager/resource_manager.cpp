#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include<asset/resource_manager/asset_type.h>
#include<asset/resource_manager/resource_manager.h>

#include<engine_core/platform/file_system.h>

#include<unordered_set>

namespace Mage {

	FSArguments global_fsargs{};

	bool ResourceManager::_custmization_readwholefile(std::vector<unsigned char>* _out_data, std::string* _err, const std::string& _filepath, void* _user_arg) {
		using namespace std;
		static unordered_set<std::string> extensions{ ".jpeg",".png",".jpg",".bin" };
		FSArguments* _user_args = static_cast<FSArguments*>(_user_arg);
		filesystem::path filepath(_filepath);

		auto postfix = filepath.extension();
		if (postfix == ".gltf" ||_user_args->_is_real_load) {
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
		return extensions.find(postfix.generic_string()) != extensions.end();
	}

	bool ResourceManager::loadMageModel(const std::string& filename, Model* out_model_data, std::string* err, std::string* warn, bool is_real_load) {
		FSArguments* pu = static_cast<FSArguments*>(fs.user_data);
		pu->_is_real_load = is_real_load;

		tinygltf::Model tiny_model;
		bool ret = gltf_loader.LoadASCIIFromFile(&tiny_model, err, warn, filename);
		out_model_data->loadFromgLTF_Model(tiny_model, filename);
		return ret;
	}

	bool ResourceManager::loadMageBuffer(const std::string& filename, Buffer* out_buffer_data, std::string* err, std::string* warn, bool is_real_load) {
		FSArguments* pu = static_cast<FSArguments*>(fs.user_data);
		pu->_is_real_load = is_real_load;

		bool ret = tinygltf::LoadExternalFile(&(out_buffer_data->m_data), err, warn, FileSystem::getFileName(filename), FileSystem::getParentPath(filename), false, 0, false, &fs);
		out_buffer_data->m_uri = filename;
		return ret;
	}

	bool ResourceManager::loadMageTexture(const std::string& filename, Texture* out_texture_data, std::string* err, std::string* warn, bool is_real_load) {
		if (is_real_load) {
			Sampler remain_sampler{ out_texture_data->m_combined_sampler };
			tinygltf::Image image;
			image.bits = 8;
			image.pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
			image.uri = filename;
			auto data = stbi_load(filename.data(), &image.width, &image.height, &image.component, 4);
			int size = image.width * image.height * 4 * (image.bits / 8);
			image.image.resize(size);
			image.image = std::vector<unsigned char>(data, data + size);
			//std::copy(data, data + size, image.image.begin());
			//stbi_image_free(data);

			out_texture_data->loadFromgLTF_Image(image);
			out_texture_data->m_combined_sampler = remain_sampler;

			return true;
		}
		return false;
	}
}