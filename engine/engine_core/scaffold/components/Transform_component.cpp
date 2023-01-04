#include"engine_core/scaffold/components/Transform_component.h"

namespace Mage {
	void TransformComponent::tick(float delta) {
		if (should_tick_in_editor) {
			//transform的更新来源有：editor用户编辑的更新，rigidbody在物理引擎下控制的更新，脚本系统带来的更新
		}
	}
}