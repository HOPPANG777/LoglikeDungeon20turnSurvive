#define _CRT_SECURE_NO_WARNINGS
#include "Input.h"

using namespace std;

Action Input::ReadInput()
{
	string input;
	getline(cin, input);

	// 빈 입력은 None 처리
	if (input.empty()) return Action::ACTION_NONE;

	unsigned char c = tolower(input[0]);

	switch (c)
	{
	case 'w': return Action::ACTION_MOVE_UP;
	case 's': return Action::ACTION_MOVE_DOWN;
	case 'a': return Action::ACTION_MOVE_LEFT;
	case 'd': return Action::ACTION_MOVE_RIGHT;
	case 'e': return Action::ACTION_INTERACT;
	case 'h': return Action::ACTION_USE_POTION;
	case 'q': return Action::ACTION_QUIT;
	default:
		// ESC 입력 시 종료
		if (input.size() >= 1 && static_cast<unsigned char>(input[0]) == 27)
			return Action::ACTION_QUIT;
		return Action::ACTION_NONE;
	}
}
