#include "showScreen.h"

#include "imgui.h"
#include <glad/glad.h>
#include <stdexcept>
#include <iostream>

HBITMAP CaptureWindow(HWND hWnd)
{
  HDC hWindowDC = GetWindowDC(hWnd);
  RECT rect;
  GetClientRect(hWnd, &rect);
  int width = rect.right - rect.left;
  int height = rect.bottom - rect.top;

  if (width == 0 || height == 0) {
    std::cerr << "Window has zero area." << std::endl;
    return nullptr;
  }

  HDC hCaptureDC = CreateCompatibleDC(hWindowDC);
  BITMAPINFO bmi;
  memset(&bmi, 0, sizeof(bmi));
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = width;
  bmi.bmiHeader.biHeight = -height;    // Negative height to create a top-down DIB
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;

  BYTE *pBits;
  HBITMAP hDIB = CreateDIBSection(hCaptureDC, &bmi, DIB_RGB_COLORS, (void **) &pBits, NULL, 0);
  if (!hDIB) {
    std::cerr << "CreateDIBSection failed." << std::endl;
    return nullptr;
  }

  HGDIOBJ hOldBmp = SelectObject(hCaptureDC, hDIB);
  if (!BitBlt(hCaptureDC, 0, 0, width, height, hWindowDC, 0, 0, SRCCOPY | CAPTUREBLT)) {
    std::cerr << "BitBlt failed." << std::endl;
  }

  SelectObject(hCaptureDC, hOldBmp);
  DeleteDC(hCaptureDC);
  ReleaseDC(hWnd, hWindowDC);

  return hDIB;
}

GLuint BitmapToTexture(HBITMAP hBitmap)
{
  puts("BitmapToTexture");
  BITMAP bmp;
  GetObject(hBitmap, sizeof(BITMAP), &bmp);

  // Generate a texture
  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  // Set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Set texture data
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bmp.bmWidth, bmp.bmHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, bmp.bmBits);

  return textureID;
}

void RenderTexture(GLuint textureID)
{
  ImGui::Begin("Window Capture");

  // Get the available region size
  ImVec2 avail = ImGui::GetContentRegionAvail();
  float availWidth = avail.x;
  float availHeight = avail.y;

  // Calculate the aspect ratio
  float imageWidth = ImGui::GetContentRegionAvail().x;
  float imageHeight = ImGui::GetContentRegionAvail().y;

  float aspectRatio = static_cast<float>(imageWidth) / static_cast<float>(imageHeight);

  // Calculate the optimal size for the image
  float imageDisplayHeight = availHeight;
  float imageDisplayWidth = imageDisplayHeight * aspectRatio;

  // If the calculated width exceeds the available width, adjust both
  if (imageDisplayWidth > availWidth) {
    imageDisplayWidth = availWidth;
    imageDisplayHeight = imageDisplayWidth / aspectRatio;
  }

  // Render the image
  ImGui::Image(reinterpret_cast<void *>(static_cast<intptr_t>(textureID)), ImVec2(imageDisplayWidth, imageDisplayHeight));

  ImGui::End();
}

void showScreen(bool &show_screen, HWND hwnd)
{
  static GLuint textureID = 0;    // Texture ID for OpenGL
  static bool isTextureReady = false;    // Flag to check if texture is ready to render

  ImGui::Begin("Screen Viewer", &show_screen);

  if (ImGui::Button("Capture Window") && hwnd != nullptr) {
    if (isTextureReady) {
      glDeleteTextures(1, &textureID);    // Delete the old texture if exists
      isTextureReady = false;
    }

    HBITMAP hBitmap = CaptureWindow(hwnd);    // Capture the window image into a bitmap
    if (hBitmap == nullptr) {
      puts("hBitmap is nullptr");
    }
    else {
      BITMAP bmpInfo;
      if (GetObject(hBitmap, sizeof(BITMAP), &bmpInfo) == 0) {
        std::cerr << "Failed to get bitmap info." << std::endl;
        // Handle error, for instance, by deleting the bitmap
        DeleteObject(hBitmap);
      }
      else {
        if (bmpInfo.bmBits == nullptr) {
          std::cerr << "Bitmap data is empty." << std::endl;
        }
        else {
          // Proceed with converting the bitmap to a texture
          textureID = BitmapToTexture(hBitmap);
          DeleteObject(hBitmap);    // Clean up the bitmap object
          isTextureReady = true;
        }
      }
    }
  }

  if (isTextureReady) {
    RenderTexture(textureID);    // Render the texture in ImGui window
  }

  ImGui::End();
}