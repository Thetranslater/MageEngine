#include"ui/windows/inspector.h"
#include"ui/widgets/group.h"

#include"engine_core/scaffold/game_object.h"

namespace Mage {
	Inspector::Inspector(const std::string& id) :Window(id) {}

	Inspector::Inspector(const std::string& id, const WindowConfig& config) : Window(id, config) {}

	void Inspector::OnSelected(GameObject& selected) {
		widgets.clear();

		//header
		std::shared_ptr<Group> header = std::make_shared<Group>();

	}

	void Inspector::draw() {

	}
}