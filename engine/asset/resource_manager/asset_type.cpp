#include<assert.h>

#include<asset/resource_manager/asset_type.h>

#include<engine_core/render_engine/renderer/vulkanInfo.h>
#include<engine_core/render_engine/renderer/vulkanRHI.h>
#include<engine_core/render_engine/renderer/vulkanHelper.h>
#include<engine_core/platform/file_system.h>
#include<engine_core/render_engine/resource_swap_header.h>
#include<engine_core/render_engine/render_model.h>


namespace Mage {
	//buffer
	Buffer::Buffer(const Buffer& lbuffer) {
		m_data = lbuffer.m_data;
		m_uri = lbuffer.m_uri;
	}

	Buffer::Buffer(Buffer&& rbuffer) {
		m_data = std::move(rbuffer.m_data);
		m_uri = std::move(rbuffer.m_uri);
	}

	Buffer& Buffer::operator=(const Buffer& lbuffer) {
		m_data = lbuffer.m_data;
		m_uri = lbuffer.m_uri;
		return *this;
	}

	Buffer& Buffer::operator=(Buffer&& rbuffer) {
		m_data = std::move(rbuffer.m_data);
		m_uri = std::move(rbuffer.m_uri);
		return *this;
	}

	VkBuffer Buffer::asVulkanBuffer(VulkanRHI* rhi) {
		assert(!m_data.empty());

		VkBuffer buffer;
		VkDeviceMemory buffer_memory;
		VulkanHelper::bufferCreationHelper(
			rhi, static_cast<VkDeviceSize>(m_data.size()),
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
			buffer, buffer_memory);

		VulkanHelper::moveDataFromVectorToBuffer(rhi, m_data.begin(), m_data.end(), buffer);

		return buffer;
	}
	void Buffer::loadFromgLTF_Buffer(tinygltf::Buffer& buffer) {
		m_data = std::move(buffer.data);
		m_uri = std::move(buffer.uri);
	}


	//accessor
	void Accessor::loadFromgLTF_Accessor(tinygltf::Accessor& accessor) {
		m_buffer_view = accessor.bufferView;
		m_byte_offset = accessor.byteOffset;
		m_component_type = static_cast<ComponentType>(accessor.componentType);
		m_type = static_cast<Type>(accessor.type);
		m_count = accessor.count;

		m_max = std::move(accessor.maxValues);
		m_min = std::move(accessor.minValues);
	}
	constexpr int Accessor::getAccessBytes() const {
		int bytes{ 1 };
		switch (m_type)
		{
			case Type::SCALAR:
				bytes = 1;
				break;
			case Type::VEC2 || Type::VEC3 || Type::VEC4:
				bytes = static_cast<int>(m_type);
				break;
			case Type::MAT2:
				bytes = 4;
				break;
			case Type::MAT3:
				bytes = 9;
				break;
			case Type::MAT4:
				bytes = 16;
				break;
			case Type::NONE:
				return 0;
		}

		switch (m_component_type)
		{
		case Mage::Accessor::COMPONENT_NONE:
			return 0;
		case Mage::Accessor::SBYTE:
		case Mage::Accessor::UBYTE:
			bytes *= 1;
			break;
		case Mage::Accessor::SSHORT:
		case Mage::Accessor::USHORT:
			bytes *= 2;
			break;
		case Mage::Accessor::UINT:
		case Mage::Accessor::FLOAT:
			bytes *= 4;
			break;
		}
		return bytes;
	}

	//buffer view
	void BufferView::loadFromgLTF_BufferView(tinygltf::BufferView& buffer_view) {
		m_buffer		= buffer_view.buffer;
		m_byte_length	= buffer_view.byteLength;
		m_byte_offset	= buffer_view.byteOffset;
		m_byte_stride	= buffer_view.byteStride;

		m_target		= static_cast<Target>(buffer_view.target);
	}

