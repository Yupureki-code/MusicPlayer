# AGENTS.md — MusicPlayer

Compact hints for OpenCode sessions in this repo.

## Project type
- C++ Qt 6 GUI desktop app for Windows, built with **Visual Studio + Qt VS Tools**.
- **Not** CMake or qmake. The buildable solution is `MusicPlayer/MusicPlayer.slnx` (new `.slnx` format).
- Qt version pinned to `6.11.1_msvc2022_64` in the `.vcxproj`.

## Build
- Platform: **x64 only**; Configurations: `Debug`, `Release`.
- Toolset: `v145` (Visual Studio 2022).
- Open `MusicPlayer/MusicPlayer.slnx` in Visual Studio with the Qt VS Tools extension installed.
- QtMsBuild handles MOC/UIC/RCC automatically via targets imported in the `.vcxproj`.

## Code layout
- All source is under `MusicPlayer/`, not the repo root.
- Entry point: `main.cpp` → `MusicPlayer` widget.
- Key widgets: `MusicPlayer` (main window), `TitleBar` (drag-to-move), `NavigateBT` (nav buttons).
- Resources: `MusicPlayer.qrc` (images under `image/`).
- UI forms: `MusicPlayer.ui`, `NavigateBT.ui`.

## Precompiled headers
- Uses `stdafx.h` / `stdafx.cpp`.
- `stdafx.cpp` is the PCH creation unit (`PrecompiledHeader=Create`).
- **Every `.cpp` file must `#include "stdafx.h"` first.**
- Qt MOC is configured to prepend `stdafx.h` automatically (`<QtMoc><PrependInclude>stdafx.h</PrependInclude></QtMoc>`).

## Runtime behavior notes
- Windows GUI subsystem (`/SUBSYSTEM:WINDOWS`), not console.
- Main window is frameless (`FramelessWindowHint`) with translucent background and a drop shadow.
- TitleBar drag-to-move uses `startSystemMove()` (not manual coordinate math).
- Navigation button icons are loaded from the Qt resource system (`:/image/image/...` — the qresource prefix is `/image` and files live in the `image/` subdir) and injected at runtime in `InitNavigateBtIcon()` by child-name convention (`objectName() + "Icon"`).

## Testing / verification
- No automated tests or CI are present in the repo.
- To verify: build in Visual Studio and run the x64 output.

## Editing gotchas
- If you add a new `.cpp` file, ensure it includes `stdafx.h` first and is added to the `.vcxproj` `<ClCompile>` items.
- If you add a new header with `Q_OBJECT`, add it to the `.vcxproj` as `<QtMoc Include="...">` so MOC runs on it.
- If you add images, register them in `MusicPlayer.qrc`.
- If you add a new Qt module dependency, update both the `QtModules` property in `.vcxproj` and `#include` it in `stdafx.h` for PCH coverage.
