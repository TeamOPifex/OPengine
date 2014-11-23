
package com.opifex;
	
public enum GamePadButtonState {
	UP(0),
	DOWN(1);
	
	private int code;
	 
	private GamePadButtonState(int c) {
		code = c;
	}
 
	public int getCode() {
		return code;
	}
}