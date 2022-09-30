#pragma once
#include "Transform.h"
#include "Model.h"

#include <memory>

namespace Goss
{
	class GameObject
	{
		using ObjectId = unsigned int;
		explicit GameObject(const ObjectId objectId) : id{objectId}{}
		ObjectId id;

	public:
		static GameObject CreateGameObject()
		{
			static ObjectId currentId = 0;
			return GameObject {currentId++};
		}

		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject(GameObject&&) = default;
		GameObject& operator=(GameObject&&) = default;

		ObjectId GetId() const { return id; }

		std::shared_ptr<Model> model{};
		glm::vec3 color{};

		Transform transform{};
	};
}
