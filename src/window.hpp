#pragma once

#include <windows.h>
#include "vector.hpp"

class Window {
    private:
    // variables

    static Window* currentInstance;
    const char* title;
    int height, width;
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    HWND hwnd{};
    unsigned int* memory = nullptr;
    size_t buffer_size{};
    BITMAPINFO buffer_bitmap{}; // color bitmap
    HDC hdc{};
    // functions

    static LRESULT CALLBACK WindowCallback(HWND, UINT, WPARAM, LPARAM); // TODO:
    void RegisterWindowClass();
    bool CreateWindowInstance(); // TODO:

    public:
    // initialize window
    // @returns true if window is initialized otherwise false;
    bool Init();

    // keeps the window running
    // @returns true if the window is still open otherwise false;
    bool IsOpen(); 

    explicit Window(const char* title = "My Window", int height=200, int width=200) : title(title), height(height), width(width) {}

    void render_background(unsigned int); // defined in "render.hpp"
    void Rectangle(Vector2, Vector2, unsigned int); // "render.hpp"
    void Circle(Vector2, int, unsigned int); // "render.hpp"

    ~Window() {
        // destroy the window after its finished
        DestroyWindow(this->hwnd);
    }

    int getHeight() const {
        return this->height;
    }

    int getWidth() const {
        return width;
    }

    /*double getDeltaTime() {
        return this->delta;
    }*/

    /*
        * March 5th 2024, 12:39AM:
            * ~Window()
            * Window()
            * IsOpen()
            * Init()
            * RegisterWindowClass()
            * WindowCallback()
        
        * March 6th 2024, 12:03AM:
            * getHeight()
            * getWidth()
            * render_background()
            * Rectangle(Unfinished)

        * March 8th 2024, 12:27AM:
            * GetMousePosition()
            * Rectangle(finished)
            * Circle()
            * struct Circle
            * struct Rectangle
            * IsKeyPressed()
            * checkCollision()
            * clamp()
            * GetMousePosition(int&, int&)
    */
};

Window* Window::currentInstance = nullptr;

void Window::RegisterWindowClass() {
    WNDCLASSEX WindowClass = {};
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = Window::WindowCallback;

    WindowClass.cbSize = sizeof(WNDCLASSEX);
    WindowClass.style = 0;
    WindowClass.lpfnWndProc = Window::WindowCallback;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = this->hInstance;
    WindowClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    WindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    WindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    WindowClass.lpszMenuName = nullptr;
    WindowClass.lpszClassName = "Window";
    WindowClass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    //register the window class;
    if(!RegisterClassEx(&WindowClass)) {
        MessageBox(nullptr, "Failed to register class", "Error", MB_ICONEXCLAMATION | MB_OK);
        exit(1);
    }
}

bool Window::CreateWindowInstance() {
    this->hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "Window",
        this->title,
        (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX),
        CW_USEDEFAULT, CW_USEDEFAULT, this->width, this->height,
        nullptr, nullptr, this->hInstance, nullptr
    );

    if(this->hwnd == nullptr) {
        // let the user know it failed
        MessageBox(nullptr, "Failed to Create Window", "Error", MB_ICONEXCLAMATION | MB_OK);
        return false; 
    }

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    return true; 
}

LRESULT CALLBACK Window::WindowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Window* instance = currentInstance;

    switch(uMsg) {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_SIZE: {
            RECT rect;
            GetClientRect(hwnd, &rect);

            instance->height = rect.bottom - rect.top;
            instance->width = rect.right - rect.left;

            instance->buffer_size = instance->width * instance->height * sizeof(unsigned int);

            if(instance->memory) VirtualFree(instance->memory, 0, MEM_RELEASE);
            instance->memory = (unsigned int*)VirtualAlloc(nullptr, instance->buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            instance->buffer_bitmap.bmiHeader.biSize = sizeof(buffer_bitmap.bmiHeader);
            instance->buffer_bitmap.bmiHeader.biWidth = instance->width;
            instance->buffer_bitmap.bmiHeader.biHeight = instance->height;
            instance->buffer_bitmap.bmiHeader.biPlanes = 1;
            instance->buffer_bitmap.bmiHeader.biBitCount = 32; // 32 bit
            instance->buffer_bitmap.bmiHeader.biCompression = BI_RGB;

        } break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

bool Window::Init() {
    currentInstance = this;

    this->RegisterWindowClass();
    if(!CreateWindowInstance()) {
        return false;
    }

    this->hdc = GetDC(this->hwnd);
    
    return true;
}

bool Window::IsOpen() {
    MSG Msg;

    while(PeekMessage(&Msg, nullptr, 0u, 0u, PM_REMOVE)) {
        if(Msg.message == WM_QUIT) {
            return false;
        }

        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    // draws stuff to the screen Ig
    StretchDIBits(hdc, 0, 0, this->width, this->height, 0, 0, this->width, this->height, this->memory, &this->buffer_bitmap, DIB_RGB_COLORS, SRCCOPY);

    return true;
}