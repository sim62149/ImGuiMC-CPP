#include "Console.h"
#include <Windows.h>
#include <iostream>

void CreateDebugConsole() {
    AllocConsole();
    FILE* file;
    freopen_s(&file, "CONOUT$", "w", stdout);
    freopen_s(&file, "CONOUT$", "w", stderr);
    std::cout << "[Debug] Console attach�e avec succ�s.\n";
}

void CloseDebugConsole() {
    std::cout << "[Debug] Fermeture de la console.\n";
    FreeConsole();
}
