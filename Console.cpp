#include <strsafe.h>
#include <cstdarg>
#include <stdexcept>

#include "Console.h"

Console* Console::m_instance = nullptr;

Console::Console() {
    if (!AllocConsole()) {
        throw new std::runtime_error("Can't allocate console");
    }

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    m_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    m_hInput = GetStdHandle(STD_INPUT_HANDLE);
}

Console::~Console() {
    FreeConsole();
}

const Console* Console::GetInstance() {
    if (Console::m_instance == nullptr) {
        Console::m_instance = new Console();
    }

    return Console::m_instance;
}

void Console::Pause() const {
    HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD inputRecord;
    DWORD numEventsRead;
    DWORD previousMode;

    GetConsoleMode(inputHandle, &previousMode);
    SetConsoleMode(inputHandle, previousMode & ~(ENABLE_MOUSE_INPUT | ENABLE_LINE_INPUT));
    do
    {
        ReadConsoleInput(inputHandle, &inputRecord, 1, &numEventsRead);
    } while (inputRecord.EventType != KEY_EVENT || !inputRecord.Event.KeyEvent.bKeyDown);

    SetConsoleMode(inputHandle, previousMode);
}

void Console::WPrintF(const wchar_t* format, ...) const {
    va_list args;
    va_start(args, format);
    WPrintFV(format, args);
    va_end(args);
}

void Console::WPrintFV(const wchar_t* format, va_list args) const {
    int bufferSize = _vscwprintf(format, args) + 1;
    wchar_t* buffer = new wchar_t[bufferSize];
    StringCbVPrintfW(buffer, bufferSize * sizeof(wchar_t), format, args);

    DWORD bytesWritten;
    WriteConsoleW(m_hOutput, buffer, bufferSize - 1, &bytesWritten, nullptr);

    delete[] buffer;
}

void Console::PrintF(const char* format, ...) const {
    va_list args;
    va_start(args, format);
    PrintFV(format, args);
    va_end(args);
}

void Console::PrintFV(const char* format, va_list args) const {
    int bufferSize = _vscprintf(format, args) + 1;
    char* buffer = new char[bufferSize];
    StringCbVPrintfA(buffer, bufferSize, format, args);

    DWORD bytesWritten;
    WriteConsoleA(m_hOutput, buffer, bufferSize - 1, &bytesWritten, nullptr);

    delete[] buffer;
}
