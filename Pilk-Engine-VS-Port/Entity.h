#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "Component.h"

#include <vector>
#include <string>

class Entity
{
public:
	/// Original Author : Piotr Moskala.
	/// <summary>
	/// Constructs an Entity.
	/// </summary>
	/// <param name="p_name"> The Entity's name. </param>
	Entity(const char* p_name) : m_name(p_name) 
	{

	}

	/// Original Author: Piotr Moskala.
	/// <summary>
	/// Returns the name of the Entity.
	/// </summary>
	const char* GetName()
	{
		return m_name;
	}

	/// Original Author: Piotr Moskala.
	/// <summary>
	/// Adds the specified Component pointer to the Entity's vector of Component pointers.
	/// </summary>
	/// <param name="p_component"> The Component pointer to add. </param>
	void AddComponent(Component* p_component)
	{
		m_components.push_back(p_component);
	}

	/// Original Author: Piotr Moskala.
	/// <summary>
	/// Removes a specific component from the entity and frees it from memory.
	/// </summary>
	/// <param name="p_component"> Component to remove. </param>
	void RemoveComponent(Component* p_component)
	{
		m_components.erase(std::remove(m_components.begin(), m_components.end(), p_component), m_components.end());
		delete p_component;
	}

	/// Original Author: Piotr Moskala.
	/// <summary>
	/// Returns the Component pointer of the specified type if one exists in the Entity.
	/// </summary>
	/// <typeparam name="T"> The type of Component to check for. </typeparam>
	/// <returns> The Component pointer of the specified type, or nullptr if there is no match. </returns>
	template<typename T>
	T* GetComponent()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			T* result = dynamic_cast<T*>(m_components[i]);
			if (result != nullptr)
			{
				return result;
			}
		}
		return nullptr;
	}

private:
	const char* m_name;
	std::vector<Component*> m_components;
};