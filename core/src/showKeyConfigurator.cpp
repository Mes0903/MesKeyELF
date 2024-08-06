#include "showKeyConfigurator.h"
#include "imgui.h"

#include <string>
#include <iostream>

void showKeyConfigurator(bool &show_key_sender, const HWND &selected_hwnd)
{
  static int selected = -1;

  ImGui::Begin("Key Event Configurator", &show_key_sender);

  if (ImGui::TreeNode("新增按鍵事件")) {
    // 顯示說明
    ImGui::Text("設定按鍵事件與時間");
    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip()) {
      ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
      ImGui::TextUnformatted("設定好後按下「新增按鍵」以將按鍵放入清單");
      ImGui::PopTextWrapPos();
      ImGui::EndTooltip();
    }

    // 配置選中的按鍵事件
    static KeyEvent event;
    ImGui::Combo("按鍵", reinterpret_cast<int *>(&event.key), keySet, IM_ARRAYSIZE(keySet));
    ImGui::InputInt("持續按下幾 ms", &event.pressDuration);
    ImGui::InputInt("按完後等待幾 ms", &event.interval);

    if (!executeSender) {
      // ImGui::BeginDisabled(executeSender);
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor(255, 0, 0));
      if (ImGui::Button("新增按鍵")) {
        keyEvents.push_back(event);
      }

      ImGui::SameLine();
      if (ImGui::Button("刪除已選按鍵")) {
        if (selected >= 0 && selected < keyEvents.size()) {
          keyEvents.erase(keyEvents.begin() + selected);
          selected = -1;
        }
      }

      ImGui::SameLine();
      if (ImGui::Button("清空按鍵")) {
        keyEvents.clear();
        selected = -1;
      }

      ImGui::PopStyleColor();
      // ImGui::EndDisabled();
    }

    ImGui::TreePop();
  }

  if (ImGui::TreeNode("按鍵清單")) {
    // 顯示說明
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip()) {
      ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
      ImGui::TextUnformatted("拖曳按鍵事件以重新排序");
      ImGui::PopTextWrapPos();
      ImGui::EndTooltip();
    }

    // 顯示按鍵事件列表
    for (int i = 0; i < keyEvents.size(); ++i) {
      const KeyEvent event = keyEvents[i];
      std::string label = std::string(keySet[event.key]) + " 按壓 " + std::to_string(event.pressDuration) + " ms 後等待 " + std::to_string(event.interval) + " ms";
      if (ImGui::Selectable(label.c_str(), selected == i))
        selected = i;

      if (ImGui::IsItemActive() && !ImGui::IsItemHovered()) {
        int n_next = i + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
        if (n_next >= 0 && n_next < keyEvents.size()) {
          keyEvents[i] = keyEvents[n_next];
          keyEvents[n_next] = event;
          selected = n_next;
          ImGui::ResetMouseDragDelta();
        }
      }
    }

    ImGui::TreePop();
  }

  ImGui::End();
}