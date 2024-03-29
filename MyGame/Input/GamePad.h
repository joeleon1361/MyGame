#pragma once

#ifndef INPUT_H_
#define INPUT_H_

// ボタンの種類
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

// 初期化
bool InitInput();

// 終了処理
void ReleaseInput();

// 更新処理
void UpdateInput();

// ボタンが押されているかの判定
bool IsButtonPush(ButtonKind button);

// ボタンの離した瞬間を判定
bool IsButtonUp(ButtonKind button);

// ボタンの押した瞬間の判定
bool IsButtonDown(ButtonKind button);

#endif
