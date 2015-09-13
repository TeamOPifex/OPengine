var OP = require('OPengine');

function Initialize() {
	this.Data.Mesh = OP.cman.LoadGet('PuzzleBlock.opm');
	this.Data.Texture = OP.cman.LoadGet('TetrisBroken.png');

	this.Data.Rotation = 0;
	this.Data.LightDirection = new OP.vec3(0, 1, 0);
	this.Data.Effect = OP.effect.Gen(
		'Common/Texture3D.vert',
		'Common/Texture.frag',
		OP.ATTR.POSITION | OP.ATTR.NORMAL | OP.ATTR.UV,
		'Textured Effect',
		0);

	this.Data.Camera = new OP.cam.Persp(2,2,2,0,1,0,0,1,0,0.1,100.0,45.0, 1280.0 / 720.0);

	OP.render.Depth(1);

	return 1;
}

function Update() {
	if(OP.keyboard.IsDown(OP.KEY.SPACE)) this.Data.Rotation++;

	var rot = this.Data.Rotation / 10.0;
	var world = new OP.mat4();
	world.RotY(rot);
	world.Translate(0, 1, 0);
	//world.Scl(0.25, 0.25, 0.25);

	OP.render.Clear(0.0, 0.0, 0.0);
	
	OP.mesh.Bind(this.Data.Mesh);
	OP.effect.Bind(this.Data.Effect);

	OP.texture.ClearActive();
	OP.effect.ParamTex('uColorTexture', this.Data.Texture);

	OP.cam.UpdateView(this.Data.Camera);
	OP.cam.UpdateProj(this.Data.Camera);

	OP.effect.ParamMat4('uWorld', world);
	OP.effect.ParamCam(this.Data.Camera);

	OP.effect.ParamVec3('vLightDirection', this.Data.LightDirection);

	OP.mesh.Render();

	OP.render.Present();

	return 0;
}

function Exit() {
	return 1;
}

module.exports = {
	Initialize: Initialize,
	Update: Update,
	Exit: Exit,
	Create: function() {
		return OP.gameState.Create(Initialize, Update, Exit);
	}
};