	//sampler
	VkSampler Sampler::asVulkanSampler(VulkanRHI* rhi, VkSamplerCreateInfo* optional_info) {
		VkSampler sampler;

		VkSamplerCreateInfo sampler_create_info = VulkanInfo::aboutVkSamplerCreateInfo();
		sampler_create_info.magFilter	= static_cast<VkFilter>(m_magfilter);
		sampler_create_info.minFilter	= static_cast<VkFilter>(m_minfilter);
		sampler_create_info.mipmapMode	= static_cast<VkSamplerMipmapMode>(m_mipmapfilter);

		sampler_create_info.addressModeU = static_cast<VkSamplerAddressMode>(m_address_mode_u);
		sampler_create_info.addressModeV = static_cast<VkSamplerAddressMode>(m_address_mode_v);
		sampler_create_info.addressModeW = static_cast<VkSamplerAddressMode>(m_address_mode_w);

		if (optional_info != nullptr) {
			sampler_create_info.borderColor				= optional_info->borderColor;
			sampler_create_info.anisotropyEnable		= optional_info->anisotropyEnable;
			sampler_create_info.maxAnisotropy			= optional_info->maxAnisotropy;
			sampler_create_info.compareEnable			= optional_info->compareEnable;
			sampler_create_info.compareOp				= optional_info->compareOp;
			sampler_create_info.mipLodBias				= optional_info->mipLodBias;
			sampler_create_info.maxLod					= optional_info->maxLod;
			sampler_create_info.minLod					= optional_info->minLod;
			sampler_create_info.unnormalizedCoordinates = optional_info->unnormalizedCoordinates;
		}

		if (VK_SUCCESS != vkCreateSampler(rhi->getDevice(), &sampler_create_info, nullptr, &sampler)) {
			MAGE_THROW(failed to create sampler)
		}

		return sampler;
	}
	void Sampler::loadFromgLTF_Sampler(tinygltf::Sampler& gltf_sampler) {
		m_magfilter = static_cast<MageFilter>(gltf_sampler.magFilter - TINYGLTF_TEXTURE_FILTER_NEAREST);

		switch (gltf_sampler.minFilter)
		{
		case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST:
			m_minfilter = MageFilter::MAGE_FILTER_NEAREST;
			m_mipmapfilter = MageFilter::MAGE_FILTER_NEAREST;
			break;
		case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR:
			m_minfilter = MageFilter::MAGE_FILTER_NEAREST;
			m_mipmapfilter = MageFilter::MAGE_FILTER_LINEAR;
			break;
		case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST:
			m_minfilter = MageFilter::MAGE_FILTER_LINEAR;
			m_mipmapfilter = MageFilter::MAGE_FILTER_NEAREST;
			break;
		case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR:
			m_minfilter = MageFilter::MAGE_FILTER_LINEAR;
			m_mipmapfilter = MageFilter::MAGE_FILTER_LINEAR;
			break;
		}

		switch (gltf_sampler.wrapS) {
		case TINYGLTF_TEXTURE_WRAP_REPEAT:
			m_address_mode_u = MageSamplerAddressMode::MAGE_SAMPLER_ADDRESS_MODE_REPEAT;
			break;
		case TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT:
			m_address_mode_u = MageSamplerAddressMode::MAGE_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
			break;
		case TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE:
			m_address_mode_u = MageSamplerAddressMode::MAGE_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			break;
		}

		switch (gltf_sampler.wrapT)
		{
		case TINYGLTF_TEXTURE_WRAP_REPEAT:
			m_address_mode_v = MageSamplerAddressMode::MAGE_SAMPLER_ADDRESS_MODE_REPEAT;
			break;
		case TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT:
			m_address_mode_v = MageSamplerAddressMode::MAGE_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
			break;
		case TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE:
			m_address_mode_v = MageSamplerAddressMode::MAGE_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			break;
		}

		m_address_mode_w = MageSamplerAddressMode::MAGE_SAMPLER_ADDRESS_MODE_REPEAT;
	}

