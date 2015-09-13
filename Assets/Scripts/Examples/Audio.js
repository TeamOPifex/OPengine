var OP = require('OPengine');

function Initialize() {
	return 1;
}

function Update() {
	OP.render.Clear(0.0, 0.0, 0.0);
	OP.render.Present();
	return 0;
}

function Exit() {
	return 1;
}

module.exports = OP.gameState.Create(Initialize, Update, Exit);