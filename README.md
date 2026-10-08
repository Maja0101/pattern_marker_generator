# Pattern Marker Generator

**The Team Project** on the Applied Computer Science

C++ project, responsible for solving 2d nesting problem, to be used as part of a Sewing Patterns Optimalization Project. 

## Requirements

- CMake 3.18+
- C++ Compiler with C++20 support (e.g. g++)
- (Optional) VS Code + extensions:
  - CMake Tools
  - C/C++ (ms-vscode.cpptools)

## Build and run using CLI

### Debug

```bash
cmake --preset debug
cmake --build --preset debug
./build/debug/pattern_marker_generator
```

### Release

```bash
cmake --preset release
cmake --build --preset release
./build/release/pattern_marker_generator
```

## Unit tests using CLI

### Debug

```bash
ctest --preset debug
```

## Build and run using VS Code

The following options should be visible on the status bar (bottom bar) in VSCode:
- Change the active configuration preset
  - *Debug*
  - *Release*
- Build the selected target
- Set the default build target 
  - all/pattern_marker_generator/unit_test for *Debug*
  - all/pattern_marker_generator for *Release*
- Launch the debugger for the selected target
- Launche the selected target (Run)
- Select the target to launch
  - pattern_marker_generator/unit_test for *Debug*
  - pattern_marker_generator for *Release*