	//texture
	Texture::Texture(const Texture& ltexture) {
		m_sampler = -1;
		m_source = -1;
	}

	Texture& Texture::operator=(const Texture& ltexture) {
		m_sampler = ltexture.m_sampler;
		m_source = ltexture.m_source;
		
		return *this;
	}

	void Texture::loadFromGLTF_Texture(tinygltf::Texture& gltf_texture) {
		m_source = gltf_texture.source;
		m_sampler = gltf_texture.sampler;
	}

	//image
	void Image::loadFromGLTF_Image(tinygltf::Image& gltf_image) {
		m_width = gltf_image.width;
		m_height = gltf_image.height;
		m_component = gltf_image.component;
		m_bits = gltf_image.bits;
		m_pixel_type = gltf_image.pixel_type;

		m_image = std::move(gltf_image.image);
		m_uri = std::move(gltf_image.uri);
		m_mimeType = std::move(gltf_image.mimeType);

		m_bufferView = gltf_image.bufferView;

		if (gltf_image.component == 4) {
			switch (gltf_image.pixel_type)
			{
			case TINYGLTF_COMPONENT_TYPE_BYTE:
				m_format = MageFormat::MAGE_FORMAT_R8G8B8A8_SNORM;
				break;
			case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
				m_format = MageFormat::MAGE_FORMAT_R8G8B8A8_UNORM;
				break;
			case TINYGLTF_COMPONENT_TYPE_INT:
				m_format = MageFormat::MAGE_FORMAT_R32G32B32A32_SINT;
				break;
			case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
				m_format = MageFormat::MAGE_FORMAT_R32G32B32A32_UINT;
				break;
			case TINYGLTF_COMPONENT_TYPE_FLOAT:
				m_format = MageFormat::MAGE_FORMAT_R32G32B32A32_SFLOAT;
				break;
			default:
				m_format = MageFormat::MAGE_FORMAT_UNDEFINED;
			}
		}
		else if (gltf_image.component == 3) {
			switch (gltf_image.pixel_type)
			{
			case TINYGLTF_COMPONENT_TYPE_BYTE:
				m_format = MageFormat::MAGE_FORMAT_R8G8B8_SNORM;
				break;
			case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
				m_format = MageFormat::MAGE_FORMAT_R8G8B8A8_UNORM;
				break;
			case TINYGLTF_COMPONENT_TYPE_INT:
				m_format = MageFormat::MAGE_FORMAT_R32G32B32_SINT;
				break;
			case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
				m_format = MageFormat::MAGE_FORMAT_R32G32B32_UINT;
				break;
			case TINYGLTF_COMPONENT_TYPE_FLOAT:
				m_format = MageFormat::MAGE_FORMAT_R32G32B32_SFLOAT;
				break;
			default:
				m_format = MageFormat::MAGE_FORMAT_UNDEFINED;
			}
		}
		else if (gltf_image.component == 2) {
			switch (gltf_image.pixel_type)
			{
			case TINYGLTF_COMPONENT_TYPE_INT:
				m_format = MageFormat::MAGE_FORMAT_R32G32_SINT;
				break;
			case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
				m_format = MageFormat::MAGE_FORMAT_R32G32_UINT;
				break;
			default:
				m_format = MageFormat::MAGE_FORMAT_UNDEFINED;
			}
		}
		else {
			//MAGE_THROW(unsupport texture format)
			m_format = MageFormat::MAGE_FORMAT_UNDEFINED;
		}
	}

