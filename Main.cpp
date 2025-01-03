#include <Windows.h>
#include <gl/GL.h>
#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_opengl2.h"
#include <detours.h>
#include <iostream>
#include "Console.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "detours.lib")

bool showMenu = false; // Menu On/Off
typedef BOOL(WINAPI* twglSwapBuffers)(HDC);
twglSwapBuffers oSwapBuffers;

// ImGui
class ImGuiHandler {
public:
    static void Init(HWND hwnd);
    static void BeginFrame();
    static void EndFrame();
    static void Shutdown();

private:
    static bool initialized;
};

bool ImGuiHandler::initialized = false;

void ImGuiHandler::Init(HWND hwnd) {
    if (initialized) return;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplOpenGL2_Init();

    initialized = true;
}

void ImGuiHandler::BeginFrame() {
    if (!initialized) return;

    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void ImGuiHandler::EndFrame() {
    if (!initialized) return;

    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiHandler::Shutdown() {
    if (!initialized) return;

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    initialized = false;
}

// Hook de la fonction wglSwapBuffers
BOOL WINAPI hkSwapBuffers(HDC hdc) {
    static bool init = false;

    if (!init) {
        HWND hwnd = FindWindow(nullptr, L"Minecraft 1.16.5"); // Change Name Window Here
        if (hwnd) {
            ImGuiHandler::Init(hwnd);
            init = true;
        }
    }

    if (GetAsyncKeyState(VK_INSERT) & 1) {
        showMenu = !showMenu;
    }

    if (showMenu) {
        ImGuiHandler::BeginFrame();

        ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Once);

        ImGui::Begin("Menu Minecraft", nullptr, ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Bienvenue dans le menu ImGui !");
        
        ImGui::End();

        ImGuiHandler::EndFrame();
    }

    return oSwapBuffers(hdc);
}

DWORD WINAPI MainThread(LPVOID lpReserved) {
    CreateDebugConsole();
    std::cout << "[INFO] CONSOLE ON .\n";
    std::cout << "[INFO] Touche 'Ins' pour ouvrir le menu .\n";

    HMODULE hOpengl = GetModuleHandleW(L"opengl32.dll");
    if (!hOpengl) {
        std::cout << "[Debug] Impossible de trouver opengl32.dll.\n";
        return 0;
    }

    oSwapBuffers = (twglSwapBuffers)GetProcAddress(hOpengl, "wglSwapBuffers");
    if (!oSwapBuffers) {
        std::cout << "[Debug] Impossible de trouver wglSwapBuffers.\n";
        return 0;
    }

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)oSwapBuffers, hkSwapBuffers);
    DetourTransactionCommit();

    std::cout << "[Debug] Hook attaché avec succès.\n";

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)oSwapBuffers, hkSwapBuffers);
        DetourTransactionCommit();

        ImGuiHandler::Shutdown();
    }
    return TRUE;
}
