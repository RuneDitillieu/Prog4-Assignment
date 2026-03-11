#pragma once
#include "Singleton.h"
#include <memory>
#include "Commands.h"

#if defined(__EMSCRIPTEN__)
#else
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

#if defined(__EMSCRIPTEN__)
#else
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
		void BindCommand(std::unique_ptr<Command>&& command, SHORT button, int controllerId);

	private:
		std::vector<CommandBindingKeyboard> m_commandsKeyboard;

#if defined(__EMSCRIPTEN__)
#else
		std::vector<std::vector<CommandBindingController>> m_commandsController{};
#endif
	};

}
