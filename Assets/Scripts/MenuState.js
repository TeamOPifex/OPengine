var OP = require('./node_modules/OPengine/OPengine');
var TetrisBoard = require('./TetrisBoard');
function Attribute(name, size) { return { Id: name, Size: size }; }

////////
// MENU
///////////////////////////
function MenuState() {

}

MenuState.prototype = {
	manager: null,
	scratchMat4: null,
	selected: 0,
	assets: {},
	
	Initialize: function() {
		OP.cman.Load('comicbook.opf');
		this.assets['font'] = OP.cman.Get('comicbook.opf');
		
		this.manager = OP.font.CreateManager(this.assets['font']);
		this.manager.SetRGBA(1,0,0,1);
		this.manager.SetAlign(1);
		this.manager.Bind();
		
		OP.input.GamePadSetDeadzone(0, 0.2);

		this.scratchMat4 = OP.mat4.Create();
	},
	
	Update: function(elapsed) {
		
		if(this.selected == 1 && (OP.input.KeyIsDown(OP.KEYS.ENTER) || OP.input.KeyIsDown(OP.KEYS.SPACE))) return { result: 1 };
		if(this.selected == 0 && (OP.input.KeyIsDown(OP.KEYS.ENTER) || OP.input.KeyIsDown(OP.KEYS.SPACE))) {
			var state = new TetrisBoard();
			state.Initialize();
			return { result: 0, state: state };
		}
		
		if((OP.input.KeyWasPressed(OP.KEYS.W) || OP.input.KeyWasPressed(OP.KEYS.UP))) this.selected--;
		if((OP.input.KeyWasPressed(OP.KEYS.S) || OP.input.KeyWasPressed(OP.KEYS.DOWN))) this.selected++;
		
		if(this.selected < 0) this.selected = 0;
		if(this.selected > 1) this.selected = 1;
		
		this.Render();
		
		return { result: 0 };
	},
	
	Render: function() {
		OP.Clear(0.2,0.2,0.2);
		OP.Depth(1);
		
		this.scratchMat4.SetScale(2, 2, 2);
		this.scratchMat4.Vec3(0, 0.5, 0);
		OP.font.RenderTextMat4('The Assemblers', this.scratchMat4);
		
		this.scratchMat4.SetVec3(0, 0.0, 0);
		if(this.selected == 0) {
			this.scratchMat4.Scale(1.5, 1.5, 1.5);
		}
		OP.font.RenderTextMat4('Play', this.scratchMat4);
		
		this.scratchMat4.SetVec3(0, -0.25, 0);
		if(this.selected == 1) {
			this.scratchMat4.Scale(1.5, 1.5, 1.5);
		}
		OP.font.RenderTextMat4('Exit', this.scratchMat4);

		OP.Present();
	},
	
	Destroy: function() {
		
	}
};


module.exports = MenuState;