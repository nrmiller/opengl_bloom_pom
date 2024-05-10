#pragma once
class Keyboard
{
public:
	Keyboard();
	~Keyboard();
	bool IsPressed(int vkCode);
	void SetPressed(int vkCode, bool pressed);
	void Print();
private:
	bool keys[256];
};

