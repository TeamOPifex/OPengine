var OP = require('./node_modules/OPengine/OPengine');
var MenuState = require('./MenuState');

var state = null;

function Attribute(name, size) { return { Id: name, Size: size }; }

////////
// GAME SETUP
///////////////////////////

function Initialize() {
	OP.cman.Init('assets/');
	OP.render.Init(function(result) {
		
		OP.font.Init();
		
		state = new MenuState();
		state.Initialize();
		
	});
}

function Update(elapsed) {
	OP.input.Update();
	
	if(OP.input.KeyIsDown(OP.KEYS.ESCAPE)) return 1;
	
	if(state) {
        var r = state.Update(elapsed);
            if (r.state) {
                state = r.state;
        }
	    return r.result;
	}
		
	return 0;
}

function Destroy() {

}

OP.Start(Initialize, Update, Destroy);