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

		template<typename First, typename Second, typename Function>
		void	each(Function&& function)
		{
			auto&	secondPool = getPool<Second>();

			getPool<First>().each(
				[&secondPool, &function](Entity entity, First& first)
				{
					Second*	second = secondPool.tryGet(entity);

					if (second != nullptr)
						function(entity, first, *second);
				}
			);
		}

	private:
		template<typename Component>
		ComponentPool<Component>&	getPool()
		{
			return std::get<ComponentPool<Component>>(m_componentPools);
		}

		Entity										m_nextEntity{1};
		std::tuple<ComponentPool<Components>...>	m_componentPools;
	};
}

#endif
