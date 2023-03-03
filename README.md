# MageEngine
---
这是本人用于学习的3D游戏引擎，从vulkan的学习到现在也接近一年。整个项目由本人独自开发，这是在是一条艰苦的路。

###Feature
- PBR
- Editor UI (ImGui)
- Reflection & Serilization
- Vulkan
- glTF2.0

###Third Party
- Metaparser
- tinyglTF
- imgui
- vulkan & glfw
- json11

### Current Status
目前引擎支持以上特性。
PBR使用了前向渲染。
UI目前拥有Hierachy, Viewer, Inspector窗口，ImGui对组件进行了封装，增加了事件系统和WFI(widget-functionality interface 类似于插件系统，用于给组件添加自定义功能)。
支持读取glTF模型(实现了glTF的承诺——直接将模型二进制数据喂给GPU，主要是我观察vulkan-glTF的项目https://github.com/SaschaWillems/Vulkan-glTF-PBR，它好像是重新将数据整理了一遍。)
vulkan抽象出了渲染通道，应该能实现多通道渲染。

###Future
未来想各个系统分开成不同的模块项目，更容易管理和开发。
PBR使用延迟渲染。
完善editor和glTF读取(应对多种情况)
添加物理系统

###Special Thanks
Piccolo，Overload，Vulkan-glTF-PBR
---
###Notice
因为还不会cmake，所以我很难给出构建方法……我是直接用vs2022构建的，我能保证的就是让项目整体结构清晰，规范文件引用等，很抱歉，cmake真的太难学了
