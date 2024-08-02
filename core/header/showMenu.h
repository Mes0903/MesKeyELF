#ifndef SHOWMENU_H
#define SHOWMENU_H

struct DisplayFlags {
  bool show_demo_window = false;
  bool show_handler = true;
  bool show_key_configurator = true;
  bool show_key_sender = true;
  bool show_screen = false;
};

void showMenu(DisplayFlags &display);

#endif