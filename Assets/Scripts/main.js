console.log('loading...');

var OP = require('OPengine');

var exampleSelectorState;

function AppInitialize() {
	OP.loaders.AddDefault();
	OP.loaders.AddOPS();
	OP.cman.Init('/Users/garretthoofman/OPengine/Assets');
	OP.render.Init();
}

function AppUpdate(elapsed) {
	OP.keyboard.Update();

	if (OP.keyboard.WasReleased(OP.KEY.ESCAPE)) return 1;	
	if (OP.keyboard.WasReleased(OP.KEY.BACKSPACE) && OP.gameState.Active != exampleSelectorState) {
		console.log('Changed');
		OP.gameState.Change(exampleSelectorState);
	}

	return OP.gameState.Active.Update(elapsed);
}

function AppDestroy() {
	//process.exit(1);
	return 1;
}

if(true) {
	AppInitialize();
	exampleSelectorState = require('./ExampleSelectorState.js');

	var timer = OP.timer.Create();
	OP.gameState.Change(exampleSelectorState);

	function loop() {
		OP.timer.Update(timer);
		if(AppUpdate(timer.elapsed)) {
			OP.gameState.Active.Exit();
			AppDestroy();
		} else {
			//setImmediate(loop);
			loop();
		}
	}
	loop();
} else {
	OP.start(
		Initialize,
		Update,
		Destroy
	);
}
