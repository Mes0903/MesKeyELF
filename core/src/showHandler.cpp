#include "showHandler.h"
#include "imgui.h"

#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <sstream>
#include <utility>

namespace handler_detail {

  static std::string Big5ToUtf8(const std::string &big5)
  {
    // convert from Big5 to UTF-16
    int utf16Length = MultiByteToWideChar(950, 0, big5.c_str(), -1, nullptr, 0);
    if (utf16Length == 0) {
      std::cerr << "Failed to convert Big5 to UTF-16." << std::endl;
      return "";
    }

    std::wstring utf16(utf16Length, L'\0');
    MultiByteToWideChar(950, 0, big5.c_str(), -1, &utf16[0], utf16Length);

    // convert from UTF-16 to UTF-8
    int utf8Length = WideCharToMultiByte(CP_UTF8, 0, utf16.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (utf8Length == 0) {
      std::cerr << "Failed to convert UTF-16 to UTF-8." << std::endl;
      return "";
    }

    std::string utf8(utf8Length, '\0');
    WideCharToMultiByte(CP_UTF8, 0, utf16.c_str(), -1, &utf8[0], utf8Length, nullptr, nullptr);
    return utf8;
  }

  static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
  {
    std::vector<std::pair<HWND, std::string>> *handles = reinterpret_cast<std::vector<std::pair<HWND, std::string>> *>(lParam);

    const int length = GetWindowTextLength(hwnd);
    std::unique_ptr<char[]> buffer = std::make_unique<char[]>(length + 1);
    GetWindowText(hwnd, buffer.get(), length + 1);

    if (IsWindowVisible(hwnd) && length > 1) {
      std::string text = Big5ToUtf8(buffer.get());
      handles->push_back(std::make_pair(hwnd, text));
    }

    return TRUE;
  }

  static BOOL CALLBACK EnumChildWindowsProc(HWND hwnd, LPARAM lParam)
  {
    std::vector<std::pair<HWND, std::string>> *child_handles = reinterpret_cast<std::vector<std::pair<HWND, std::string>> *>(lParam);

    const int length = GetWindowTextLength(hwnd);
    std::unique_ptr<char[]> buffer = std::make_unique<char[]>(length + 1);
    GetWindowText(hwnd, buffer.get(), length + 1);

    if (IsWindowVisible(hwnd) && length > 1) {
      std::string text = Big5ToUtf8(buffer.get());
      child_handles->push_back(std::make_pair(hwnd, text));
    }

    return TRUE;
  }

}    // namespace handler_detail

void showHandler(bool &show_handler, HWND &selected_hwnd)
{
  using namespace handler_detail;

  static std::vector<std::pair<HWND, std::string>> handles;
  static std::vector<std::pair<HWND, std::string>> child_handles;

  ImGui::Begin("HANDLER", &show_handler);

  if (ImGui::Button("掃描視窗")) {
    handles.clear();
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&handles));
  }

  static int selected = -1;
  if (!handles.empty()) {
    if (selected > -1) {
      ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
      ImGui::Text("你選擇了「%s」視窗，hwnd: %p", handles[selected].second.c_str(), handles[selected].first);
      ImGui::PopStyleColor();
    }

    for (int n = 0; n < handles.size(); n++) {
      const char *title = handles[n].second.c_str();

      if (ImGui::Selectable(title, selected == n)) {
        selected = n;
        selected_hwnd = handles[n].first;
        child_handles.clear();
        EnumChildWindows(selected_hwnd, EnumChildWindowsProc, reinterpret_cast<LPARAM>(&child_handles));
      }
    }
  }

  if (ImGui::TreeNode("Child Windows")) {
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip()) {
      ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
      ImGui::TextUnformatted("這邊會顯示你選取視窗的子視窗，現在還只實作了顯示功能，沒有辦法選取");
      ImGui::PopTextWrapPos();
      ImGui::EndTooltip();
    }

    if (!child_handles.empty()) {
      for (const auto &child_handle : child_handles) {
        ImGui::Text("CHILD HANDLE: %p TITLE: %s\n", child_handle.first, child_handle.second.c_str());
      }
    }
    ImGui::TreePop();
  }

  if (ImGui::TreeNode("Details")) {
    if (!handles.empty()) {
      for (const auto &handle : handles) {
        ImGui::Text("HANDLE: %p TITLE: %s\n", handle.first, handle.second.c_str());
      }
    }
    ImGui::TreePop();
  }

  ImGui::End();
}