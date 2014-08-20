var OP = require('./node_modules/OPengine/OPengine');

var state = null;

function Attribute(name, size) { return { Id: name, Size: size }; }

////////
// GAME SETUP
///////////////////////////

var scene;
var boxes = [];
var scratchMat4;
var scratch2Mat4;
var scratch3Mat4;
var assets = [];
var camera;
var lightDir;
var go = true;

function Attribute(name, size) { return { Id: name, Size: size }; }

function Initialize() {
	console.log('TEST: ' + Test);
	
	OP.cman.Init('assets/');
	OP.render.Init(function(result) {
	
		
		OP.cman.Load("impact.wav");
		OP.cman.Load("background.ogg");
		
		OP.audio.Init();
		
		OP.physics.Init();
		scene = OP.physics.CreateScene();
		scene.CreatePlane();
		
		for(var i = 0; i < 4; i++) {
			var r = OP.math.Rand();
			var r2 = 0.5;
			var r3 = OP.math.Rand();
			var size = 0.05 + (OP.math.Rand() * 2.0);
			var box = scene.CreateBoxDynamic({
				x: -20 + (40 * r),
				y: 5 + 200 * r3,
				z: -20 + (40 * r3),
				sx: size,
				sy: size,
				sz: size
			});
			box.size = size;
			boxes.push(box);
		}
		
		scratchMat4 = OP.mat4.Create();
		scratch2Mat4 = OP.mat4.Create();
		scratch3Mat4 = OP.mat4.Create();
		
		OP.cman.Load("PuzzleBlock.opm");
		OP.cman.Load("TetrisBroken.png");
		OP.cman.Load("TexturedModel.vert");
		OP.cman.Load("TexturedModel.frag");
		
		assets['sound'] = OP.audio.Create(OP.cman.Get("impact.wav"));
		assets['bgSound'] = OP.audio.Create(OP.cman.Get("background.ogg"));
		
		assets["PuzzleBlock"] = OP.cman.Get("PuzzleBlock.opm");
		assets["TetrisBroken"] = OP.cman.Get("TetrisBroken.png");
		
        assets['vert'] = OP.cman.Get("TexturedModel.vert");
        assets['frag'] = OP.cman.Get("TexturedModel.frag");
		assets['effect'] = OP.effect.Create({
			Name: 'Model Effect', 
			Vertex: assets['vert'],
			Fragment: assets['frag'],
			Stride: 32,
			Attributes: [ Attribute('aPosition', 3), Attribute('aNormal', 3), Attribute('aUV', 2) ] 
		});
		
        
        camera = OP.camera.Create(0, 30, 30, 0, 0, 0);
		lightDir = OP.vec3.Create(1, 1, 1);
	});
}

function Update(elapsed) {
	OP.input.Update();
	
	if(OP.input.KeyWasPressed(OP.KEYS.ENTER)) {	
		assets['bgSound'].Bind();
		assets['bgSound'].Volume(0.05);
		assets['bgSound'].Play();
	}
	
	scene.Update(elapsed);
	
	if(OP.input.KeyIsDown(OP.KEYS.ESCAPE)) return 1;
	
	OP.Clear(0,0,0);
	
	OP.Depth(1);		

	scratchMat4.Identity();
	
	OP.render.BindMesh(assets['PuzzleBlock']);
	OP.render.BindEffect(assets['effect']);
	OP.render.ClearActiveTextures();
	OP.render.ParamTexture('uColorTexture', assets['TetrisBroken']);
	
	for(var i = 0; i < boxes.length; i++) {
		boxes[i].GetTransform(scratch3Mat4);		
		scratch2Mat4.SetScale(boxes[i].size, boxes[i].size, boxes[i].size);
		OP.mat4.Mul(scratchMat4, scratch2Mat4, scratch3Mat4);
		
		OP.render.ParamMat4v('uWorld', scratchMat4);
		OP.render.Mesh(camera);
	}
		
	OP.Present();
		
	return 0;
}

function Destroy() {

}

OP.Start(Initialize, Update, Destroy);