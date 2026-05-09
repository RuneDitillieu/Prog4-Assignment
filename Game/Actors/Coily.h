#ifndef COILY_H
#define COILY_H

#include "Component.h"
#include "CoilyStates.h"

namespace QBert
{
    class GameObject;
	class Coily : public dae::Component
	{
    public:
        Coily(dae::GameObject* pOwner);
        ~Coily() = default;
        Coily(const Coily& other) = delete;
        Coily(Coily&& other) = delete;
        Coily& operator=(const Coily& other) = delete;
        Coily& operator=(Coily&& other) = delete;

        void Update() override;

        std::type_index GetType() const override;

    private:
        std::unique_ptr<CoilyState> m_pState{ std::make_unique<QBert::EggState>(GetOwner()) };
	};
}

#endif // !COILY_H