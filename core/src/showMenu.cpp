#include "showMenu.h"
#include "imgui.h"

void showMenu(DisplayFlags &display)
{
  ImGui::Begin("Menu");

  ImGui::SeparatorText("ABOUT MesKeyELF:");

  ImGui::BulletText("這是一個簡單版的按鍵精靈，可以拿來往背景視窗輸入鍵盤與滑鼠的操作");
  ImGui::BulletText("使用方法如下：");
  ImGui::Indent();
  ImGui::BulletText("先打開掃描視窗按「掃描視窗」");
  ImGui::Indent();
  ImGui::BulletText("此時你會看到現在可以選擇的視窗，在這裡選擇你的目標視窗");
  ImGui::Unindent();
  ImGui::BulletText("接著打開鍵盤設定視窗，設定你要的按鍵事件");
  ImGui::BulletText("最後打開執行鍵盤指令視窗，按下「執行一次」即可開始執行按鍵事件");
  ImGui::BulletText("如果要重複執行，把執行旁邊的 Checkbox 勾起來就可以了");
  ImGui::BulletText("如果視窗上面有 (?) 的圖示，那是說明，可以把滑鼠移到上面看看");

  ImGui::SeparatorText("視窗顯示清單");

  ImGui::Checkbox("掃描視窗", &display.show_handler);
  ImGui::Checkbox("鍵盤設定視窗", &display.show_key_configurator);
  ImGui::Checkbox("執行鍵盤指令視窗", &display.show_key_sender);
  ImGui::Checkbox("截圖小玩具", &display.show_screen);
  ImGui::Checkbox("ImGui 功能一覽", &display.show_demo_window);

  ImGui::SeparatorText("Contact me:");

  ImGui::BulletText("Discord: mes_0903");
  ImGui::BulletText("github: Mes0903");
  ImGui::BulletText("project link: https://github.com/Mes0903/MesKeyELF");
  ImGui::SameLine();
  if (ImGui::SmallButton("copy")) {
    ImGui::SetClipboardText("https://github.com/Mes0903/MesKeyELF");
  }

  ImGui::End();
}