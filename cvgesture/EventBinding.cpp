#include <Windows.h>
#include <assert.h>
#include "cvgesture.h"
#include "GestureRecognizer.h"

using namespace cvg;

void bind_handler(EventType eventType, ActionHandler actionHandler)
{
	auto& em = GestureRecognizer::EVENT_MAP;
	assert(em.find(eventType) == em.end());
	em[eventType] = actionHandler;
}

void bind_handler_default()
{
	bind_handler(EventType::MOVE_LEFT, [](HandData handData){
		HWND hwnd = GetForegroundWindow();
		::SendMessage(hwnd, WM_SYSKEYDOWN, VK_LEFT, 0);
		::SendMessage(hwnd, WM_SYSKEYUP, VK_LEFT, 0);
		printf("Move Left.\n");
	});

	bind_handler(EventType::MOVE_RIGHT, [](HandData handData){
		HWND hwnd = GetForegroundWindow();
		::SendMessage(hwnd, WM_SYSKEYDOWN, VK_RIGHT, 0);
		::SendMessage(hwnd, WM_SYSKEYUP, VK_RIGHT, 0);
		printf("Move Right.\n");
	});

	bind_handler(EventType::MOVE_UP, [](HandData handData){
		HWND hwnd = GetForegroundWindow();
		::SendMessage(hwnd, WM_SYSKEYDOWN, VK_UP, 0);
		::SendMessage(hwnd, WM_SYSKEYUP, VK_UP, 0);
		printf("Move Up.\n");
	});

	bind_handler(EventType::MOVE_DOWN, [](HandData handData){
		HWND hwnd = GetForegroundWindow();
		::SendMessage(hwnd, WM_SYSKEYDOWN, VK_DOWN, 0);
		::SendMessage(hwnd, WM_SYSKEYUP, VK_DOWN, 0);
		printf("Move Down.\n");
	});

	bind_handler(EventType::PALM_OPEN, [](HandData handData){
		HWND hwnd = GetForegroundWindow();
		::SendMessage(hwnd, WM_SYSKEYDOWN, VK_RETURN, 0);
		::SendMessage(hwnd, WM_SYSKEYUP, VK_RETURN, 0);
		printf("Palm Open.\n");
	});

	bind_handler(EventType::PALM_CLOSE, [](HandData handData){
		HWND hwnd = GetForegroundWindow();
		::SendMessage(hwnd, WM_SYSKEYDOWN, VK_RETURN, 0);
		::SendMessage(hwnd, WM_SYSKEYUP, VK_RETURN, 0);
		printf("Palm Closed.\n");
	});
}
