#pragma once
#include <memory>
#include <stdexcept>
#include <source_location>
#include <iostream>
#include <format>

/// Fail-fast diagnostics for runtime-checked pointer wrappers.
class RuntimeCheckedPtrBase
{
    protected:
    static void handleNullDereference(const std::source_location& location)
    {
        throw std::runtime_error(std::format("Attempted to dereference a null pointer initialized in {} at line: {}", location.function_name(),
                                                 static_cast<int>(location.line())));
    }
};

template<typename T>
/// Ownership-preserving pointer that performs runtime null checks before each access.
class RuntimeCheckedPtr : private RuntimeCheckedPtrBase
{
    public:
    // Constructors
    RuntimeCheckedPtr(const std::source_location loc = std::source_location::current()) noexcept : m_ptr(nullptr), m_loc(loc)
    {}

    explicit RuntimeCheckedPtr(T* ptr, const std::source_location loc = std::source_location::current()) noexcept : m_ptr(ptr), m_loc(loc)
    {
        if (m_ptr == nullptr)
        {
            handleNullDereference(m_loc);
        }
    }

    // Move constructor
    RuntimeCheckedPtr(RuntimeCheckedPtr&& other) noexcept : m_ptr(std::move(other.m_ptr))
    {
        other.m_ptr = nullptr;
    }

    // Move assignment operator
    RuntimeCheckedPtr& operator=(RuntimeCheckedPtr&& other) noexcept
    {
        if (this != &other)
        {
            m_ptr       = std::move(other.m_ptr);
            other.m_ptr = nullptr;
        }
        return *this;
    }

    // Deleted copy constructor and assignment operator
    RuntimeCheckedPtr(const RuntimeCheckedPtr&)            = delete;
    RuntimeCheckedPtr& operator=(const RuntimeCheckedPtr&) = delete;

    // Destructor
    ~RuntimeCheckedPtr()
    {
        if (m_ptr)
        {
            delete m_ptr;
        }
    }

    // Dereference operator
    T& operator*() const
    {
        if (!m_ptr)
        {
            handleNullDereference(m_loc);
        }
        return *m_ptr;
    }

    // Member access operator
    T* operator->() const
    {
        if (!m_ptr)
        {
            handleNullDereference(m_loc);
        }
        return m_ptr;
    }

    // Get the raw pointer
    T* get() const noexcept
    {
        if (!m_ptr)
        {
            handleNullDereference(m_loc);
        }
        return m_ptr;
    }

    // Boolean conversion
    explicit operator bool() const noexcept
    {
        return static_cast<bool>(m_ptr);
    }

    private:
    T*                   m_ptr;
    std::source_location m_loc;
};
