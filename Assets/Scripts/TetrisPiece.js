var OP = require('./node_modules/OPengine/OPengine');
var SHAPES = require('./Shapes');

function Attribute(name, size) { return { Id: name, Size: size }; }

////////
// TetrisPiece
///////////////////////////
function TetrisPiece(height) {
	this.topLeft.row = height;
	this.topLeft.col = 4;
	var rand = OP.math.Rand();
	console.log('Randome:', rand);
	var ind = OP.math.Floor(rand * 7);
	console.log('Index', ind);
	this.Setup(ind);
}

TetrisPiece.prototype = {
	landed: false,
	color: 0,
	type: 0,
	shape: [],
	topLeft: { row:0, col: 0 },
	potentialTopLeft: { row: 0, col: 0 },
	rotations:[],
	rotation: 0,

	Copy: function(arr) {
		this.rotations = [];
		for(var k = 0; k < arr.length; k++) {
			var rotation = [];
			for(var i = 0; i < arr[k].length;i++) {
				var r = [];
				for(var j  = 0; j < arr[k][i].length; j++) {
					r.push(arr[k][i][j] + 0);
				}
				rotation.push(r);
			}
			this.rotations.push(rotation);
		}
	},
	Setup: function(item) {

		var special = 0, block = 0;

		if(OP.math.Rand() > 0.5) {
			var sp = OP.math.Floor(OP.math.Rand() * 2);
			special = 9 + sp;
			block = OP.math.Floor(OP.math.Rand() * 4);
		}

		switch(item) {
			case 0: {
				this.Copy(SHAPES.Square);
				this.shape = this.rotations[0];
				if(special) {
					switch(block){
						case 0: {
							this.rotations[0][0][0] = special;
							this.rotations[1][0][1] = special;
							this.rotations[2][1][1] = special;
							this.rotations[3][1][0] = special;							
							break;
						}
						case 1: {
							this.rotations[0][0][1] = special;
							this.rotations[1][1][1] = special;
							this.rotations[2][1][0] = special;
							this.rotations[3][0][0] = special;							
							break;
						}
						case 2: {
							this.rotations[0][1][1] = special;
							this.rotations[1][1][0] = special;
							this.rotations[2][0][0] = special;
							this.rotations[3][0][1] = special;							
							break;
						}
						case 3: {
							this.rotations[0][1][0] = special;
							this.rotations[1][0][0] = special;
							this.rotations[2][0][1] = special;
							this.rotations[3][1][1] = special;							
							break;
						}
					}
				}
				break;
			}
			case 1: {
				this.Copy(SHAPES.L);
				this.shape = this.rotations[0];
				if(special) {
					switch(block){
						case 0: {
							this.rotations[0][0][0] = special;
							this.rotations[1][0][2] = special;
							this.rotations[2][2][1] = special;
							this.rotations[3][1][0] = special;							
							break;
						}
						case 1: {
							this.rotations[0][1][0] = special;
							this.rotations[1][0][1] = special;
							this.rotations[2][1][1] = special;
							this.rotations[3][1][1] = special;							
							break;
						}
						case 2: {
							this.rotations[0][2][0] = special;
							this.rotations[1][0][0] = special;
							this.rotations[2][0][1] = special;
							this.rotations[3][1][2] = special;							
							break;
						}
						case 3: {
							this.rotations[0][2][1] = special;
							this.rotations[1][1][0] = special;
							this.rotations[2][0][0] = special;
							this.rotations[3][0][2] = special;							
							break;
						}
					}
				}
				break;
			}
			case 2: {
				this.Copy(SHAPES.J);
				this.shape = this.rotations[0];
				if(special) {
					switch(block){
						case 0: {
							this.rotations[0][0][1] = special;
							this.rotations[1][1][2] = special;
							this.rotations[2][2][0] = special;
							this.rotations[3][0][0] = special;							
							break;
						}
						case 1: {
							this.rotations[0][1][1] = special;
							this.rotations[1][1][1] = special;
							this.rotations[2][1][0] = special;
							this.rotations[3][0][1] = special;							
							break;
						}
						case 2: {
							this.rotations[0][2][1] = special;
							this.rotations[1][1][0] = special;
							this.rotations[2][0][0] = special;
							this.rotations[3][0][2] = special;							
							break;
						}
						case 3: {
							this.rotations[0][2][0] = special;
							this.rotations[1][0][0] = special;
							this.rotations[2][0][1] = special;
							this.rotations[3][1][2] = special;							
							break;
						}
					}
				}
				break;
			}
			case 3: {
				this.shape = SHAPES.Long[0];
				this.rotations = SHAPES.Long;
				break;
			}
			case 4: {
				this.shape = SHAPES.Triangle[0];
				this.rotations = SHAPES.Triangle;
				break;
			}
			case 5: {
				this.shape = SHAPES.Z[0];
				this.rotations = SHAPES.Z;
				break;
			}
			case 6: {
				this.shape = SHAPES.N[0];
				this.rotations = SHAPES.N;
				break;
			}
			default: {
				this.shape = SHAPES.Square[0];
				this.rotations = SHAPES.Square;
				break;
			}
		}
	},	
	Rotate: function() {
		this.rotation++;
		if(this.rotation == this.rotations.length)
			this.rotation = 0;
        this.shape = this.rotations[this.rotation];
	},

	RotateBack: function() {
		this.rotation--;
		if(this.rotation < 0)
			this.rotation = this.rotations.length - 1;
		this.shape = this.rotations[this.rotation];
	},

	Initialize: function() {

	},
		
	Update: function(elapsed) {
		
		return 0;
	},
	
	Render: function(camera) {
		OP.render.Mesh(camera);
	},
	
	Destroy: function() {
		
	}
};

module.exports = TetrisPiece;