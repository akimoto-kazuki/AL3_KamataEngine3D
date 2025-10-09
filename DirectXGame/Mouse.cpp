#include "Mouse.h"

void Mouse::Update(HWND hwnd) 
{ 
	GetPhysicalCursorPos(&m_pos); 
	ScreenToClient(hwnd, &m_pos);
}