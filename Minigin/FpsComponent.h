#pragma once

namespace dae
{
    class FpsComponent
    {
    public:
        FpsComponent();
        FpsComponent(const FpsComponent&) = delete;
        FpsComponent& operator=(const FpsComponent&) = delete;
        ~FpsComponent() = default;

        float GetCurrentFps(float deltaTime);
    };

}

