#include <SDL3/SDL.h>
#include "InputManager.h"
#include <backends/imgui_impl_sdl3.h>

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;

	while (SDL_PollEvent(&e)) 
	{
		if (e.type == SDL_EVENT_QUIT) 
		{
			return false;
		}
		/*if (e.type == SDL_EVENT_KEY_DOWN) 
		{
			for (const auto& commandBinding : m_commands)
			{
				if (e.key.scancode == commandBinding.key)
				{
					commandBinding.command->Execute();
				}
			}
		}
		if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) 
		{
			
		}*/

		for (const auto& commandBinding : m_commands)
		{
			if (e.key.scancode == commandBinding.key)
			{
				if (e.type == static_cast<Uint32>(commandBinding.triggerEvent))
					commandBinding.command->Execute();
				else if (e.type == static_cast<Uint32>(commandBinding.releaseEvent))
					commandBinding.command->Release();
			}
		}
		// etc...
		
		//process event for IMGUI
		ImGui_ImplSDL3_ProcessEvent(&e);
	}

	return true;
}

void dae::InputManager::BindCommand(std::unique_ptr<Command>&& command, SDL_Scancode scanCode, SDL_EventType triggerEvent, SDL_EventType releaseEvent)
{
	auto it = std::find_if(m_commands.begin(), m_commands.end(), [&command](const CommandBinding& commandBinding) { return commandBinding.command == command; });

	// if binding already exists, replace keybind
	if (it != m_commands.end())
	{
		it->key = scanCode;
		it->triggerEvent = triggerEvent;
		it->releaseEvent = releaseEvent;
	}
	// else create a new binding
	else
	{
		m_commands.emplace_back(CommandBinding(std::move(command), scanCode, triggerEvent, releaseEvent));
	}
}
