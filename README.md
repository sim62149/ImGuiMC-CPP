# ImGuiMC-Dll

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## **Description**

**ImGuiMC-Dll** est un projet en **C++** permettant d'afficher une interface utilisateur **ImGui** dans le jeu **Minecraft**, compatible avec les versions allant de **1.0 à 1.16.5**. Ce module injecte un hook dans la fonction `wglSwapBuffers` de **OpenGL** et utilise **ImGui** pour afficher un menu personnalisé dans le jeu.

---

## **Fonctionnalités**

- Affichage d'une interface graphique dans Minecraft avec **ImGui**.
- Activation/désactivation du menu via la touche **Insert**.
- Console de débogage intégrée pour afficher des logs en temps réel.
- Support des versions Minecraft **1.0 à 1.16.5**.

---

## **Configuration requise**

- Système d'exploitation : **Windows** avec prise en charge de **OpenGL**.
- Environnement de développement supportant **C++** (Visual Studio recommandé).
- Bibliothèques nécessaires :
  - **opengl32.lib** (inclus NORMALEMENT dans Windows)
  - **detours.lib** (inclus dans le projet).
  - **ImGui** (inclus dans le projet).

---

## **Installation et compilation**

### Étapes d'installation

1. Clonez ce dépôt sur votre machine :
   ```bash
   git clone https://github.com/sim62149/ImGuiMC-Dll.git
   cd ImGuiMC-Dll
   ```

2. Ouvrez le fichier `ImGuiMC-Dll.sln` dans **Visual Studio**.

3. Avant de compiler, modifiez le nom de la fenêtre Minecraft ciblée dans le fichier `Main.cpp` :
   ```cpp
   HWND hwnd = FindWindow(nullptr, L"Minecraft 1.16.5"); // Remplacez par le titre exact de la fenêtre Minecraft
   ```

4. Compilez le projet en mode **Release** ou **Debug**.

5. Injectez le fichier DLL généré dans le processus Minecraft à l'aide d'un injecteur de DLL de votre choix.

---

## **Structure du projet**

- `Main.cpp` : Contient la logique principale, notamment l'initialisation de ImGui et le hook de la fonction `wglSwapBuffers`.
- `Console.cpp` / `Console.h` : Gère la console de débogage.
- `Include/` - `lib/` : Contient les fichiers de bibliothèque nécessaires, y compris les fichiers backend pour ImGui.

---

## **Fonctionnement du code**

Lors de l'injection de la DLL, la fonction `DllMain` démarre un thread principal qui :
- Initialise une console de débogage.
- Recherche la fenêtre Minecraft.
- Configure ImGui.
- Attache un hook sur la fonction OpenGL `wglSwapBuffers`.

À chaque frame :
- Si le menu est activé via la touche **Insert**, il est affiché à l'écran grâce à ImGui.

Lors du détachement de la DLL :
- Le hook est retiré.
- ImGui est correctement arrêté.
- La console de débogage est fermée.

---

## **Licence**

Ce projet est sous **licence MIT**. Vous pouvez consulter le fichier [LICENSE](LICENSE) pour plus de détails.

---

## **Avertissements**

- L'injection de DLL dans un jeu peut être considérée comme une modification non autorisée. Utilisez-le à vos propres risques et uniquement dans un environnement de test.

---

## **Crédits**

Développé par **sim62149**, basé sur les bibliothèques :
- **ImGui**
- **Detours**
