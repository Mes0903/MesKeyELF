# Mes Key ELF

This is an tool that can help you send the keyboard and mouse input to the background window.

Or you can call it auto clicker, keyboard auto type or sth else.

The tool was wrote by C++ and ImGui, only support Windows now.

# How to Build

```bash
git clone https://github.com/Mes0903/MesKeyELF.git
cd MesKeyELF
mkdir build && cd build
cmake ..
cmake --build .
```

# How to Use

1. Scan the window and choose the target handler.
2. Add your key input into the key list.
3. Press the "Execute once" to test your operation.
4. Tick the "Execute" checkbox to make it execute cyclically.

Notice that if the target window was reopen, you need to scan the window again since the hwnd would be different.

# Dev list

- https://hackmd.io/41fB0zhyQIGpe2QScq_NQw