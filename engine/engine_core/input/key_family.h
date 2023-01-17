#pragma once

constexpr unsigned int MAGE_KEYFAMILY_FULL = 0xffffffffu;
//character
#define MAGE_KEYFAMILY_CHARACTER_A 0x00000001u
#define MAGE_KEYFAMILY_CHARACTER_B 0x00000002u
#define MAGE_KEYFAMILY_CHARACTER_C 0x00000004u
#define MAGE_KEYFAMILY_CHARACTER_D 0x00000008u
#define MAGE_KEYFAMILY_CHARACTER_E 0x00000010u
#define MAGE_KEYFAMILY_CHARACTER_F 0x00000020u
#define MAGE_KEYFAMILY_CHARACTER_G 0x00000040u
#define MAGE_KEYFAMILY_CHARACTER_H 0x00000080u
#define MAGE_KEYFAMILY_CHARACTER_I 0x00000100u
#define MAGE_KEYFAMILY_CHARACTER_J 0x00000200u
#define MAGE_KEYFAMILY_CHARACTER_K 0x00000400u
#define MAGE_KEYFAMILY_CHARACTER_L 0x00000800u
#define MAGE_KEYFAMILY_CHARACTER_M 0x00001000u
#define MAGE_KEYFAMILY_CHARACTER_N 0x00002000u
#define MAGE_KEYFAMILY_CHARACTER_O 0x00004000u
#define MAGE_KEYFAMILY_CHARACTER_P 0x00008000u
#define MAGE_KEYFAMILY_CHARACTER_Q 0x00010000u
#define MAGE_KEYFAMILY_CHARACTER_R 0x00020000u
#define MAGE_KEYFAMILY_CHARACTER_S 0x00040000u
#define MAGE_KEYFAMILY_CHARACTER_T 0x00080000u
#define MAGE_KEYFAMILY_CHARACTER_U 0x00100000u
#define MAGE_KEYFAMILY_CHARACTER_V 0x00200000u
#define MAGE_KEYFAMILY_CHARACTER_W 0x00400000u
#define MAGE_KEYFAMILY_CHARACTER_X 0x00800000u
#define MAGE_KEYFAMILY_CHARACTER_Y 0x01000000u
#define MAGE_KEYFAMILY_CHARACTER_Z 0x02000000u

#define MAGE_KEYFAMILY_CHARACTER_UP			0x04000000u
#define MAGE_KEYFAMILY_CHARACTER_DOWN		0x08000000u
#define MAGE_KEYFAMILY_CHARACTER_LEFT		0x10000000u
#define MAGE_KEYFAMILY_CHARACTER_RIGHT		0x20000000u

#define MAGE_KEYFAMILY_CHARACTER_SPACE		0x40000000u

#define MAGE_KEYFAMILY_CHARACTER_CAPS_LOCK	0x80000000u

//number
#define MAGE_KEYFAMILY_NUMBER_1 0x00000001u
#define MAGE_KEYFAMILY_NUMBER_2 0x00000002u
#define MAGE_KEYFAMILY_NUMBER_3 0x00000004u
#define MAGE_KEYFAMILY_NUMBER_4 0x00000008u
#define MAGE_KEYFAMILY_NUMBER_5 0x00000010u
#define MAGE_KEYFAMILY_NUMBER_6 0x00000020u
#define MAGE_KEYFAMILY_NUMBER_7 0x00000040u
#define MAGE_KEYFAMILY_NUMBER_8 0x00000080u
#define MAGE_KEYFAMILY_NUMBER_9 0x00000100u
#define MAGE_KEYFAMILY_NUMBER_0 0x00000200u

#define MAGE_KEYFAMILY_NUMBER_NUM_LOCK 0x00000400u

#define MAGE_KEYFAMILY_NUMBER_PLUS 0x00000800u
#define MAGE_KEYFAMILY_NUMBER_MINUS 0x00001000u
#define MAGE_KEYFAMILY_NUMBER_MULTIPLY 0x00002000u
#define MAGE_KEYFAMILY_NUMBER_DIVIDE 0x00004000u
#define MAGE_KEYFAMILY_NUMBER_PERIOD 0x00008000u

