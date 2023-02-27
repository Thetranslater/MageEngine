#include"ui/editor_ui.h"

#include"engine_core/scaffold/components/transformcomponent/Transform_component.h"

namespace Mage {
	void TransformComponent::SetPosition(const Vector3& new_p) {
		world_position = new_p;
		position_write_buffer = new_p;
		write_type = TransformWriteType::WORLD_WRITE;

		position = (parent == nullptr) ? world_position : parent->worldToLocalMatrix() * new_p;
	}

	void TransformComponent::SetLocalPosition(const Vector3& new_p) {
		position = new_p;
		position_write_buffer = new_p;
		write_type = TransformWriteType::LOCAL_WRITE;

		world_position = (parent == nullptr) ? position : parent->localToWorldMatrix() * new_p;
	}

	void TransformComponent::WritePosition(const Vector3& new_p) {
		position_write_buffer = new_p;
		write_type = TransformWriteType::WORLD_WRITE;
	}

	void TransformComponent::WriteLocalPosition(const Vector3& new_p) {
		position_write_buffer = new_p;
		write_type = TransformWriteType::LOCAL_WRITE;
	}

	void TransformComponent::SetRotation(const Quaternion& new_q) {
		world_rotation = new_q;
		rotation_write_buffer = new_q;
		write_type = TransformWriteType::WORLD_WRITE;

		rotation = (parent == nullptr) ? world_rotation : Quaternion::Inverse(parent->Rotation()) * new_q;
	}
	
	void TransformComponent::SetLocalRotation(const Quaternion& new_q) {
		rotation = new_q;
		rotation_write_buffer = new_q;
		write_type = TransformWriteType::LOCAL_WRITE;

		world_rotation = parent == nullptr ? rotation : parent->Rotation() * new_q;
	}

	void TransformComponent::WriteRotation(const Quaternion& new_q) {
		rotation_write_buffer = new_q;
		write_type = TransformWriteType::WORLD_WRITE;
	}

	void TransformComponent::WriteLocalRotation(const Quaternion& new_q) {
		rotation_write_buffer = new_q;
		write_type = TransformWriteType::LOCAL_WRITE;
	}

	void TransformComponent::SetScale(const Vector3& new_s) {
		world_scale = new_s;
		scale_write_buffer = new_s;
		write_type = TransformWriteType::WORLD_WRITE;

		scale = parent == nullptr ? world_scale : new_s / parent->Scale();
	}

	void TransformComponent::SetLocalScale(const Vector3& new_s) {
		scale = new_s;
		scale_write_buffer = new_s;
		write_type = TransformWriteType::LOCAL_WRITE;

		world_scale = parent == nullptr ? scale : parent->Scale() * scale;
	}

	void TransformComponent::WriteScale(const Vector3& new_s) {
		scale_write_buffer = new_s;
		write_type = TransformWriteType::WORLD_WRITE;
	}

	void TransformComponent::WriteLocalScale(const Vector3& new_s) {
		scale_write_buffer = new_s;
		write_type = TransformWriteType::LOCAL_WRITE;
	}

	void TransformComponent::tick(float delta) {
		if (should_tick_in_editor) {
			position_write_buffer = position;
			rotation_write_buffer = rotation;
			scale_write_buffer = scale;
		}
		else {
			switch (write_type)
			{
			case Mage::WORLD_WRITE:
				SetPosition(position_write_buffer);
				SetRotation(rotation_write_buffer);
				SetScale(scale_write_buffer);
				break;
			case Mage::LOCAL_WRITE:
				SetLocalPosition(position_write_buffer);
				SetLocalRotation(rotation_write_buffer);
				SetLocalScale(scale_write_buffer);
				break;
			default:
				break;
			}
		}
		write_type = TransformWriteType::NONE_WRITE;
	}
}