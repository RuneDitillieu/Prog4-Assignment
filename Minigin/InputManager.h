#pragma once
#include "Singleton.h"
#include <memory>
#include "Commands.h"
#include "Windows.h"
#include "Xinput.h"

namespace dae
{
	struct CommandBinding
	{
		std::unique_ptr<Command> command;
		SDL_Scancode key;
		SDL_EventType triggerEvent;
		SDL_EventType releaseEvent;
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		void BindCommand(std::unique_ptr<Command>&& command, SDL_Scancode scanCode, SDL_EventType eventType, SDL_EventType releaseEvent);

	private:
		//std::unordered_map<std::unique_ptr<Command>, SDL_Scancode> m_commands;
		std::vector<CommandBinding> m_commands;
	};

}
