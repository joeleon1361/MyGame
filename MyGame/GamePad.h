#pragma once

#ifndef INPUT_H_
#define INPUT_H_

// �{�^���̎��
enum ButtonKind
{
	UpButton,
	DownButton,
	LeftButton,
	RightButton,
	Button_A,
	Button_B,
	Button_X,
	Button_Y,
	Button_LB,
	Button_RB,
	Button_LeftMenu,
	Button_RightMenu,
	Button_LeftStick,
	Button_RightStick,
	ButtonKindMax,
};

enum ButtonState
{
	ButtonStateNone,
	ButtonStateDown,
	ButtonStatePush,
	ButtonStateUp,
	ButtonStateMax,
};

// ������
bool InitInput();

// �I������
void ReleaseInput();

// �X�V����
void UpdateInput();

// �{�^����������Ă��邩�̔���
bool IsButtonPush(ButtonKind button);

// �{�^���̗������u�Ԃ𔻒�
bool IsButtonUp(ButtonKind button);

// �{�^���̉������u�Ԃ̔���
bool IsButtonDown(ButtonKind button);

#endif
