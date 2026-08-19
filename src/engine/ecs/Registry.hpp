#pragma once
#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include "ComponentPool.hpp"
#include "Entity.hpp"

#include <tuple>
#include <utility>

namespace engine::ecs
{
	template<typename... Components>
	class Registry
	{
	public:
		Entity	createEntity()
		{
			return m_nextEntity++;
		}

		void	destroyEntity(Entity entity)
		{
			(getPool<Components>().remove(entity), ...);
		}

		template<typename Component>
		Component&	addComponent(Entity entity, Component component)
		{
			return getPool<Component>().add(entity, std::move(component));
		}

		template<typename Component>
		void	removeComponent(Entity entity)
		{
			getPool<Component>().remove(entity);
		}

		template<typename Component>
		Component*	tryGetComponent(Entity entity)
		{
			return getPool<Component>().tryGet(entity);
		}

		template<typename Component>
		const Component*	tryGetComponent(Entity entity) const
		{
			return getPool<Component>().tryGet(entity);
		}

		template<typename First, typename Second, typename Function>
		void	each(Function&& function)
		{
			eachMatching<First, Second>(*this, std::forward<Function>(function));
		}

		template<typename First, typename Second, typename Function>
		void	each(Function&& function) const
		{
			eachMatching<First, Second>(*this, std::forward<Function>(function));
		}

	private:
		template<typename First, typename Second, typename RegistryType, typename Function>
		static void	eachMatching(RegistryType& registry, Function&& function)
		{
			auto&	secondPool = registry.template getPool<Second>();

			registry.template getPool<First>().each(
				[&secondPool, &function](Entity entity, auto& first)
				{
					auto*	second = secondPool.tryGet(entity);

					if (second != nullptr)
						function(entity, first, *second);
				}
			);
		}

		template<typename Component>
		ComponentPool<Component>&	getPool()
		{
			return std::get<ComponentPool<Component>>(m_componentPools);
		}

		template<typename Component>
		const ComponentPool<Component>&	getPool() const
		{
			return std::get<ComponentPool<Component>>(m_componentPools);
		}

		Entity										m_nextEntity{1};
		std::tuple<ComponentPool<Components>...>	m_componentPools;
	};
}

#endif
