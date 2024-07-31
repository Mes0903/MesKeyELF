#ifndef KEYCOMMON_H
#define KEYCOMMON_H

#include <windows.h>
#include <vector>

enum KeyType { LEFT_MOUSE, RIGHT_MOUSE, KEY_1, KEY_2, KEY_Q, KEY_E, KEY_R, KEY_T };

struct KeyEvent {
  KeyType key;
  int pressDuration;    // 按下的時間，單位為毫秒
  int interval;    // 按鍵間隔，單位為毫秒
};

inline std::vector<KeyEvent> keyEvents;    // 按鍵事件列表
inline const char *keySet[] = { "左鍵", "右鍵", "1 (!)", "2 (@)", "Q", "E", "R", "T" };

int GetKeySetIndex(KeyType key);

#endif