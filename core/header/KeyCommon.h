#ifndef KEYCOMMON_H
#define KEYCOMMON_H

#include <windows.h>
#include <vector>
#include <atomic>

enum KeyType { LEFT_MOUSE, RIGHT_MOUSE, SPACE, KEY_W, KEY_A, KEY_S, KEY_D, KEY_1, KEY_2, KEY_SHIFT, KEY_Q, KEY_R, KEY_T, KEY_E };

struct KeyEvent {
  KeyType key = KeyType::LEFT_MOUSE;    // 按鍵
  int pressDuration = 500;    // 按下的時間，單位為毫秒
  int interval = 100;    // 按鍵間隔，單位為毫秒
};

inline std::vector<KeyEvent> keyEvents;    // 按鍵事件列表
inline const char *keySet[] = { "左鍵", "右鍵", "空白鍵", "W", "A", "S", "D", "1 (!)", "2 (@)", "Shift", "Q", "R", "T", "E" };

inline bool executeFlag = false;    // if true, the main loop would execute the sender, and set executeSender to true
inline std::atomic<bool> executeSender = false;

#endif