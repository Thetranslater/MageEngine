#include<engine_core/platform/file_system.h>

namespace Mage {
	std::string FileSystem::getFilenameExtension(const std::string& filename) {
		using namespace std;

		filesystem::path file_path(filename);
		return file_path.extension().generic_string();
	}
	
	std::string FileSystem::getParentPath(const std::string& path) {
		using namespace std;
		filesystem::path file_path(path);
		return file_path.parent_path().generic_string();
	}

	std::string FileSystem::getRelativePath(const std::string& des, const std::string& src) {
		using namespace std;
		filesystem::path des_path(des), src_path(src);
		return des_path.lexically_relative(src_path).generic_string();
	}
}