#pragma once
#ifndef COMPONENTPOOL_HPP
#define COMPONENTPOOL_HPP

#include "Entity.hpp"
#include <cstddef>
#include <unordered_map>
#include <utility>
#include <vector>

namespace engine::ecs
{
	template<typename Component>
	class ComponentPool
	{
	public:
		Component&	add(Entity entity, Component component)
		{
			const auto	existing = m_entityToIndex.find(entity);

			if (existing != m_entityToIndex.end())
			{
				m_components[existing->second] = std::move(component);
				return m_components[existing->second];
			}

			const std::size_t	index = m_components.size();

			m_components.push_back(std::move(component));
			m_entities.push_back(entity);
			m_entityToIndex.emplace(entity, index);

			return m_components.back();
		}

		void	remove(Entity entity)
		{
			const auto	found = m_entityToIndex.find(entity);

			if (found == m_entityToIndex.end())
				return;

			const std::size_t	removedIndex = found->second;
			const std::size_t	lastIndex = m_components.size() - 1;

			if (removedIndex != lastIndex)
			{
				m_components[removedIndex] = std::move(m_components[lastIndex]);
				const Entity movedEntity = m_entities[lastIndex];
				m_entities[removedIndex] = movedEntity;
				m_entityToIndex[movedEntity] = removedIndex;
			}
			m_components.pop_back();
			m_entities.pop_back();
			m_entityToIndex.erase(entity);
		}

		Component*	tryGet(Entity entity)
		{
			const std::size_t*	index = findIndex(entity);

			return index != nullptr ? &m_components[*index] : nullptr;
		}

		const Component*	tryGet(Entity entity) const
		{
			const std::size_t*	index = findIndex(entity);

			return index != nullptr ? &m_components[*index] : nullptr;
		}

		template<typename Function>
		void	each(Function&& function)
		{
			eachComponent(m_components, m_entities, std::forward<Function>(function));
		}

		template<typename Function>
		void	each(Function&& function) const
		{
			eachComponent(m_components, m_entities, std::forward<Function>(function));
		}

	private:
		const std::size_t*	findIndex(Entity entity) const
		{
			const auto	found = m_entityToIndex.find(entity);

			return found != m_entityToIndex.end() ? &found->second : nullptr;
		}

		template<typename ComponentCollection, typename Function>
		static void	eachComponent(
			ComponentCollection& components,
			const std::vector<Entity>& entities,
			Function&& function
		)
		{
			for (std::size_t index = 0; index < components.size(); ++index)
				function(entities[index], components[index]);
		}

		std::vector<Component>					m_components;
		std::vector<Entity>						m_entities;
		std::unordered_map<Entity, std::size_t>	m_entityToIndex;
	};
}

#endif
