#include"ui/editor_ui.h"

#include"engine_core/scaffold/components/transformcomponent/Transform_component.h"

namespace Mage {
	void TransformComponent::tick(float delta) {
		if (should_tick_in_editor) {
			//transform�ĸ�����Դ�У�editor�û��༭�ĸ��£�rigidbody�����������¿��Ƶĸ��£��ű�ϵͳ�����ĸ���
			position_write_buffer = position;
			rotation_write_buffer = rotation;
			scale_write_buffer = scale;
		}
	}
}