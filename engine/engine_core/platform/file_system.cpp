#include<engine_core/platform/file_system.h>

namespace Mage {
	std::string FileSystem::getFilenameExtension(const std::string& filename) {
		using namespace std;

		filesystem::path file_path(filename);
		return file_path.extension().generic_string();
	}
}