#define MAGE_KEYFAMILY_NUMBER_ALPHA1 0x00010000u
#define MAGE_KEYFAMILY_NUMBER_ALPHA2 0x00020000u
#define MAGE_KEYFAMILY_NUMBER_ALPHA3 0x00040000u
#define MAGE_KEYFAMILY_NUMBER_ALPHA4 0x00080000u
#define MAGE_KEYFAMILY_NUMBER_ALPHA5 0x00100000u
#define MAGE_KEYFAMILY_NUMBER_ALPHA6 0x00200000u
#define MAGE_KEYFAMILY_NUMBER_ALPHA7 0x00400000u
#define MAGE_KEYFAMILY_NUMBER_ALPHA8 0x00800000u
#define MAGE_KEYFAMILY_NUMBER_ALPHA9 0x01000000u
#define MAGE_KEYFAMILY_NUMBER_ALPHA0 0x02000000u

//others
#define MAGE_KEYFAMILY_FUNCTION_1 0x00000001u
#define MAGE_KEYFAMILY_FUNCTION_2 0x00000002u
#define MAGE_KEYFAMILY_FUNCTION_3 0x00000004u
#define MAGE_KEYFAMILY_FUNCTION_4 0x00000008u
#define MAGE_KEYFAMILY_FUNCTION_5 0x00000010u
#define MAGE_KEYFAMILY_FUNCTION_6 0x00000020u
#define MAGE_KEYFAMILY_FUNCTION_7 0x00000040u
#define MAGE_KEYFAMILY_FUNCTION_8 0x00000080u
#define MAGE_KEYFAMILY_FUNCTION_9 0x00000100u
#define MAGE_KEYFAMILY_FUNCTION_10 0x00000200u
#define MAGE_KEYFAMILY_FUNCTION_11 0x00000400u
#define MAGE_KEYFAMILY_FUNCTION_12 0x00000800u

#define MAGE_KEYFAMILY_TAB 0x00001000u
#define MAGE_KEYFAMILY_LEFT_SHIFT 0x00002000u
#define MAGE_KEYFAMILY_RIGHT_SHIFT 0x00004000u
#define MAGE_KEYFAMILY_LEFT_ALT 0x00008000u
#define MAGE_KEYFAMILY_RIGHT_ALT 0x00010000u
#define MAGE_KEYFAMILY_LEFT_CONTROL 0x00020000u
#define MAGE_KEYFAMILY_RIGHT_CONTROL 0x00040000u
#define MAGE_KEYFAMILY_DELETE 0x00080000u
#define MAGE_KEYFAMILY_BACKSPACE 0x00100000u

//mouse
#define MAGE_MOUSEFAMILY_BUTTON01 0x00000001u
#define MAGE_MOUSEFAMILY_BUTTON02 0x00000002u
#define MAGE_MOUSEFAMILY_BUTTON03 0x00000004u
#define MAGE_MOUSEFAMILY_BUTTON04 0x00000008u
#define MAGE_MOUSEFAMILY_BUTTON05 0x00000010u
#define MAGE_MOUSEFAMILY_BUTTON06 0x00000020u
#define MAGE_MOUSEFAMILY_BUTTON07 0x00000040u
#define MAGE_MOUSEFAMILY_BUTTON08 0x00000080u

#define MAGE_MOUSEFAMILY_PRIMARY MAGE_MOUSEFAMILY_BUTTON01
#define MAGE_MOUSEFAMILY_SECONDARY MAGE_MOUSEFAMILY_BUTTON02
#define MAGE_MOUSEFAMILY_MIDDLE MAGE_MOUSEFAMILY_BUTTON03


namespace Mage {
	enum InputMode {
		NONE=0,
		CAPS_LOCK = 1,
		NUM_LOCK = 2,
		CURSOR_DISABLE = 4
	};

	enum KeyCode {
		A = 0, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		UpArrow, DownArrow, LeftArrow, RightArrow, Space, CapsLock,
		Keypad1, Keypad2, Keypad3, Keypad4, Keypad5, Keypad6, Keypad7, Keypad8, Keypad9, Keypad0,
		NumLock, KeypadPlus, KeypadMinus, KeypadMultiply, KeypadDivide, KeypadPeriod,
		Alpha1, Alpha2, Alpha3, Alpha4, Alpha5, Alpha6, Alpha7, Alpha8, Alpha9, Alpha0,
		F1 = 64, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, 
		Tab, LeftShift, RightShift, LeftAlt, RightAlt, LeftControl, RightControl, Delete, Backspace
	};
}
