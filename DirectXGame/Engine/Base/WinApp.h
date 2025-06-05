#pragma once
#include <Windows.h>
#include <cstdint>

namespace DaiEngine {
	class WinApp {
	private:
		WinApp() = default;
		~WinApp() = default;
		WinApp(const WinApp&) = delete;
		WinApp& operator=(const WinApp&) = delete;

	private:

		WNDCLASS wc_{};
		HWND hwnd_ = nullptr;

		bool endRequest_ = false;

	public: //静的メンバ変数
		static const int32_t kClientWidth = 1280;
		static const int32_t kClientHeight = 720;

	public: //静的メンバ関数

		static WinApp* GetInstance();

		static LRESULT WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	public: //メンバ関数
		HWND GetHwnd() const { return hwnd_; }

		HINSTANCE GetHInstance() const { return wc_.hInstance; }
		//ウィンドウ生成
		void CreateGameWindow(const wchar_t* title = L"DirectXGame", int32_t clientWidth = kClientWidth, int32_t clientHeight = kClientHeight);

		bool ProcessMessage();

		void TerminateGameWindow();

		void GameEnd() { endRequest_ = true; }

		bool IsEndRequest() { return endRequest_; }

	};

}
