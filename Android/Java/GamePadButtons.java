
package com.opifex;

public enum GamePadButtons {
	DPAD_UP(0),
	DPAD_DOWN(1),
	DPAD_LEFT(2),
	DPAD_RIGHT(3),
	START(4),
	BACK(5),
	LEFT_THUMB(6),
	RIGHT_THUMB(7),
	LEFT_SHOULDER(8),
	RIGHT_SHOULDER(9),
	A(10),
	B(11),
	X(12),
	Y(13),
	Max(14);
	
	private int code;
	 
	private GamePadButtons(int c) {
		code = c;
	}
 
	public int getCode() {
		return code;
	}
}