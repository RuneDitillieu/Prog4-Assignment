#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

#include <memory>
#include "SoundSystem.h"

namespace dae
{
	class ServiceLocator final
	{
	public:
		static SoundSystem& GetSoundSystem() { return *_ss_instance; }
		static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss) 
		{
			_ss_instance = ss == nullptr ? std::make_unique<NullSoundSystem>() : std::move(ss);
		}

	private:
		static std::unique_ptr<SoundSystem> _ss_instance;
	};
}

#endif // !SERVICELOCATOR_H