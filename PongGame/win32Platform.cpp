#include <windows.h>
#include "utils.cpp"

// Function Parameter
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


struct RenderState {
    int height, width;
    void* memory;
    BITMAPINFO bitmapinfo;
};

// Global variables
static bool running = true;
RenderState renderState;
#include "platformCommon.cpp"
#include "renderer.cpp"
#include "game.cpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Define the window class
    WNDCLASS windowClass = { 0 };
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProcedure;
    windowClass.hInstance = hInstance;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    windowClass.lpszClassName = L"WindowClass";

    // Register the window class
    if (!RegisterClass(&windowClass))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Create the window
    HWND hwnd = CreateWindow(
        L"WindowClass",             // Class name
        L"Pong",                    // Window title
        WS_OVERLAPPEDWINDOW,        // Window style
        CW_USEDEFAULT,              // X position
        CW_USEDEFAULT,              // Y position
        1280,                        // Width
        720,                        // Height
        NULL,                       // Parent window
        NULL,                       // Menu
        hInstance,                  // Instance handle
        NULL                        // Additional application data
    );

    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Display the window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    HDC hdc = GetDC(hwnd);

    MSG msg = {};

    Input input = {};

    float deltaTime = 0.016666f;
    LARGE_INTEGER frameBeginTÝme;
    QueryPerformanceCounter(&frameBeginTÝme);

    float performanceFrequency;
    {
        LARGE_INTEGER perf;
        QueryPerformanceFrequency(&perf);
        performanceFrequency = (float)perf.QuadPart;
    }


    for (int i = 0; i < buttonCount; i++)
    {
        input.buttons[i].changed = false;
    }

    while (running)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            switch (msg.message)
            {
            case WM_KEYUP:
            case WM_KEYDOWN: {
                unsigned int vkCode = (unsigned int)msg.wParam;
                bool isDown = ((msg.lParam & (1 << 31)) == 0);

                #define processButton(b, vk) \
                    case vk: {\
                        input.buttons[b].changed = isDown != input.buttons[b].isDown ; \
                        input.buttons[b].isDown = isDown; \
                    } break;

                switch (vkCode)
                {
                    processButton(buttonUp, VK_UP);
                    processButton(buttonDown, VK_DOWN);
                    processButton(buttonW, 'W');
                    processButton(buttonS, 'S');
                    processButton(buttonLeft, VK_LEFT);
                    processButton(buttonRight, VK_RIGHT);
                    processButton(buttonEnter, VK_RETURN);
                }
            } break;

            default:
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            }
        }

        // Your game logic goes here
        // Simulate, Render, etc.
        renderBackground();
        simulateGame(&input, deltaTime);
        StretchDIBits(hdc, 0, 0, renderState.width, renderState.height, 0, 0, renderState.width, renderState.height, renderState.memory, &renderState.bitmapinfo, DIB_RGB_COLORS, SRCCOPY);

        LARGE_INTEGER frameEndTÝme;
        QueryPerformanceCounter(&frameEndTÝme);
        deltaTime = (float)(frameEndTÝme.QuadPart - frameBeginTÝme.QuadPart) / performanceFrequency;
        frameBeginTÝme = frameEndTÝme;
    }

    return msg.wParam;
}


LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CLOSE:
        case WM_DESTROY:
            {
            running = false;
        } break;
                     
    case WM_SIZE:
    {
        RECT rect;
        GetClientRect(hwnd, &rect);
        renderState.width = rect.right - rect.left;
        renderState.height = rect.bottom - rect.top;
        int size = renderState.width * renderState.height * sizeof(unsigned int);
        
        if (renderState.memory)
        {
            VirtualFree(renderState.memory, 0, MEM_RELEASE);
        }

        renderState.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);


        // BITMAPINFOHEADER 
        renderState.bitmapinfo.bmiHeader.biSize             = sizeof(BITMAPINFOHEADER);
        renderState.bitmapinfo.bmiHeader.biWidth            = renderState.width;
        renderState.bitmapinfo.bmiHeader.biHeight           = renderState.height;
        renderState.bitmapinfo.bmiHeader.biPlanes           = 1;
        renderState.bitmapinfo.bmiHeader.biBitCount         = 32;
        renderState.bitmapinfo.bmiHeader.biCompression      = BI_RGB;
        renderState.bitmapinfo.bmiHeader.biSizeImage        = 0;
        renderState.bitmapinfo.bmiHeader.biXPelsPerMeter    = 0;
        renderState.bitmapinfo.bmiHeader.biYPelsPerMeter    = 0;
        renderState.bitmapinfo.bmiHeader.biClrUsed          = 0;
        renderState.bitmapinfo.bmiHeader.biClrImportant     = 0;

        break;
    }
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}
