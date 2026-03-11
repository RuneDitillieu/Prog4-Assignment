#pragma once
#include "Singleton.h"
#include <memory>
#include "Commands.h"
#include "Windows.h"
#include "Xinput.h"

namespace dae
{
	struct CommandBindingKeyboard
	{
		std::unique_ptr<Command> command;
		SDL_Scancode key;
		SDL_EventType eventType;
	};

	struct CommandBindingController
	{
		std::unique_ptr<Command> command;
		SHORT button;
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		bool ProcessInput();
		void BindCommand(std::unique_ptr<Command>&& command, SDL_Scancode scanCode, SDL_EventType eventType);
		void BindCommand(std::unique_ptr<Command>&& command, SHORT button, int controllerId);

	private:
		std::vector<CommandBindingKeyboard> m_commandsKeyboard;
		std::vector<std::vector<CommandBindingController>> m_commandsController{};
	};

}
