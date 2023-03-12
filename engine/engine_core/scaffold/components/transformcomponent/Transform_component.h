#pragma once

#include"core/math/vector3.h"
#include"core/math/quaternion.h"
#include"core/math/matrix4x4.h"

#include"ui/widgets/widget.h"

#include"engine_core/scaffold/components/component.h"

namespace Mage {
	enum TransformWriteType {
		NONE_WRITE = -1,
		WORLD_WRITE,
		LOCAL_WRITE
	};

	REFLECTION_TYPE(TransformComponent)
	CLASS(TransformComponent:public Component, WhiteListFields) {
		REFLECTION_BODY(TransformComponent)
	public:
		TransformComponent() :parent{ nullptr } { should_tick_in_editor = true; }

		void tick(float delta) override;

		std::shared_ptr<Widget> Draw() override;

		inline Vector3		Position() { return world_position; }
		inline Quaternion	Rotation() { return world_rotation; }
		inline Vector3		Scale() { return world_scale; }
		inline Vector3		LocalPosition() { return position; }
		inline Quaternion	LocalRotation() { return rotation; }
		inline Vector3		LocalScale() { return scale; }

		//attention: Because SetXXX function directly sets the properties of the transform, it may cause inconsistency.
		// Please use WriteXXX when you have the consistency requirement.
		//TODO:setÂß¼­ÖØÐ´
private:void SetPosition(const Vector3& new_p);
private:void SetRotation(const Quaternion& new_q);
private:void SetScale(const Vector3& new_s);
private:void SetLocalPosition(const Vector3& new_p);
private:void SetLocalRotation(const Quaternion& new_q);
private:void SetLocalScale(const Vector3& new_s);

		//TODO:writeÂß¼­ÖØÐ´
public: void WritePosition(const Vector3& new_p);
		void WriteRotation(const Quaternion& new_q);
		void WriteScale(const Vector3& new_s);
		void WriteLocalPosition(const Vector3& new_p);
		void WriteLocalRotation(const Quaternion& new_q);
		void WriteLocalScale(const Vector3& new_s);

		inline Matrix4x4 localToWorldMatrix() { return Matrix4x4::TRS(world_position, world_rotation, world_scale); }
		inline Matrix4x4 worldToLocalMatrix() { Matrix4x4 ret; Matrix4x4::Inverse3DAffine(Matrix4x4::TRS(world_position, world_rotation, world_scale), ret); return ret; }

		inline TransformComponent* GetParent() { return parent; }
		inline void SetParent(TransformComponent* p_transform) { parent = p_transform; p_transform->GetChildren().emplace_back(this); }

		inline TransformComponent* GetChild(int index) { return children.at(index); }
		inline std::vector<TransformComponent*>& GetChildren() { return children; }

		inline bool IsChildOf() { return parent != nullptr; }
		inline TransformComponent* Root() { if (parent != nullptr) return parent->Root(); return this; }
	protected:
		META(Enable)
		Vector3 position {Vector3::zero};
		META(Enable)
		Quaternion rotation {Quaternion::identity};
		META(Enable)
		Vector3 scale {Vector3::one};

		Vector3 world_position{ Vector3::zero };
		Quaternion world_rotation{ Quaternion::identity };
		Vector3 world_scale{ Vector3::one };

		Vector3 position_write_buffer{ Vector3::zero };
		Quaternion rotation_write_buffer{ Quaternion::identity };
		Vector3 scale_write_buffer{ Vector3::one };
		TransformWriteType write_type{ TransformWriteType::NONE_WRITE };

		//parent
		TransformComponent* parent;
		std::vector<TransformComponent*> children;
	};
}