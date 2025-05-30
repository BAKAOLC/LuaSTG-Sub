#pragma once
#include "GameResource/ResourceBase.hpp"
#include "core/implement/ReferenceCounted.hpp"

namespace luastg
{
	template<typename Base>
	class ResourceBaseImpl : public core::implement::ReferenceCounted<Base>
	{
	private:
		ResourceType m_resource_type;
		std::string m_resource_name;
	public:
		ResourceType GetType() const noexcept { return m_resource_type; }
		std::string_view GetResName() const noexcept { return m_resource_name; }
	public:
		ResourceBaseImpl(ResourceType t, std::string_view name) : m_resource_type(t), m_resource_name(name) {}
	};
}