	//Material
	void Material::loadFromgLTF_Material(tinygltf::Material& material) {
		m_emissive_factor[0] = (float)material.emissiveFactor[0];
		m_emissive_factor[1] = (float)material.emissiveFactor[1];
		m_emissive_factor[2] = (float)material.emissiveFactor[2];

		m_pbr_metallic_roughness.m_base_color_factor[0]	= (float)material.pbrMetallicRoughness.baseColorFactor[0];
		m_pbr_metallic_roughness.m_base_color_factor[1]	= (float)material.pbrMetallicRoughness.baseColorFactor[1];
		m_pbr_metallic_roughness.m_base_color_factor[2]	= (float)material.pbrMetallicRoughness.baseColorFactor[2];
		m_pbr_metallic_roughness.m_base_color_factor[3]	= (float)material.pbrMetallicRoughness.baseColorFactor[3];
		m_pbr_metallic_roughness.m_metallic_factor		= (float)material.pbrMetallicRoughness.metallicFactor;
		m_pbr_metallic_roughness.m_roughness_factor		= (float)material.pbrMetallicRoughness.roughnessFactor;
		m_pbr_metallic_roughness.m_base_color_texture.m_index				= material.pbrMetallicRoughness.baseColorTexture.index;
		m_pbr_metallic_roughness.m_base_color_texture.m_texCoord			= material.pbrMetallicRoughness.baseColorTexture.texCoord;
		m_pbr_metallic_roughness.m_metallic_roughness_texture.m_index		= material.pbrMetallicRoughness.metallicRoughnessTexture.index;
		m_pbr_metallic_roughness.m_metallic_roughness_texture.m_texCoord	= material.pbrMetallicRoughness.metallicRoughnessTexture.texCoord;

		m_normal_texture.m_index	= material.normalTexture.index;
		m_normal_texture.m_texCoord = material.normalTexture.texCoord;
		m_normal_texture.m_scale	= material.normalTexture.scale;

		m_emissive_texture.m_index		= material.emissiveTexture.index;
		m_emissive_texture.m_texCoord	= material.emissiveTexture.texCoord;

		m_occlusion_texture.m_index		= material.occlusionTexture.index;
		m_occlusion_texture.m_texCoord	= material.occlusionTexture.texCoord;
		m_occlusion_texture.m_strength	= material.occlusionTexture.strength;

		m_double_side = material.doubleSided;
	}

	//primitive
	void Primitive::loadFromgLTF_Primitive(tinygltf::Primitive& primitive) {
		m_attributes = std::move(primitive.attributes);
		m_indices = primitive.indices;
		m_material = primitive.material;
		m_mode = primitive.mode;
	}

	//mesh
	void Mesh::loadFromgLTF_Mesh(tinygltf::Mesh& mesh) {
		m_primitives.resize(mesh.primitives.size());
		for (int i = 0; i < m_primitives.size(); ++i) {
			m_primitives[i].loadFromgLTF_Primitive(mesh.primitives[i]);
		}
	}

	//node
	void Node::loadFromgLTF_Node(tinygltf::Node& node) {
		m_camera = node.camera;
		m_mesh = node.mesh;

		m_children = std::move(node.children);
		//change double to float 
		std::copy(node.rotation.begin(), node.rotation.end(), std::back_inserter(m_rotation));
		std::copy(node.scale.begin(), node.scale.end(), std::back_inserter(m_scale));
		std::copy(node.translation.begin(), node.translation.end(), std::back_inserter(m_translation));
		std::copy(node.matrix.begin(), node.matrix.end(), std::back_inserter(m_matrix));

		//clear raw data
		node.rotation.clear();
		node.scale.clear();
		node.translation.clear();
		node.matrix.clear();

		m_parent = -1;
	}

