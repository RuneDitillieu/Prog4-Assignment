#pragma once

namespace dae
{
    class TextComponent;
    class FpsComponent final
    {
    public:
        FpsComponent(TextComponent* owner);
        ~FpsComponent() = default;
        FpsComponent(const FpsComponent& other) = delete;
        FpsComponent(FpsComponent&& other) = delete;
        FpsComponent& operator=(const FpsComponent& other) = delete;
        FpsComponent& operator=(FpsComponent&& other) = delete;

        void Update(float deltaTime);

    private:
        TextComponent* m_owner;
    };
}

