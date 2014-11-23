package com.opifex;

// Button States
// 0 - Not pressed
// 1 - Down
// 2 - Released

public class ControllerState{
	public ControllerState(int val) {
		button_A = val;
	}
	
	private int button_A;
	private int button_B;
	private int button_X;
	private int button_Y;
}