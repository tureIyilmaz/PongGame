struct ButtonState
{
	bool isDown;
	bool changed;
};

enum {
	buttonUp,
	buttonDown,
	buttonW,
	buttonS,
	buttonLeft,
	buttonRight,
	buttonEnter,
	buttonCount,
};

struct Input {

	ButtonState buttons[buttonCount];
};