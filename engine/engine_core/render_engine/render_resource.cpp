#include<engine_core/render_engine/render_resource.h>
#include<engine_core/render_engine/renderer/vulkanRHI.h>
#include<engine_core/render_engine/renderer/vulkanHelper.h>

namespace Mage {
	bool RenderResource::hasMesh(const GUID32& guid) {
		return m_guid_buffer_map.find(guid) != m_guid_buffer_map.end();
	}

	bool RenderResource::hasTexture(const GUID32& guid) {
		return m_guid_texture_map.find(guid) != m_guid_texture_map.end();
	}

	bool RenderResource::getOrCreateRenderResource(VulkanRHI* rhi, GUID32& guid, std::variant<VkRenderBuffer, std::vector<unsigned char>>& buffer) {
		if (hasMesh(guid)) {
			buffer = m_guid_buffer_map[guid];
			return true;
		}

		assert(not std::get<1>(buffer).empty());

		VkRenderBuffer render_buffer{};
		auto& data = std::get<1>(buffer);
		VulkanHelper::bufferCreationHelper(rhi, data.size(),
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			render_buffer.m_bi_data, render_buffer.m_bi_data_memory);

		VulkanHelper::moveDataFromVectorToBuffer(rhi, data.begin(), data.end(), render_buffer.m_bi_data);
		data.clear();
		buffer = render_buffer;
		m_guid_buffer_map.emplace(std::make_pair(guid, render_buffer));
		return true;
	}
}