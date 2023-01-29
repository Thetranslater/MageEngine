#pragma once
#include<sstream>
#include<fstream>
#include<filesystem>

#define TINYGLTF_NO_STB_IMAGE_WRITE
#include<tinygITF/tiny_gltf.h>

#include<asset/resource_manager/asset_type.h>

#include"core/meta/serializer/serializer.h"
#include"core/_generated/serializer/all_serializer.h"

namespace Mage {

	struct FSArguments {
		bool _is_real_load;
	};

	extern FSArguments global_fsargs;
	//gltf loader
	class ResourceManager {
	public:
		ResourceManager() :gltf_loader() { gltf_loader.SetFsCallbacks(fs); }
		~ResourceManager() = default;

		void setFsCallbacks(tinygltf::FsCallbacks& fscb) { fs = fscb; }

		//改成模板函数,返回asset type
		template<typename Asset>
		bool loadAsset(const std::string& filename, Asset* out_model_data, std::string* err, std::string* warn, bool is_real_load = true) {
			if (is_real_load) {
				//filename must absolute
				std::ifstream json_file(filename);
				if (!json_file.is_open()) {
					MAGE_THROW(failed to open json load file)
				}
				std::stringstream ss;
				ss << json_file.rdbuf();
				std::string json_string(ss.str());

				std::string local_err;
				auto&& _json = json11::Json::parse(json_string, local_err);
				if (err != nullptr && !local_err.empty()) {
					*err = std::move(local_err);
					return false;
				}
				PSerializer::read(_json, *out_model_data);
				return true;
			}
			return false;
		}

		bool loadMageModel(const std::string& filename, Model* out_model, std::string* err, std::string* warn, bool is_real_load = true);
		bool loadMageBuffer(const std::string& filename, Buffer* out_buffer, std::string* err, std::string* warn, bool is_real_load = true);
		bool loadMageImage(const std::string& filename, Image* out_image, std::string* err, std::string* warn, bool is_real_load = true);

	private:
		tinygltf::TinyGLTF gltf_loader;
		tinygltf::FsCallbacks fs{ &tinygltf::FileExists,&tinygltf::ExpandFilePath,&_custmization_readwholefile, &tinygltf::WriteWholeFile,&global_fsargs };

		//fs callbacks
		static bool _custmization_readwholefile(std::vector<unsigned char>* _out_data, std::string* _err, const std::string& _filepath, void* _user_arg);
	};
}