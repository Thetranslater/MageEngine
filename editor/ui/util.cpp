#include<string>

#include"core/math/quaternion.h"
#include"core/math/vector3.h"
#include"core/meta/reflection/reflection.h"

#include"ui/util.h"
#include"ui/widgets/group.h"
#include"ui/widgets/drag_float.h"
#include"ui/widgets/text.h"
#include"ui/widgets/sameline.h"

#include"ui/widgets/WFI/bindable.h"
#include"ui/widgets/WFI/resizable.h"

namespace Mage {
	namespace Util {
		std::shared_ptr<Widget> DrawVec3Widget(void* instance) {
			auto vector3 = Reflection::TypeMeta::newMetaFromName("Vector3");
			Reflection::FieldAccessor* vector3_field_list{ nullptr };
			int vector3_field_count = vector3.getFieldsList(vector3_field_list);

			ID last{ invalid_widget_id };
			auto attr_group = CREATE_WIDGET(Group);
			for (int i{ 0 }; i < vector3_field_count; ++i) {
				attr_group->addWidget(CREATE_WIDGET(Text, vector3_field_list[i].getFieldName()));
				attr_group->addWidget(CREATE_WIDGET(SameLine));
				auto drag = CREATE_WIDGET(DragFloat);
				if (instance != nullptr) {
					auto getter = [&](void* instance) ->float {
						return *static_cast<float*>(instance);
					};
					auto setter = [&](void* instance, float& val) {
						*static_cast<float*>(instance) = val;
					};
					auto drag_bindable = CREATE_WFI(Bindable<float>);
					drag_bindable->bind(std::bind(getter, vector3_field_list[i].get(instance)), std::bind(setter, vector3_field_list[i].get(instance), std::placeholders::_1));
					drag->addWFI(drag_bindable);
					drag->addWFI(CREATE_WFI(Resizable, 60.f));
				}
				attr_group->addWidget(drag);
				last = attr_group->addWidget(CREATE_WIDGET(SameLine));
			}
			attr_group->removeWidget(last);

			return attr_group;
		}

		std::shared_ptr<Widget> DrawQuatWidget(void* instance) {
			auto quaternion = Reflection::TypeMeta::newMetaFromName("Quaternion");
			Reflection::FieldAccessor* quaternion_field_list{ nullptr };
			int quaternion_field_count = quaternion.getFieldsList(quaternion_field_list);
			
			auto group = CREATE_WIDGET(Group);
			ID last_id{ invalid_widget_id };

#define QuatFieldBind(field) \
			group->addWidget(CREATE_WIDGET(Text, #field));\
			group->addWidget(CREATE_WIDGET(SameLine));\
			auto drag_##field## = CREATE_WIDGET(DragFloat); \
			drag_##field##->is_always_clamp = true; \
			auto getter_##field## = [](void* in) -> float { \
				float _##field## = static_cast<Quaternion*>(in)->ToEulerAngles().##field##; return _##field##; \
			}; \
			auto setter_##field## = [](void* in, float& value) { \
				Vector3 angle{ static_cast<Quaternion*>(in)->ToEulerAngles() }; \
				angle.##field## = value; \
				static_cast<Quaternion*>(in)->SetEulerAngels(angle); \
			}; \
			auto bind_##field## = CREATE_WFI(Bindable<float>); \
			bind_##field##->bind(std::bind(getter_##field##, instance), std::bind(setter_##field##, instance, std::placeholders::_1)); \
			drag_##field##->addWFI(bind_##field##); \
			drag_##field##->addWFI(CREATE_WFI(Resizable, 60.f)); \
			group->addWidget(drag_##field##);\
			last_id = group->addWidget(CREATE_WIDGET(SameLine));


			QuatFieldBind(x);
			QuatFieldBind(y);
			QuatFieldBind(z);
#undef QuatFieldBind

			group->removeWidget(last_id);

			return group;
		}

		std::string WString2String(const std::wstring& ws)
		{
			std::string strLocale = setlocale(LC_ALL, "");
			const wchar_t* wchSrc = ws.c_str();
			size_t nDestSize = 0ull;
			wcstombs_s(&nDestSize, NULL, 0, wchSrc, 0);
			char* chDest = new char[nDestSize];
			memset(chDest, 0, nDestSize);
			wcstombs_s(&nDestSize, chDest, nDestSize, wchSrc, nDestSize - 1);
			std::string strResult = chDest;
			delete[]chDest;
			setlocale(LC_ALL, strLocale.c_str());
			return strResult;
		}

		std::wstring String2WString(const std::string& s)
		{
			std::string strLocale = setlocale(LC_ALL, "");
			const char* chSrc = s.c_str();
			size_t nDestSize = 0ull;
			mbstowcs_s(&nDestSize, NULL, 0, chSrc, 0);
			wchar_t* wchDest = new wchar_t[nDestSize];
			wmemset(wchDest, 0, nDestSize);
			mbstowcs_s(&nDestSize, wchDest, nDestSize, chSrc, nDestSize - 1);
			std::wstring wstrResult = wchDest;
			delete[]wchDest;
			setlocale(LC_ALL, strLocale.c_str());
			return wstrResult;
		}
	}
}