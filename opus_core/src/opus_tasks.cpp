#include "opus_tasks.h"

// Class Task
namespace opus::tasks
{
    Task::Task() = default;

    Task::Task(uint32_t modulo, uint32_t offset, bool enable, bool internal)
        : m_enabled(enable),
          m_internal(internal),
          m_modulo(modulo == 0 ? 1u : modulo),
          m_offset(0),
          m_count(0)
    {
        m_offset = (m_modulo > 0) ? (offset % m_modulo) : 0u;
    }

    Task::~Task() = default;

    void Task::Initialize()
    {
        if (m_initialized)
            return;

        OnInitialize();
        m_initialized = true;
    }

    void Task::Update()
    {
        // Internal counter mode: uses m_count as the scheduling count
        UpdateImpl(m_count, true);
    }

    void Task::Update(uint64_t count)
    {
        // External/parent counter mode: uses caller-provided count
        UpdateImpl(count, false);
    }

    void Task::UpdateImpl(uint64_t count, bool useInternal)
    {
        if (!m_enabled)
            return;

        if (!m_initialized)
            Initialize();

        // Select internal or external Task Count
        uint64_t taskCount = count;
        if (m_internal && !useInternal)
            taskCount = m_count;

        // Gate by modulo/offset
        if (((taskCount + m_offset) % m_modulo) == 0)
        {
            OnUpdate(taskCount);

            // Increment count if using internal
            if (m_internal)
                ++m_count;
        }
    }

    void Task::Enable()  { m_enabled = true; }
    void Task::Disable() { m_enabled = false; }
    void Task::ResetCount() { m_count = 0; }
    bool Task::IsEnabled() const { return m_enabled; }
    bool Task::IsInternal() const { return m_internal; }
    bool Task::IsInitialized() const { return m_initialized; }
    uint64_t Task::Count() const { return m_count; }
}

// Clase Task Container 
namespace opus::tasks
{
    TaskContainer::TaskContainer()
        : Task(1, 0, true, false) // default: every tick, external by default
    {
    }

    TaskContainer::TaskContainer(uint32_t modulo, uint32_t offset, bool enabled, bool internal)
        : Task(modulo, offset, enabled, internal)
    {
    }

    bool TaskContainer::AddTask(Task& task)
    {
        auto it = std::find(m_tasks.begin(), m_tasks.end(), &task);
        if (it != m_tasks.end())
            return false;

        m_tasks.push_back(&task);
        return true;
    }

    bool TaskContainer::RemoveTask(Task& task)
    {
        auto it = std::remove(m_tasks.begin(), m_tasks.end(), &task);
        if (it == m_tasks.end())
            return false;

        m_tasks.erase(it, m_tasks.end());
        return true;
    }

    void TaskContainer::OnUpdate(uint64_t count)
    {
        for (Task* t : m_tasks)
        {
            if (t)
                t->Update(count);
        }
    }
}
