#include "showKeySender.h"
#include "KeyCommon.h"

#include "imgui.h"
#include <glad/glad.h>

#include <iostream>
#include <vector>
#include <string>

namespace keysender_detail {

  static BYTE GetVirtualKeyCode(KeyType key)
  {
    switch (key) {
    case KeyType::LEFT_MOUSE:
      return VK_LBUTTON;
    case KeyType::RIGHT_MOUSE:
      return VK_RBUTTON;
    case KeyType::KEY_1:
      return '1';
    case KeyType::KEY_2:
      return '2';
    case KeyType::KEY_Q:
      return 'Q';
    case KeyType::KEY_E:
      return 'E';
    case KeyType::KEY_R:
      return 'R';
    case KeyType::KEY_T:
      return 'T';
    default:
      return 0;
    }
  }

  static void SendKeyEvent(HWND hwnd, const KeyEvent &event)
  {
    BYTE vkCode = VkKeyScan(GetVirtualKeyCode(event.key));
    PostMessage(hwnd, WM_KEYDOWN, vkCode, 0);
    PostMessage(hwnd, WM_CHAR, GetVirtualKeyCode(event.key), 0);
    PostMessage(hwnd, WM_KEYUP, vkCode, 0);
  }
}    // namespace keysender_detail

void showKeySender(bool &show_key_sender, HWND selected_hwnd)
{
  using namespace keysender_detail;

  ImGui::Begin("Send Key Events", &show_key_sender);

  // 發送按鍵事件
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor(255, 0, 0));
  if (ImGui::Button("Send Key Events") && selected_hwnd != nullptr) {
    for (const KeyEvent &event : keyEvents) {
      std::cout << "HWND: " << selected_hwnd << ", Key: " << event.key << ", Press Duration: " << event.pressDuration << ", Interval: " << event.interval << std::endl;
      SendKeyEvent(selected_hwnd, event);
      Sleep(event.interval);
    }
  }
  ImGui::PopStyleColor();

  ImGui::End();
}