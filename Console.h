#pragma once

#include <Windows.h>

class Console final {
public:
    ~Console();

    static const Console* GetInstance();
    void WPrintF(const wchar_t* format, ...) const;
    void WPrintFV(const wchar_t* format, va_list args) const;
    void PrintF(const char* format, ...) const;
    void PrintFV(const char* format, va_list args) const;
    void Pause() const;
private:
    Console();
    Console(const Console&) = delete;
    void operator=(const Console&) = delete;

    static Console* m_instance;
    HANDLE m_hOutput;
    HANDLE m_hInput;
};
