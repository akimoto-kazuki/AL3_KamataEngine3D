#pragma once
#include <Windows.h>
class Mouse 
{
public:
	void Update(HWND hwnd);
	POINT GetPosition() const { return m_pos; }

private:
	POINT m_pos = {0, 0};
};
