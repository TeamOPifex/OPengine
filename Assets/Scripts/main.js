var OP = require('OPengine');
	
var exampleSelectorState = require('./ExampleSelectorState.js');

function AppInitialize() {
	//OP.loaders.AddDefault();
	//OP.cman.Init();
	//OP.render.Init();
}

function AppUpdate(elapsed) {
	OP.keyboard.Update();

	if (OP.keyboard.WasReleased(OP.KEY.ESCAPE)) return 1;	
	if (OP.keyboard.WasReleased(OP.KEY.BACKSPACE) && OP.gameState.Active != exampleSelectorState) {
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
