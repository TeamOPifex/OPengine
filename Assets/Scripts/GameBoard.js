var OP = require('./node_modules/OPengine/OPengine');
function Attribute(name, size) { return { Id: name, Size: size }; }

////////
// GameBoard
///////////////////////////
function GameBoard() {

}

GameBoard.prototype = {
	manager: null,
	selected: 0,
	assets: {},
	camera: null,
	world: null,
	scratchMat4: null,
	pieces: [],
	boardWidth: 80,
	spinDir: -1,
	sheepRot: 0,
	lightDir: null,
	cameraPos: null,
	cameraPosY: 5,
	
	Initialize: function() {
				
		OP.cman.Load('GroundCube.opm');
		OP.cman.Load('GroundCubeGrass.opm');
		OP.cman.Load('Sheep.opm');
		OP.cman.Load('GroundCubeTex.png');
		OP.cman.Load('TexturedModel.frag');
		OP.cman.Load('TexturedModel.vert');
		OP.cman.Load('ColorModel.frag');
		OP.cman.Load('ColorModel.vert');
		
		this.assets['model'] = OP.cman.Get('GroundCube.opm');
		this.assets['grass'] = OP.cman.Get('GroundCubeGrass.opm');
		this.assets['sheep'] = OP.cman.Get('Sheep.opm');
		this.assets['image'] = OP.cman.Get('GroundCubeTex.png');
		this.assets['frag'] = OP.cman.Get('TexturedModel.frag');
		this.assets['vert'] = OP.cman.Get('TexturedModel.vert');
		this.assets['fragColor'] = OP.cman.Get('ColorModel.frag');
		this.assets['vertColor'] = OP.cman.Get('ColorModel.vert');
		
		this.assets['effect'] = OP.effect.Create({
			Name: 'Model Effect', 
			Vertex: this.assets['vert'],
			Fragment: this.assets['frag'],
			Stride: 32,
			Attributes: [ Attribute('aPosition', 3), Attribute('aNormal', 3), Attribute('aUV', 2) ] 
		});
		
		this.assets['colorEffect'] = OP.effect.Create({
			Name: 'Color Effect', 
			Vertex: this.assets['vertColor'],
			Fragment: this.assets['fragColor'],
			Stride: 36,
			Attributes: [ Attribute('aPosition', 3), Attribute('aNormal', 3), Attribute('aColor', 3) ] 
		});
		
		this.camera = OP.camera.Create(0,5,10, 0, 0, 0);		
		this.world = OP.mat4.Create();
		this.scratchMat4 = OP.mat4.Create();
		
		OP.cman.Load('comicbook.opf');
		this.assets['font'] = OP.cman.Get('comicbook.opf');
		
		this.manager = OP.font.CreateManager(this.assets['font']);
		this.manager.SetRGBA(1,0,0,1);
		this.manager.SetAlign(1);
		this.manager.Bind();
		
		this.lightDir = OP.vec3.Create(0, 1, 0);
		this.cameraPos = OP.vec3.Create(0, 5, 10);
		
		for(var x = 0; x < 8; x++) {
			for(var z = 0; z < 8; z++) {
				var piece = {
					x: x - 4,
					y: 0,
					z: z - 4,
					stuck: false,
					rotation: OP.math.Rand(),
					grass: OP.math.Rand() > 0.5
				};
				
				this.pieces.push(piece);
			}
		}
	},
		
	Update: function(elapsed) {
		
		if(OP.math.Rand() > 0.99) this.spinDir *= -1;		
		this.sheepRot += elapsed * this.spinDir * 0.001;
		
		if(OP.input.KeyIsDown(OP.KEYS.S)) this.cameraPosY -= elapsed * 0.01;
		if(OP.input.KeyIsDown(OP.KEYS.W)) this.cameraPosY += elapsed * 0.01;
		
		this.cameraPos.Set(0, this.cameraPosY, 10);
		OP.camera.SetPos(this.camera, this.cameraPos.Id);
		
		this.Render();
		
		return 0;
	},
	
	Render: function() {
		OP.Clear(0.2,0.2,0.2);
		OP.Depth(1);		

		this.world.Identity();
		
		OP.render.BindMesh(this.assets['model']);
		OP.render.BindEffect(this.assets['effect']);
		
		for(var i = 0; i < this.pieces.length; i++) {
			var piece = this.pieces[i];
			this.world.SetVec3(piece.x, piece.y, piece.z);
			this.world.Scale(3,1,3);
			OP.render.ParamMat4v('uWorld', this.world);
			
			OP.render.ClearActiveTextures();
			OP.render.ParamTexture('uColorTexture', this.assets['image']);
			OP.render.Mesh(this.camera);
		}
		
		OP.render.BindMesh(this.assets['grass']);
		OP.render.BindEffect(this.assets['effect']);
		
		for(var i = 0; i < this.pieces.length; i++) {
			var piece = this.pieces[i];
			if(!piece.grass) continue;
			
			this.world.SetRotY(piece.rotation);
			this.world.Vec3(piece.x, piece.y, piece.z);
			this.world.Scale(3,1,3);
			OP.render.ParamMat4v('uWorld', this.world);
			
			OP.render.ClearActiveTextures();
			OP.render.ParamTexture('uColorTexture', this.assets['image']);
			OP.render.Mesh(this.camera);
		}
		
		OP.render.BindMesh(this.assets['sheep']);
		OP.render.BindEffect(this.assets['colorEffect']);
		
		this.world.Identity();
		this.world.RotY(OP.math.PI + this.sheepRot);
		this.world.Vec3(0, 0.5, 0);
		OP.render.ParamMat4v('uWorld', this.world);		
		
		OP.render.ParamVec3('uLightDirection', this.lightDir.Id);
		
		
		OP.render.Mesh(this.camera);
			
		this.scratchMat4.SetScale(2, 2, 2);
		this.scratchMat4.Vec3(0, 0.5, 0);
		OP.font.RenderTextMat4('The Game', this.scratchMat4);
		
		OP.Present();
	},
	
	Destroy: function() {
		
	}
};

module.exports = GameBoard;