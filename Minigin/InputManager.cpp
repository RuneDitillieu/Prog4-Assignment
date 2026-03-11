#include <SDL3/SDL.h>
#include "InputManager.h"
#include <backends/imgui_impl_sdl3.h>

#pragma comment(lib, "xinput.lib")

dae::InputManager::InputManager()
{
#if defined(__EMSCRIPTEN__)
#else
	for (int idx{ 0 }; idx < 4; ++idx)
	{
		m_commandsController.emplace_back();
	}
#endif
}

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;

	while (SDL_PollEvent(&e)) 
	{
		if (e.type == SDL_EVENT_QUIT) 
		{
			return false;
		}

		for (const auto& commandBinding : m_commandsKeyboard)
		{
			if (e.type == static_cast<Uint32>(commandBinding.eventType)
				&& e.key.scancode == commandBinding.key)
			{
				commandBinding.command->Execute();
			}
		}
		// etc...
		
		//process event for IMGUI
		ImGui_ImplSDL3_ProcessEvent(&e);
	}

#if defined(__EMSCRIPTEN__)
	return true;
#else
	DWORD dwResult;
	for (DWORD controllerId = 0; controllerId < XUSER_MAX_COUNT; controllerId++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(controllerId, &state);

		if (dwResult == ERROR_SUCCESS)
		{
			for (const auto& commandBinding : m_commandsController[controllerId])
			{
				if (state.Gamepad.wButtons == commandBinding.button)
				{
					commandBinding.command->Execute();
				}
			}
		}
	}

	return true;
#endif
}

void dae::InputManager::BindCommand(std::unique_ptr<Command>&& command, SDL_Scancode scanCode, SDL_EventType eventType)
{
	auto it = std::find_if(m_commandsKeyboard.begin(), m_commandsKeyboard.end(), [&command](const CommandBindingKeyboard& commandBinding) { return commandBinding.command == command; });

	// if binding already exists, replace keybind
	if (it != m_commandsKeyboard.end())
	{
		it->key = scanCode;
		it->eventType = eventType;
	}
	// else create a new binding
	else
	{
		m_commandsKeyboard.emplace_back(CommandBindingKeyboard(std::move(command), scanCode, eventType));
	}
}

#if defined(__EMSCRIPTEN__)
#else
void dae::InputManager::BindCommand(std::unique_ptr<Command>&& command, SHORT button, int controllerId)
{
	auto it = std::find_if(m_commandsController[controllerId].begin(), m_commandsController[controllerId].end(), [&command](const CommandBindingController& commandBinding) { return commandBinding.command == command; });

	// if binding already exists, replace keybind
	if (it != m_commandsController[controllerId].end())
	{
		it->button = button;
	}
	// else create a new binding
	else
	{
		m_commandsController[controllerId].emplace_back(CommandBindingController(std::move(command), button));
	}
}
#endif
