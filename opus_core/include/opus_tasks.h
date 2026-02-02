#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <functional>

namespace opus::tasks
{
    class Task
    {
    public:
        // Constructor and Destructor
        Task();
        Task(uint32_t modulo, uint32_t offset, bool enable, bool internal);
        virtual ~Task();

        // Life Cycle
        void Initialize();
        void Update(); // For Internal Counter
        void Update(uint64_t count); // For External Counter

        // Control
        void Enable();
        void Disable();
        void ResetCount();

        // State
        bool IsEnabled() const;
        bool IsInternal() const;
        bool IsInitialized() const;
        uint64_t Count() const;

    protected:
        virtual void OnInitialize(){}
        virtual void OnUpdate(uint64_t count) = 0;

    private:
        void UpdateImpl(uint64_t count, bool count_is_internal);
        bool m_enabled = false; // Enable Task
        bool m_internal = false; // Use Interal verse External (Parent Count)
        bool m_initialized = false; // Initialized
        uint32_t m_modulo = 1; // Task Modulo
        uint32_t m_offset = 0; // Task Offset
        uint64_t m_count = 0; // Internal Task Count
    };
}

namespace opus::tasks
{
    class TaskContainer : public Task
    {
    public:
        // Constructor and Destructor
        TaskContainer();
        TaskContainer(uint32_t modulo, uint32_t offset, bool enabled, bool internal);
        ~TaskContainer() override = default;

        // Control
        bool AddTask(Task& task);
        bool RemoveTask(Task& task);

    protected:
        void OnInitialize() override {}
        void OnUpdate(uint64_t count) override;

    private:
        std::vector<Task*> m_tasks;
    };
}


