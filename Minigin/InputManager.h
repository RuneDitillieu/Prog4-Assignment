#pragma once
#include "Singleton.h"
#include <memory>
#include "Commands.h"

#ifndef __EMSCRIPTEN__
#include "Windows.h"
#include "Xinput.h"
#endif

namespace dae
{
	struct CommandBindingKeyboard
	{
		std::unique_ptr<Command> command;
		SDL_Scancode key;
		SDL_EventType eventType;
	};

#ifndef __EMSCRIPTEN__
	struct CommandBindingController
	{
		std::unique_ptr<Command> command;
		SHORT button;
	};
#endif

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		bool ProcessInput();
		void BindCommand(std::unique_ptr<Command>&& command, SDL_Scancode scanCode, SDL_EventType eventType);
#ifndef __EMSCRIPTEN__
		void BindCommand(std::unique_ptr<Command>&& command, SHORT button, int controllerId);
#endif

	private:
		std::vector<CommandBindingKeyboard> m_commandsKeyboard;

#ifndef __EMSCRIPTEN__
		std::vector<std::vector<CommandBindingController>> m_commandsController{};
#endif
	};

}
