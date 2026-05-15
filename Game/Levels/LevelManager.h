#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "Component.h"
#include "IObserver.h"

namespace QBert
{
	class LevelManager : public dae::Component, public dae::IObserver
	{
	public:
		LevelManager(dae::GameObject* pOwner);
		~LevelManager() = default;
		LevelManager(const LevelManager& other) = delete;
		LevelManager(LevelManager&& other) = delete;
		LevelManager& operator=(const LevelManager& other) = delete;
		LevelManager& operator=(LevelManager&& other) = delete;

	private:

	};
}

#endif // !LEVELMANAGERCOMPONENT_H
