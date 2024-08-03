#include <stdio.h>
#include <windows.h>

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

int main()
{
    printf("Welcome to C-AutoClicker.\n");

    HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (hhkLowLevelKybd == 0x0) {
        printf("Failed to install hook!\n");
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hhkLowLevelKybd);
    return 0;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if(wParam == WM_KEYDOWN)
    {
        KBDLLHOOKSTRUCT *pKeyboard = (KBDLLHOOKSTRUCT *)lParam;

        switch(pKeyboard->vkCode)
        {
            case 117:
                printf("Pressed F6\n");
            break;

            case 118:
                printf("Program will terminate now.\n");
                exit(0); // Exit with status 0, indicating successful termination
            break;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}
