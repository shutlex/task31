#include <windows.h>
#include <iostream>

int main() {
    HANDLE hReadPipe, hWritePipe;
    if (!CreatePipe(&hReadPipe, &hWritePipe, NULL, 0)) {
        std::cerr << "Failed to create canal!" << std::endl;
        return 1;
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdInput = hReadPipe;     
    si.hStdOutput = hWritePipe;   
    si.hStdError = hWritePipe;    
    si.dwFlags = STARTF_USESTDHANDLES;

    WCHAR cmd[] = L"cmd.exe";   
    if (!CreateProcess(
        NULL,                        
        cmd,                         
        NULL,                        
        NULL,                        
        TRUE,                        
        0,                           
        NULL,                        
        NULL,                        
        &si,                         
        &pi                          
    )) {
        std::cerr << "Failed to create process!" << std::endl;
        return 1;
    }

    CloseHandle(hReadPipe);
    CloseHandle(hWritePipe);

    const char* message = "echo Hello from parents proccesss!\n";
    DWORD bytesWritten;
    if (!WriteFile(pi.hProcess, message, strlen(message), &bytesWritten, NULL)) {
        std::cerr << "Failed to write!" << std::endl;
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
