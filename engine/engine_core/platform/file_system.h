#pragma once
#include<filesystem>

namespace Mage {
	//TODO: path and file
	class FileSystem {
	public:
		//get extension name
		static std::string getFilenameExtension(const std::string& filename);
		static std::string getParentPath(const std::string& path);
		static std::string getRelativePath(const std::string& des, const std::string& src);
	};
}