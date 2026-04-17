#define _CRT_SECURE_NO_WARNINGS
#include "Input.h"

using namespace std;

Action Input::ReadInput()
{
	string input;
	getline(cin, input);

	if (input.empty()) return Action::None;

	unsigned char c = tolower(input[0]);

	switch (c)
	{
	case 'w': return Action::MoveUp;
	case 's': return Action::MoveDown;
	case 'a': return Action::MoveLeft;
	case 'd': return Action::MoveRight;
	case 'e': return Action::Interact;
	case 'q': return Action::Quit;
	default: return Action::None;
	}
}
