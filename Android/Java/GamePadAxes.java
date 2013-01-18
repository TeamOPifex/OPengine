
package com.opifex.smrf;

public enum GamePadAxes {
	LS_X(0),
	LS_Y(1),
	RS_X(2),
	RS_Y(3),
	L2(4),
	R2(5),
	Max(6);
	
	private int code;
	 
	private GamePadAxes(int c) {
		code = c;
	}
 
	public int getCode() {
		return code;
	}
}	