	//model
	void Model::loadFromgLTF_Model(tinygltf::Model& model, const std::string& file) {
		//cut redundancy
		redundancyRemove(model);

		m_model_filepath = file;
		
		m_meshes.resize(model.meshes.size());
		m_accessors.resize(model.accessors.size());
		m_buffers.resize(model.buffers.size());
		m_buffer_views.resize(model.bufferViews.size());
		m_textures.resize(model.textures.size());
		m_nodes.resize(model.nodes.size());
		m_materials.resize(model.materials.size());
		m_samplers.resize(model.samplers.size());
		m_images.resize(model.images.size());

		for (int i = 0; i < m_meshes.size();				++i) m_meshes[i].loadFromgLTF_Mesh(model.meshes[i]);
		for (int i = 0; i < m_accessors.size();				++i) m_accessors[i].loadFromgLTF_Accessor(model.accessors[i]);
		for (int i = 0; i < m_buffers.size();				++i) m_buffers[i].loadFromgLTF_Buffer(model.buffers[i]);
		for (int i = 0; i < m_buffer_views.size();			++i) m_buffer_views[i].loadFromgLTF_BufferView(model.bufferViews[i]);
		for (int i = 0; i < m_images.size();				++i) m_images[i].loadFromGLTF_Image(model.images[i]);
		for (int i = 0; i < m_textures.size();				++i) m_textures[i].loadFromGLTF_Texture(model.textures[i]);
		for (int i = 0; i < m_materials.size();				++i) m_materials[i].loadFromgLTF_Material(model.materials[i]);
		for (int i = 0; i < m_samplers.size();				++i) m_samplers[i].loadFromgLTF_Sampler(model.samplers[i]);
		for (int i = 0; i < m_nodes.size();					++i) m_nodes[i].loadFromgLTF_Node(model.nodes[i]);
		for (int i = 0; i < m_nodes.size();					++i) {
			for (int j = 0; j < m_nodes[i].m_children.size(); ++j) {
				m_nodes[m_nodes[i].m_children[j]].m_parent = i;
			}
		}
		
		//TODO:process transform matrixs that attached to specific mesh
	}
	//TODO:重新写一遍，根据现有的增加的image结构
	VkRenderModelInfo Model::getVkRenderModelInfo() {
		std::string parent_directory = FileSystem::getParentPath(m_model_filepath);

		VkRenderModelInfo render_model_info;
		//render_model_info.m_go_id = m_go_id;

		auto& mesh_info = render_model_info.m_mesh_info;
		auto& images_info = render_model_info.m_images_info;
		auto& materials_info = render_model_info.m_materials_info;

		for (int i = 0; i < m_buffers.size(); ++i) {
			if (not m_buffers[i].m_data.empty()) {
				m_buffers[i].m_uri.clear();

				std::string combined_path_index = m_model_filepath + ":" + std::to_string(i);
				RawMeshData front_mesh;
				front_mesh.m_accessory = combined_path_index;
				front_mesh.m_raw = std::move(m_buffers[i]);

				mesh_info.m_infos[i] = std::move(front_mesh);
			}
			else {
				mesh_info.m_infos[i] = VkRenderMeshURI{parent_directory + "/" + m_buffers[i].m_uri};
			}
		}

		images_info.m_infos.resize(m_images.size());
		for (int i = 0; i < m_images.size(); ++i) {
			if (not m_images[i].m_image.empty()) {
				m_images[i].m_uri.clear();

				RawImageData rtd;
				rtd.m_accessory = m_model_filepath + ':' + std::to_string(i);
				rtd.m_raw = std::move(m_images[i]);

				images_info.m_infos[i].m_detail = std::move(rtd);
			}
			else {
				images_info.m_infos[i].m_detail = VkRenderImageURI{parent_directory + "/" + m_images[i].m_uri};
			}
		}
		for (auto& material : m_materials) {
			if (material.m_pbr_metallic_roughness.m_base_color_texture.m_index != -1) {
				images_info.m_infos[material.m_pbr_metallic_roughness.m_base_color_texture.m_index].is_srgb = true;
			}
		}

		materials_info.m_infos.resize(m_materials.size());
		for (int i{ 0 }; i < materials_info.m_infos.size(); ++i) {
			materials_info.m_infos[i].m_double_side = m_materials[i].m_double_side;
			materials_info.m_infos[i].m_base_color_texture_index = m_textures[m_materials[i].m_pbr_metallic_roughness.m_base_color_texture.m_index].m_source;
			materials_info.m_infos[i].m_metallic_roughness_texture_index = m_textures[m_materials[i].m_pbr_metallic_roughness.m_metallic_roughness_texture.m_index].m_source;
			materials_info.m_infos[i].m_normal_texture_index = m_textures[m_materials[i].m_normal_texture.m_index].m_source;
			materials_info.m_infos[i].m_occlusion_texture_index = m_textures[m_materials[i].m_occlusion_texture.m_index].m_source;

			//sampler
			if (m_textures[m_materials[i].m_pbr_metallic_roughness.m_base_color_texture.m_index].m_sampler != -1) {
				materials_info.m_infos[i].m_base_color_texture_sampler = m_samplers[m_textures[m_materials[i].m_pbr_metallic_roughness.m_base_color_texture.m_index].m_sampler];
			}
			if (m_textures[m_materials[i].m_normal_texture.m_index].m_sampler != -1) {
				materials_info.m_infos[i].m_normal_texture_sampler = m_samplers[m_textures[m_materials[i].m_normal_texture.m_index].m_sampler];
			}
			if (m_textures[m_materials[i].m_pbr_metallic_roughness.m_metallic_roughness_texture.m_index].m_sampler != -1) {
				materials_info.m_infos[i].m_metallic_roughness_texture_sampler = m_samplers[m_textures[m_materials[i].m_pbr_metallic_roughness.m_metallic_roughness_texture.m_index].m_sampler];
			}
			//TODO:增加其他的sampler
		}

		//meshes
		int sub_meshes_num{ 0 };
		for (auto& mesh : m_meshes)
			sub_meshes_num += mesh.m_primitives.size();
		mesh_info.m_transfer_mesh_descriptions.resize(sub_meshes_num);
		sub_meshes_num = 0;
		int vertex_off{ 0 };
		auto mesh_process = [&](const int& mesh_index, const Matrix4x4& matrix)->void {
			for (auto& primitive : m_meshes[mesh_index].m_primitives) {
				//TODO:目前是排除非index mesh情况，之后需要增加没有indices的情况
				Accessor* attribute_accessor = nullptr;
				Material* primitive_material = nullptr;

				//material
				if (primitive.m_material != -1) {
					primitive_material = &m_materials[primitive.m_material];
					mesh_info.m_transfer_mesh_descriptions[mesh_index].m_base_color_factor	= primitive_material->m_pbr_metallic_roughness.m_base_color_factor;
					mesh_info.m_transfer_mesh_descriptions[mesh_index].m_metallic_factor	= primitive_material->m_pbr_metallic_roughness.m_metallic_factor;
					mesh_info.m_transfer_mesh_descriptions[mesh_index].m_roughness_factor	= primitive_material->m_pbr_metallic_roughness.m_roughness_factor;

					mesh_info.m_transfer_mesh_descriptions[mesh_index].m_material_index = primitive.m_material;
				}
				else {
					//TODO:无material的primitive
				}

				//mesh
				uint32_t buffer_stride{ 0xffffffff }, buffer_offset{ 0xffffffff }, element_count{ 0xffffffff };
				auto get_attribute_info = [&](const std::string& attribute, int index) {
					auto& attribute_info = mesh_info.m_transfer_mesh_descriptions[mesh_index].m_attribute_infos[index];
					attribute_accessor = &m_accessors[primitive.m_attributes[attribute]];

					attribute_info.m_stride = m_buffer_views[attribute_accessor->m_buffer_view].m_byte_stride;
					attribute_info.m_offset = m_buffer_views[attribute_accessor->m_buffer_view].m_byte_offset + attribute_accessor->m_byte_offset;
					attribute_info.m_count = attribute_accessor->m_count;
					attribute_info.m_buffer_index = m_buffer_views[attribute_accessor->m_buffer_view].m_buffer;
				};
				//position
				if (primitive.m_attributes.find("POSITION") != primitive.m_attributes.end()) {
					get_attribute_info("POSITION", 0);
				}
				//normal
				if (primitive.m_attributes.find("NORMAL") != primitive.m_attributes.end()) {
					get_attribute_info("NORMAL", 1);
				}
				//tangent
				if (primitive.m_attributes.find("TANGENT") != primitive.m_attributes.end()) {
					get_attribute_info("TANGENT", 2);
				}
				//uv_0
				if (primitive.m_attributes.find("TEXCOORD_0") != primitive.m_attributes.end()) {
					get_attribute_info("TEXCOORD_0", 3);
				}
				//uv_1
				if (primitive.m_attributes.find("TEXCOORD_1") != primitive.m_attributes.end()) {
					get_attribute_info("TEXCOORD_1", 4);
				}
				//color_0
				if (primitive.m_attributes.find("COLOR_0") != primitive.m_attributes.end()) {
					get_attribute_info("COLOR_0", 5);
				}
				
				//indices
				if (primitive.m_indices != -1) {
					auto& indices_info = mesh_info.m_transfer_mesh_descriptions[mesh_index].m_attribute_infos[6];
					attribute_accessor = &m_accessors[primitive.m_indices];
					
					indices_info.m_stride = m_buffer_views[attribute_accessor->m_buffer_view].m_byte_stride == 0 ? attribute_accessor->getAccessBytes() : m_buffer_views[attribute_accessor->m_buffer_view].m_byte_stride;
					indices_info.m_offset = m_buffer_views[attribute_accessor->m_buffer_view].m_byte_offset + attribute_accessor->m_byte_offset;
					indices_info.m_count = attribute_accessor->m_count;
					indices_info.m_buffer_index = m_buffer_views[attribute_accessor->m_buffer_view].m_buffer;
				}

				//params
				mesh_info.m_transfer_mesh_descriptions[mesh_index].m_matrix = matrix;
			}
		};
		Matrix4x4 root_mat{ Matrix4x4::identity };
		processNode(m_nodes, 0, root_mat, std::function<void(const int&, const Matrix4x4&)>(mesh_process));

		return render_model_info;
	}

