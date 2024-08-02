#include "showKeySender.h"
#include "KeyCommon.h"

#include "imgui.h"
#include <glad/glad.h>

#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <thread>

namespace keysender_detail {
  static std::deque<DWORD> KeyLogs;

  static BYTE GetVirtualKeyCode(KeyType key)
  {
    // Virtual-Key table: https://learn.microsoft.com/zh-tw/windows/win32/inputdev/virtual-key-codes
    switch (key) {
    case KeyType::LEFT_MOUSE:
      return MK_LBUTTON;
    case KeyType::RIGHT_MOUSE:
      return MK_RBUTTON;
    case KeyType::SPACE:
      return VK_SPACE;
    case KeyType::KEY_W:
      return 0x57;
    case KeyType::KEY_A:
      return 0x41;
    case KeyType::KEY_S:
      return 0x53;
    case KeyType::KEY_D:
      return 0x44;
    case KeyType::KEY_1:
      return 0x31;
    case KeyType::KEY_2:
      return 0x32;
    case KeyType::KEY_SHIFT:
      return VK_SHIFT;
    case KeyType::KEY_Q:
      return 0x51;
    case KeyType::KEY_R:
      return 0x52;
    case KeyType::KEY_T:
      return 0x54;
    case KeyType::KEY_E:
      return 0x45;
    default:
      return VK_SPACE;
    }
  }

  static void SendKeyEvent(HWND hwnd)
  {
    const std::vector<KeyEvent> &localKeyEvents = keyEvents;

    for (const KeyEvent &event : localKeyEvents) {
      BYTE vkCode = GetVirtualKeyCode(event.key);

      if (event.key == KeyType::LEFT_MOUSE || event.key == KeyType::RIGHT_MOUSE) {
        UINT msgDOWN = KeyType::LEFT_MOUSE ? WM_LBUTTONDOWN : WM_RBUTTONDOWN;
        UINT msgUP = KeyType::LEFT_MOUSE ? WM_LBUTTONUP : WM_RBUTTONUP;

        PostMessage(hwnd, msgDOWN, vkCode, MAKELPARAM(0, 0));
        Sleep(event.pressDuration);
        PostMessage(hwnd, msgUP, 0, MAKELPARAM(0, 0));
      }
      else {
        // keydown lparam flag: https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-keydown
        // MapVirtualKey: https://learn.microsoft.com/zh-tw/windows/win32/api/winuser/nf-winuser-mapvirtualkeyw
        LPARAM keyDownLPARAM = static_cast<LPARAM>(1 | (MapVirtualKey(vkCode, MAPVK_VK_TO_VSC) << 16) | (0 << 24) | (0 << 29) | (0 << 31));
        LPARAM keyUpLPARAM = static_cast<LPARAM>(1 | (MapVirtualKey(vkCode, MAPVK_VK_TO_VSC) << 16) | (0 << 24) | (0 << 29) | (1 << 30) | (0 << 31));

        PostMessage(hwnd, WM_KEYDOWN, static_cast<WPARAM>(vkCode), keyDownLPARAM);
        Sleep(event.pressDuration);
        PostMessage(hwnd, WM_KEYUP, static_cast<WPARAM>(vkCode), keyUpLPARAM);
      }

      KeyLogs.push_front(vkCode);
      if (KeyLogs.size() > 10)
        KeyLogs.pop_back();

      Sleep(event.interval);
    }

    executeSender.store(false);
  }

  static LRESULT CALLBACK KeyDebugger(int nCode, WPARAM wParam, LPARAM lParam)
  {
    PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT) lParam;

    // a key was pressed
    if (wParam == WM_KEYDOWN && nCode == HC_ACTION) {
      // print key
      std::cout << "Key: " << static_cast<char>(key->vkCode) << " pressed, key valued: " << std::hex << key->vkCode << '\n';
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
  }
}    // namespace keysender_detail

void showKeySender(bool &show_key_sender, HWND selected_hwnd)
{
  using namespace keysender_detail;
  static bool execute_flag = false;

  ImGui::Begin("Send Key Events", &show_key_sender);

  // 發送按鍵事件
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor(255, 0, 0));
  if (ImGui::Button("執行一次") && selected_hwnd != nullptr) {
    execute_flag = true;

    if (!executeSender) {
      executeSender.store(true);
      std::thread t(SendKeyEvent, selected_hwnd);
      t.detach();
    }

    execute_flag = false;
  }
  ImGui::PopStyleColor();

  ImGui::Checkbox("執行", &execute_flag);
  if (execute_flag && !executeSender) {
    executeSender.store(true);
    std::thread t(SendKeyEvent, selected_hwnd);
    t.detach();
  }

  if (ImGui::TreeNode("Key logs")) {
    for (auto vkCodeLog : KeyLogs)
      ImGui::Text("Key: %c pressed, key valued: 0x%x", static_cast<char>(vkCodeLog), vkCodeLog);

    ImGui::TreePop();
  }

  ImGui::End();
}