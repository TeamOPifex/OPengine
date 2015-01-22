var OP = require('OPengine');

// function Initialize() {
// 	print('SCRIPT INITIALIZED');	
// }

// function Update(elapsed) {
	
// 	OP.input.Update();

// 	if(OP.input.KeyIsDown(OP.KEYS.B)) return 1;

var amnt = 0;
var dir = 1;

console.log('Test1');

function clearToBlack(r, g, b) {
    //print('rgb:' + r + ' ' + g + ' ' + b);
	amnt += dir ? 0.1 : -0.01;
	if(amnt > 1.0 || amnt < 0.0) {
		dir = !dir;
	}
	OP.render.Clear(Math.sin(amnt),Math.cos(amnt),1);	

	//print('.');
		
	OP.render.Present();

	//return { test: 'Test' };
}

function run() {
	console.log('Test');
}
// 	return 0;
// }

// function Destroy() {

// }

// OP.Start(Initialize, Update, Destroy);