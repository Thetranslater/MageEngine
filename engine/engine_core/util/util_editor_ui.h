#pragma once

#define NOMINMAX
#include<ShlObj.h>
#include<locale>
#include<assert.h>

#include<ui/widgets/widget.h>

namespace Mage {
	namespace Util {

#define STR(s) #s
#define MakeExtension(ex) STR(*ex*)
#define MakeFileTypeFilter(collection, extension) std::make_pair(#collection, MakeExtension(extension))

		using FileTypeFilters = std::vector<std::pair<std::string, std::string>>;

		std::shared_ptr<Widget> DrawVec3Widget(
			void* instance = nullptr, 
			std::function<float(void*)> getter = [](void* instance){return *static_cast<float*>(instance); }, 
			std::function<void(void*, float&)> setter = [](void* instance, float& val) {*static_cast<float*>(instance) = val; });

		std::shared_ptr<Widget> DrawQuatWidget(void* instance = nullptr);

		std::string WString2String(const std::wstring& ws);

		std::wstring String2WString(const std::string& s);

		inline void OpenFileDialog(std::string& out, const FileTypeFilters& types) {
			std::vector<std::pair<std::wstring, std::wstring>> convert(types.size());
			for (int i{ 0 }; i < types.size(); ++i) {
				convert[i] = std::make_pair(String2WString(types[i].first), String2WString(types[i].second));
			}

#define CHECK(CALLER) assert(SUCCEEDED(CALLER))
			CHECK(CoInitialize(NULL));

			IFileDialog* fd_ptr{ NULL };
			CHECK(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&fd_ptr)));
			COMDLG_FILTERSPEC* _types = new COMDLG_FILTERSPEC[types.size()];
			for (int i{ 0 }; i < types.size(); ++i) {
				_types[i].pszName = convert[i].first.c_str();
				_types[i].pszSpec = convert[i].second.c_str();
			}
			CHECK(fd_ptr->SetFileTypes(types.size(), _types));
			CHECK(fd_ptr->SetFileTypeIndex(1));
			if (SUCCEEDED(fd_ptr->Show(NULL))) {
				IShellItem* result_ptr;
				CHECK(fd_ptr->GetResult(&result_ptr));
				LPWSTR filename{ NULL };
				CHECK(result_ptr->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &filename));
				std::wstring trans{ filename };
				out = WString2String(trans);
				result_ptr->Release();
			}
			delete[] _types;
			fd_ptr->Release();

			CoUninitialize();
#undef CHECK
		}
	}
}