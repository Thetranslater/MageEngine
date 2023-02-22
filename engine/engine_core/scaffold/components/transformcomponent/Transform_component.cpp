#include"ui/editor_ui.h"

#include"engine_core/scaffold/components/transformcomponent/Transform_component.h"

namespace Mage {
	void TransformComponent::tick(float delta) {
		if (should_tick_in_editor) {
			//transform的更新来源有：editor用户编辑的更新，rigidbody在物理引擎下控制的更新，脚本系统带来的更新
			position_write_buffer = position;
			rotation_write_buffer = rotation;
			scale_write_buffer = scale;
		}
	}
}