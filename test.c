#include <stdio.h>
#include <windows.h>

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
DWORD WINAPI keyHookThread();
DWORD WINAPI clickthread();

int enabled = 0;

int main()
{
    printf("Welcome to C-AutoClicker.\n");

    HANDLE thread1 = CreateThread(NULL, 0, keyHookThread, NULL, 0, NULL);
    HANDLE thread2 = CreateThread(NULL, 0, clickthread, NULL, 0, NULL);
    
    WaitForSingleObject(thread1, INFINITE); // Wait for the thread to finish
    WaitForSingleObject(thread2, INFINITE); // Wait for the thread to finish

    CloseHandle(thread1);
    CloseHandle(thread2);
    return 0;
}


void LeftClick() {
    // Set the mouse position (optional)
    // SetCursorPos(100, 100);

    // Simulate mouse left button down
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(INPUT));

    // Simulate mouse left button up
    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(INPUT));
}

DWORD WINAPI clickthread()
{
    while(1)
    {
        if (enabled)
        {
            LeftClick();
            Sleep(1); // Delay to avoid overwhelming the console and system
        }
    }
    return 0;
}

DWORD WINAPI keyHookThread()
{
    HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (hhkLowLevelKybd == 0x0) 
    {
        printf("Failed to install hook!\n");
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) 
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hhkLowLevelKybd);
    return 0;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if(nCode >= 0 && wParam == WM_KEYDOWN)
    {
        KBDLLHOOKSTRUCT *pKeyboard = (KBDLLHOOKSTRUCT *)lParam;

        switch(pKeyboard->vkCode)
        {
            case VK_F6: // Use VK_F6 instead of 117 for clarity
                enabled = !enabled; // Toggle enabled
                printf("Pressed F6, Enabled = %d\n", enabled);
            break;

            case VK_F7: // Use VK_F9 instead of 118 for clarity
                printf("Program will terminate now.\n");
                exit(0); // Exit with status 0, indicating successful termination
            break;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}