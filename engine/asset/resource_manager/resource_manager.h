#pragma once
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include<tinygITF/tiny_gltf.h>

#include<asset/resource_manager/asset_type.h>

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
		bool loadAsset(const std::string& filename, Asset* out_model_data, std::string* err, std::string* warn, bool is_real_load = true);
	private:
		tinygltf::TinyGLTF gltf_loader;
		tinygltf::FsCallbacks fs{ &tinygltf::FileExists,&tinygltf::ExpandFilePath,&_custmization_readwholefile, &tinygltf::WriteWholeFile,&global_fsargs };

		//fs callbacks
		static bool _custmization_readwholefile(std::vector<unsigned char>* _out_data, std::string* _err, const std::string& _filepath, void* _user_arg);
	};
}