	void Model::processNode(const std::vector<Node>& nodes, int curr_index, const Matrix4x4& parent_matrix, const std::function<void(const int&, const Matrix4x4&)>& process_func) {
		auto& node = nodes[curr_index];

		//get local transform
		Matrix4x4 curr_matrix{ Matrix4x4::identity };
		if (node.m_matrix.size()) {
			curr_matrix.SetMatrix(node.m_matrix);
		}
		else if (node.m_translation.size() && node.m_rotation.size() && node.m_scale.size()) {
			curr_matrix.SetTRS(Vector3(node.m_translation), Quaternion(node.m_rotation), Vector3(node.m_scale));
		}

		curr_matrix *= parent_matrix;

		//process node
		if (node.m_mesh != -1) {
			process_func(node.m_mesh, curr_matrix);
		}
		//TODO:process other things

		//process children
		for (auto& child : node.m_children) {
			processNode(nodes, child, curr_matrix, process_func);
		}
	}
	//目前处理纹理的冗余索引数据
	void Model::redundancyRemove(tinygltf::Model& gltf_model) {
		std::unordered_map<uint32_t, uint32_t> img_tex_map;
		for (uint32_t i{ 0u }; i <= gltf_model.textures.size(); ++i) {
			if (img_tex_map.find(gltf_model.textures[i].source) != img_tex_map.end()) {
				int owner = img_tex_map[gltf_model.textures[i].source];
				for (auto& material : gltf_model.materials) {
					if (material.normalTexture.index == i) material.normalTexture.index = owner;
					else if (material.emissiveTexture.index == i) material.emissiveTexture.index = owner;
					else if (material.occlusionTexture.index == i) material.occlusionTexture.index = owner;
					else if (material.pbrMetallicRoughness.baseColorTexture.index == i) material.pbrMetallicRoughness.baseColorTexture.index = owner;
					else if (material.pbrMetallicRoughness.metallicRoughnessTexture.index == i) material.pbrMetallicRoughness.metallicRoughnessTexture.index = owner;
				}
			}
			else {
				img_tex_map[gltf_model.textures[i].source] = i;
			}
		}
	}
}