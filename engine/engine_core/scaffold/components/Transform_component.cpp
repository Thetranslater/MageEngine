#include"engine_core/scaffold/components/Transform_component.h"

namespace Mage {
	void TransformComponent::tick(float delta) {
		if (should_tick_in_editor) {
			//transform�ĸ�����Դ�У�editor�û��༭�ĸ��£�rigidbody�����������¿��Ƶĸ��£��ű�ϵͳ�����ĸ���
		}
	}
}