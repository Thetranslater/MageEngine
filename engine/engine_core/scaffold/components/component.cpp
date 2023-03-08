#include"ui/widgets/widget.h"
#include"ui/widgets/treenode.h"

#include"engine_core/scaffold/components/component.h"

namespace Mage {
	std::shared_ptr<Widget> Component::Draw() {
		return CREATE_WIDGET(TreeNode);
	}
}