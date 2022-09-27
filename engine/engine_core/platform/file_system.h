#pragma once
#include<filesystem>

namespace Mage {
	//TODO: path and file
	class FileSystem {
	public:
		//get extension name
		static std::string getFilenameExtension(const std::string& filename);
	};
}