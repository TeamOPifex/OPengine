var OP = require('./node_modules/OPengine/OPengine');

var TetrisPiece = require('./TetrisPiece');

function Attribute(name, size) { return { Id: name, Size: size }; }

////////
// GameBoard
///////////////////////////
function GameBoard() {

}

GameBoard.prototype = {
	manager: null,
	assets: {},
	scratchMat4: null,
	landed: [],
	boardWidth: 10,
	boardHeight: 24,
	lightDir: null,
    camera: null,
	cameraPos: { x: 0, y: 11.5, z: 27 },
    cameraTarget: { x: 0, y: 11.5, z: 0 },
	currentPiece: null,
    nextPiece: null,
	lastDrop: 0,
    dropTime: 500,
    lastPush: 0,
	movedLeft: 0,
	movedRight: 0,
	moveRepeatRate: 100,
	score: 0,
	paused: false,
	showSlow: 0,
	characterPos: { x: 0, y: 12, z: 0 },
    canJump: false,
	velocityY: 0,
    dir: 1,
    effects: [],
    lost: false,
    
    Load: function(name, asset) {
        OP.cman.Load(asset);
        this.assets[name] = OP.cman.Get(asset);
    },
	
	Initialize: function() {
        
        OP.sprite3D.Init();
        OP.cman.Load('Effects.opss');
        this.assets['pow'] = OP.cman.Get('Effects/Pow');
        this.assets['effects'] = OP.sprite3D.Create([this.assets['pow']]);
        
        this.Load('dude', 'TetrisDude.opm');
        this.Load('dudeTex', 'TetrisDudeTex.png');
        this.Load('base', 'TetrisBase.opm');
        this.Load('baseTex', 'TetrisBase.png');
        this.Load('wall', 'TetrisWall.opm');
        this.Load('wallTex', 'TetrisWall.png');
        this.Load('model', 'PuzzleBlock.opm');
        this.Load('image', 'PuzzleBlockGear.png');
        this.Load('TetrisRed', 'TetrisRed.png');
        this.Load('TetrisYellow', 'TetrisYellow.png');
        this.Load('TetrisBlue', 'TetrisBlue.png');
        this.Load('TetrisGreen', 'TetrisGreen.png');
        this.Load('TetrisGray', 'TetrisGray.png');
        this.Load('TetrisOrange', 'TetrisOrange.png');
        this.Load('TetrisPurple', 'TetrisPurple.png');
        this.Load('TetrisExplode', 'TetrisExplode.png');
        this.Load('TetrisSlow', 'TetrisSlow.png');
        this.Load('TetrisBroken', 'TetrisBroken.png');
        this.Load('TetrisFlawed', 'TetrisFlawed.png');
        this.Load('font', 'comicbook.opf');

        this.Load('frag', 'TexturedModel.frag');
        this.Load('vert', 'TexturedModel.vert');
		this.assets['effect'] = OP.effect.Create({
			Name: 'Model Effect', 
			Vertex: this.assets['vert'],
			Fragment: this.assets['frag'],
			Stride: 32,
			Attributes: [ Attribute('aPosition', 3), Attribute('aNormal', 3), Attribute('aUV', 2) ] 
		});
		
		this.manager = OP.font.CreateManager(this.assets['font']);
		this.manager.SetRGBA(1,0,0,1);
		this.manager.SetAlign(1);
		this.manager.Bind();
        
        
        this.camera = OP.camera.Create(0, 14, 10, 0, 5, 0);
        this.scratchMat4 = OP.mat4.Create();

		this.lightDir = OP.vec3.Create(1, 1, 1);

		this.landed = [];
		for(var y = 0; y < this.boardHeight; y++) {
			var row = [];
			for(var x = 0; x < this.boardWidth; x++) {
				row.push(0);
			}
			this.landed.push(row);
        }

		this.currentPiece = new TetrisPiece(this.boardHeight - 1);
		this.nextPiece = new TetrisPiece(this.boardHeight - 1);

		console.log('Finished Initialize');
	},
		
	LandIt: function() {
		for (var row = 0; row < this.currentPiece.shape.length; row++) {
			for (var col = 0; col < this.currentPiece.shape[row].length; col++) {
				if(this.currentPiece.shape[row][col] == 0) continue;
				this.landed[this.currentPiece.topLeft.row - row][col + this.currentPiece.topLeft.col] = this.currentPiece.shape[row][col];
			}
		}
		this.score++;
		this.currentPiece = this.nextPiece;
        this.nextPiece = new TetrisPiece(this.boardHeight - 1);
        
        this.currentPiece.potentialTopLeft.row = this.currentPiece.topLeft.row;
        this.currentPiece.potentialTopLeft.col = this.currentPiece.topLeft.col;

        if (this.HasCollision(false)) {
            this.lost = true;
            console.log('Lose Condition!');
        }

		var lines = 0;
		var slowed = false;
		for(var i = 0; i < this.boardHeight; i++) {
			var all = true;
			var hasSlow = false;
			for(var j = 0; j < this.boardWidth; j++) {
				if(this.landed[i][j] == 0 || this.landed[i][j] == 10) { 
					all = false;
					break;
				}
				if(this.landed[i][j] == 9) {
					hasSlow = true;
				}
			}
			if(all) {
				if(hasSlow) {
					slowed = true;
					this.dropTime *= 1.5;
					this.showSlow = 500;
				}
				lines++;
				for(var k = i; k < this.boardHeight - 1; k++) {
					for(var j = 0; j < this.boardWidth; j++) {
						this.landed[k][j] = this.landed[k + 1][j];
					}
				}
				for(var j = 0; j < this.boardWidth; j++) {
					this.landed[this.boardHeight - 1][j] = 0;
				}
				i--;
			}
		}

		if(lines) {
			if(!slowed) this.dropTime *= 0.85;
			var amnt = 10;
			for(var i = 0; i < lines; i++) amnt *= 2;
			this.score += amnt;
		}
	},

	HasCollision: function(rotation) {

		for (var row = 0; row < this.currentPiece.shape.length; row++) {
			for (var col = 0; col < this.currentPiece.shape[row].length; col++) {
				if(this.currentPiece.shape[row][col] == 0) continue;

				var r = this.currentPiece.potentialTopLeft.row - row;
				var c = this.currentPiece.potentialTopLeft.col + col;

				var endC = c + this.currentPiece.shape[0].length - 2;
				if(rotation) {	
					console.log('Width: ' + this.currentPiece.shape[0].length, this.boardWidth, c, endC);
				}
				if(rotation && (endC > this.boardWidth) ) {
					console.log('HIT!');
					return true;
				}

                if (r < 0) {
                    console.log(r, c);
					return true;
				} else {
					if(r < 0 || c < 0) {
						return false;
					}
					var p = this.landed[r][c];
					if( p != 0) {
                        console.log('hit:', r, c, this.currentPiece.potentialTopLeft.row, this.currentPiece.potentialTopLeft.col);
						return true;
					}
				}
			}
		}

		return false;
	},

	DropOnce: function() {

		var acceptLocation = true;

		this.currentPiece.potentialTopLeft.row = this.currentPiece.topLeft.row;
		this.currentPiece.potentialTopLeft.col = this.currentPiece.topLeft.col;

		this.currentPiece.potentialTopLeft.row--;
		if(this.currentPiece.potentialTopLeft.row - this.currentPiece.shape.length < 0) {
			this.LandIt();
			acceptLocation = false;
		}

		if(acceptLocation) {
			if(this.HasCollision()) {
				this.LandIt();
				acceptLocation = false;
			}
		}

		if(acceptLocation) {
			this.currentPiece.topLeft.row = this.currentPiece.potentialTopLeft.row;
			this.currentPiece.topLeft.col = this.currentPiece.potentialTopLeft.col;
			return false;
		}

		return true;
	},

	SetPieceTexture: function(piece) {
		OP.render.ClearActiveTextures();

		switch(piece){
			case 1: {
				OP.render.ParamTexture('uColorTexture', this.assets['TetrisRed']);
				break;
			}
			case 2: {
				OP.render.ParamTexture('uColorTexture', this.assets['TetrisYellow']);
				break;
			}
			case 3: {
				OP.render.ParamTexture('uColorTexture', this.assets['TetrisBlue']);
				break;
			}
			case 4: {
				OP.render.ParamTexture('uColorTexture', this.assets['TetrisGreen']);
				break;
			}
			case 5: {
				OP.render.ParamTexture('uColorTexture', this.assets['TetrisGray']);
				break;
			}
			case 6: {
				OP.render.ParamTexture('uColorTexture', this.assets['TetrisOrange']);
				break;
			}
			case 7: {
				OP.render.ParamTexture('uColorTexture', this.assets['TetrisPurple']);
				break;
			}
			case 8: {
				OP.render.ParamTexture('uColorTexture', this.assets['TetrisExplode']);
				break;
			}
			case 9: {
				OP.render.ParamTexture('uColorTexture', this.assets['TetrisSlow']);
				break;
			}
			case 10: {
				OP.render.ParamTexture('uColorTexture', this.assets['TetrisFlawed']);
				break;
			}
			case 11: {
				OP.render.ParamTexture('uColorTexture', this.assets['TetrisBroken']);
				break;
			}
			default: {
				OP.render.ParamTexture('uColorTexture', this.assets['TetrisPurple']);
			}
		}
	},

	////////////////
	// UPDATE
	////////////////////////////////
    Update: function (elapsed) {

		if(OP.input.KeyWasPressed(OP.KEYS.ENTER))
			this.paused = !this.paused;

		if(this.paused || this.lost) {
			this.Render();
			return 0;
		}


		this.lastDrop -= elapsed;
		if(this.lastDrop < 0) {
			this.lastDrop = this.dropTime;
			if(this.currentPiece != null) {
				this.DropOnce();
			}
		}

		if(OP.input.KeyWasPressed(OP.KEYS.SPACE)) {
			while(!this.DropOnce()) {

			}
		}

		this.movedLeft -= elapsed;
		this.movedRight -= elapsed;
		this.showSlow -= elapsed;

		if(this.currentPiece != null) {
			var acceptLocation = true;

			var change = false;
			var rotated = false;

			this.currentPiece.potentialTopLeft.row = this.currentPiece.topLeft.row;
			this.currentPiece.potentialTopLeft.col = this.currentPiece.topLeft.col;

			if(this.movedLeft < 0 && (OP.input.KeyIsDown(OP.KEYS.D) || OP.input.GamePadLeftThumbIs(1, OP.FACING.RIGHT) )) {
				this.movedLeft = this.moveRepeatRate;
				this.currentPiece.potentialTopLeft.col++;
				change = true;
				console.log('D');
			}

			if(this.movedRight < 0 && (OP.input.KeyIsDown(OP.KEYS.A) || OP.input.GamePadLeftThumbIs(1, OP.FACING.LEFT) )) {
				this.movedRight = this.moveRepeatRate;
				this.currentPiece.potentialTopLeft.col--;
				change = true;
				console.log('A');
			}

			if(OP.input.KeyWasPressed(OP.KEYS.Q) || OP.input.GamePadWasPressed(1, OP.BUTTONS.A)) {
				this.currentPiece.Rotate();
				rotated = true;
				change = true;
				console.log('Q');
			}

			if(change) {
				if(this.currentPiece.potentialTopLeft.col < 0) 
					acceptLocation = false;

				if(this.currentPiece.potentialTopLeft.col + this.currentPiece.shape[0].length > this.boardWidth) {
					acceptLocation = false;
					if(rotated) {
						this.currentPiece.RotateBack();
					}
				}

				if(acceptLocation) {
					if(this.HasCollision(rotated)) {
						acceptLocation = false;
						if(rotated) {
							this.currentPiece.RotateBack();
						}
					}
				}

				if(acceptLocation) {
					this.currentPiece.topLeft.row = this.currentPiece.potentialTopLeft.row;
					this.currentPiece.topLeft.col = this.currentPiece.potentialTopLeft.col;
				}
			}
        }

	    this.lastPush -= elapsed;
        if (this.lastPush < 0 && OP.input.KeyIsDown(OP.KEYS.S)) {
            this.lastPush = 100;
            this.DropOnce();
        }

		this.camera.SetPos(0, this.cameraPos.y, this.cameraPos.z);
		this.camera.SetTarget(0, this.cameraTarget.y, 0);
        
        var gridXStart = OP.math.Floor(this.characterPos.x + (this.boardWidth / 2) + 0.5);
        var gridYStart = OP.math.Floor(this.characterPos.y);

		this.characterPos.y -= elapsed * 0.0098 + this.velocityY; // gravity
		if (this.characterPos.y < 0) {
		    this.characterPos.y = 0;
            this.canJump = true;
		}

		this.velocityY += elapsed * 0.01;
		if(this.velocityY > 0) this.velocityY = 0;

		if(OP.input.GamePadWasPressed(0, OP.BUTTONS.A) && this.canJump) {
            this.velocityY = -1.0;
		    this.canJump = false;
		}

		var xChange = OP.input.GamePadLeftThumbX(0) * 0.01 * elapsed;
		this.characterPos.x += xChange;

		if(OP.input.GamePadLeftThumbIs(0, OP.FACING.LEFT)) {
			this.dir = -1;
		}
		if(OP.input.GamePadLeftThumbIs(0, OP.FACING.RIGHT)) {
			this.dir = 1;
		}

		this.gridX = OP.math.Floor(this.characterPos.x + (this.boardWidth / 2) + 0.5);
		this.gridY = OP.math.Floor(this.characterPos.y);

        if (gridXStart != this.gridX) {
            var currentRow = this.gridY + 1;
		    if (this.landed[currentRow][this.gridX] != 0) {
		        this.characterPos.x -= xChange;
		    }
            this.gridX = OP.math.Floor(this.characterPos.x + (this.boardWidth / 2) + 0.5);
            if (this.landed[currentRow][this.gridX] != 0) {
                this.characterPos.y = currentRow;
                this.canJump = true;
            }
		} else {
            if (this.landed[this.gridY + 1][this.gridX] != 0) {
                this.characterPos.y = this.gridY + 1;
                this.canJump = true;
            }
		}

        this.gridX = OP.math.Floor(this.characterPos.x + (this.boardWidth / 2) + 0.5);
        this.gridY = OP.math.Floor(this.characterPos.y);
        
        if (OP.input.GamePadWasPressed(0, OP.BUTTONS.B) && this.effects.length == 0 && (OP.input.GamePadLeftThumbIs(0, OP.FACING.DOWN) || OP.input.GamePadLeftThumbIs(0, OP.FACING.UP))) {
            
            var currentRow = this.gridY + 1;
            var currentColumn = this.gridX;
            if (OP.input.GamePadLeftThumbIs(0, OP.FACING.UP)) {
                currentRow += 1;
            } else {
                currentRow -= 1;
            }

            if (currentRow == 0) {
                
            } else if (this.landed[currentRow][currentColumn] == 10) {
                this.landed[currentRow][currentColumn] = 11;
            } else if (this.landed[currentRow][this.gridX + this.dir] == 11) {
            } else if (this.landed[currentRow][this.gridX + this.dir] != 0) {
                this.landed[currentRow][currentColumn] = 0;
                if (this.landed[currentRow][currentColumn] == 0) {
                    for (var k = currentRow; k < this.boardHeight - 1; k++) {
                        this.landed[k][currentColumn] = this.landed[k + 1][currentColumn];
                    }
                }
                this.effects.push({
                    x: currentColumn - (this.boardWidth / 2),
                    y: currentRow,
                    time: 1000
                });
            }

        } else if(OP.input.GamePadWasPressed(0, OP.BUTTONS.B) && this.effects.length == 0) {			
			var currentRow = this.gridY + 1;

			if(this.landed[currentRow][this.gridX + this.dir] == 10) {
                this.landed[currentRow][this.gridX + this.dir] = 11;
			} else
			if (this.landed[currentRow][this.gridX + this.dir] == 11) {
                } else if (this.landed[currentRow][this.gridX + this.dir] != 0) {
                    this.landed[currentRow][this.gridX + this.dir] = 0;
                    if (this.landed[currentRow][this.gridX + this.dir] == 0) {
                        for (var k = currentRow; k < this.boardHeight - 1; k++) {
                            this.landed[k][this.gridX + this.dir] = this.landed[k + 1][this.gridX + this.dir];
                        }
                    }
                    this.effects.push({
                        x: this.gridX + this.dir - (this.boardWidth / 2),
                        y: currentRow,
                        time: 1000
                    });
			}
        }
        
        for (var eff = 0; eff < this.effects.length; eff++) {
            this.effects[eff].time -= elapsed;
            if (this.effects[eff].time < 0) {
                this.effects = this.effects.splice(1, 0);
                eff--;
            }
        }
        

        this.Render();

		return { result: 0 };
	},
	
    Render: function () {

		OP.Clear(0.2,0.2,0.2);
		OP.Depth(1);		

		this.scratchMat4.Identity();
        
		OP.render.BindMesh(this.assets['model']);
		OP.render.BindEffect(this.assets['effect']);
        
		if(this.currentPiece != null) {
			for (var row = 0; row < this.currentPiece.shape.length; row++) {
				for (var col = 0; col < this.currentPiece.shape[row].length; col++) {
					if (this.currentPiece.shape[row][col] == 0)  continue;

					var x = this.currentPiece.topLeft.col + col - (this.boardWidth / 2);
					var y = this.currentPiece.topLeft.row - row;
					this.scratchMat4.SetVec3(x, y, 0);
					OP.render.ParamMat4v('uWorld', this.scratchMat4);
					
					this.SetPieceTexture(this.currentPiece.shape[row][col]);

					OP.render.Mesh(this.camera);
				
				}
			}
		}
        
		if(this.nextPiece != null) {
			for (var row = 0; row < this.nextPiece.shape.length; row++) {
				for (var col = 0; col < this.nextPiece.shape[row].length; col++) {
					if (this.nextPiece.shape[row][col] == 0)  continue;

					var x = col + 10;
					var y = 12 - row;

					this.scratchMat4.SetVec3(x, y, 0);
					OP.render.ParamMat4v('uWorld', this.scratchMat4);

					this.SetPieceTexture(this.nextPiece.shape[row][col]);
					
					OP.render.Mesh(this.camera);
				
				}
			}
		}

		for(var y = 0; y < this.boardHeight; y++) {
			for(var x = 0; x < this.boardWidth; x++) {
				var piece = this.landed[y][x];
				if(piece == 0) continue;
				this.scratchMat4.SetVec3(x - this.boardWidth / 2, y, 0);
				OP.render.ParamMat4v('uWorld', this.scratchMat4);
				
				this.SetPieceTexture(piece);

				OP.render.Mesh(this.camera);
			}
		}


		OP.render.BindMesh(this.assets['base']);
		OP.render.BindEffect(this.assets['effect']);
		this.scratchMat4.SetVec3(-0.5, 0.5, 0);
		OP.render.ParamMat4v('uWorld', this.scratchMat4);
		OP.render.ParamTexture('uColorTexture', this.assets['wallTex']);
		OP.render.ParamVec3('uLightDirection', this.lightDir.Id);
		OP.render.Mesh(this.camera);

		OP.render.BindMesh(this.assets['wall']);
		OP.render.BindEffect(this.assets['effect']);		
		OP.render.ParamTexture('uColorTexture', this.assets['wallTex']);
		for(var i = 1; i <= this.boardHeight; i++) {
			this.scratchMat4.SetVec3(-0.5, 0.5 + i, 0);
			OP.render.ParamMat4v('uWorld', this.scratchMat4);
			OP.render.Mesh(this.camera);
		}




		OP.render.BindMesh(this.assets['dude']);
		OP.render.BindEffect(this.assets['effect']);		
		OP.render.ParamTexture('uColorTexture', this.assets['dudeTex']);
		this.scratchMat4.SetRotY(this.dir * (OP.math.PI / 2));
		this.scratchMat4.Vec3(this.characterPos.x, this.characterPos.y + 0.5, this.characterPos.z);
		OP.render.ParamMat4v('uWorld', this.scratchMat4);
		OP.render.Mesh(this.camera);



 
		this.scratchMat4.SetVec3(-0.75, 0.75, 0);
		OP.font.RenderTextMat4('Score:' + this.score, this.scratchMat4);

		if(this.paused) {
			this.scratchMat4.SetVec3(0, 0, 0);
			this.scratchMat4.Scale(3,3,3);
			OP.font.RenderTextMat4('Paused', this.scratchMat4);
		}

		if(this.showSlow > 0) {
			this.scratchMat4.SetVec3(0, 0, 0);
			this.scratchMat4.Scale(3,3,3);
			OP.font.RenderTextMat4('Slower!', this.scratchMat4);
		}


		//this.scratchMat4.SetVec3(-0.75, 0.25, 0);
  //      OP.font.RenderTextMat4('X:' + this.gridX + ', Y:' + this.gridY, this.scratchMat4);
        
        for (var eff = 0; eff < this.effects.length; eff++) {
            var x = this.effects[eff].x;
            var y = this.effects[eff].y;
            OP.vec3.Set({ Id: this.assets['effects'].Position }, x, y, 0);
            this.assets['effects'].Render(this.camera);
        }
        
        if (this.lost) {
            this.scratchMat4.SetScale(3, 3, 3);
            this.scratchMat4.Vec3(0, .25, 0);
            OP.font.RenderTextMat4('Lost!', this.scratchMat4);
            
            this.scratchMat4.SetScale(3, 3, 3);
            this.scratchMat4.Vec3(0, -0.25, 0);
            OP.font.RenderTextMat4('Score: ' + this.score, this.scratchMat4);
        }

        OP.Present();
	},
	
	Destroy: function() {
		
	}
};

module.exports = GameBoard;