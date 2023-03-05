#include"ui/editor_ui.h"

#include"engine_core/scaffold/components/transformcomponent/Transform_component.h"

#include"ui/widgets/table.h"
#include"ui/widgets/treenode.h"
#include"ui/widgets/text.h"
#include"ui/widgets/sameline.h"
#include"ui/widgets/drag_float.h"
#include"ui/widgets/group.h"

#include"ui/widgets/WFI/resizable.h"
#include"ui/widgets/WFI/bindable.h"

#include"ui/util.h"

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
		position_write_buffer = new_p - world_position;
		write_type = TransformWriteType::WORLD_WRITE;
	}

	void TransformComponent::WriteLocalPosition(const Vector3& new_p) {
		position_write_buffer = new_p - position;
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

	std::shared_ptr<Widget> TransformComponent::Draw() {
		auto node = CREATE_WIDGET(TreeNode, "Transform");

		auto ui_table = std::make_shared<Table<3, 2>>();
		ui_table->is_size_fixed_fit = true;

		//position draw
		{
			auto position_resizable = CREATE_WFI(Resizable, 20.f);
			auto position_text = CREATE_WIDGET(Text, "Position");
			position_text->addWFI(position_resizable);
			ui_table->operator[](0)[0] = position_text;

			ui_table->operator[](0)[1] = Util::DrawVec3Widget(&position);
		}

		//quaternion draw
		{
			auto rotation_resizable = CREATE_WFI(Resizable, 20.f);
			auto rotation_text = CREATE_WIDGET(Text, "Rotation");
			rotation_text->addWFI(rotation_resizable);
			ui_table->operator[](1)[0] = rotation_text;

			ui_table->operator[](1)[1] = Util::DrawQuatWidget(&rotation);
		}

		//scale draw
		{
			auto scale_resizable = CREATE_WFI(Resizable, 20.f);
			auto scale_text = CREATE_WIDGET(Text, "Scale");
			scale_text->addWFI(scale_resizable);
			ui_table->operator[](2)[0] = scale_text;

			ui_table->operator[](2)[1] = Util::DrawVec3Widget(&scale);
		}

		node->addWidget(ui_table);
		return node;
	}
}