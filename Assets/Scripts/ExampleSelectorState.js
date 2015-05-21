// Modeled after the ExampleSelectorState.cpp in the OPengine Application

var OP = require('OPengine');
var live = require('require-live');

var data = {
	Selected: 0,
    Initialized: false,
	FontManager: null,
	Examples: [],
    CurrentHierarchy: -1,
    HierarchyDepth: [],
    CurrentDepth: 0,
	Background: null,
    Controller: null,
    Size: {}
};

function Initialize() {

	data.Background = OP.texture2D.Create(OP.cman.LoadGet("subtle-irongrip.png"), 0, 0, 0, 3, 3);
	OP.vec2.Set(data.Background.Scale, 2, 2);


	if(!data.Initialized) {
		data.Initialized = true;

		console.log('cman', OP.cman.Load("test.png"));
		console.log('cman', OP.cman.Get("test.png"));

		data.Size = OP.render.Size();


		data.Examples.push({ name: "Basics", state: null, available: 1, parent: -1 });
		data.Examples.push({ name: "Intermediate", state: null, available: 1, parent: -1 });
		data.Examples.push({ name: "Advanced", state: null, available: 1, parent: -1 });
		data.Examples.push({ name: "Exit", state: null, available: 1, parent: -1 });

		data.Examples.push({ name: "Audio", state: require('./Examples/Audio.js'), available: OP.defined.OPIFEX_OPTION_AUDIO != undefined, parent: 0 });
		data.Examples.push({ name: "FMOD", state: require('./Examples/FMOD.js'), available: OP.defined.OPIFEX_OPTION_FMOD != undefined, parent: 0 });
		data.Examples.push({ name: "Free Flight Camera", state: require('./Examples/FreeFlightCamera.js'), available: 1, parent: 0 });
		
		var modelState = live('./Examples/Model.js', function(result) { modelState.Update = result.Update; }).Create();
		data.Examples.push({ name: "Model", state: modelState, available: 1, parent: 0 });
		var texturedState = live('./Examples/Textured.js', function(result) { texturedState.Update = result.Update; }).Create();
		data.Examples.push({ name: "Textured", state: texturedState, available: 1, parent: 0 });
		data.Examples.push({ name: "Sprite", state: require('./Examples/FreeFlightCamera.js'), available: 1, parent: 0 });
		data.Examples.push({ name: "Sprite System", state: require('./Examples/FreeFlightCamera.js'), available: 1, parent: 0 });

		data.Examples.push({ name: "Mesh Builder", state: require('./Examples/FreeFlightCamera.js'), available: 1, parent: 1 });
		data.Examples.push({ name: "Particle System", state: require('./Examples/FreeFlightCamera.js'), available: 1, parent: 1 });
		data.Examples.push({ name: "Spine", state: require('./Examples/FreeFlightCamera.js'), available: 1, parent: 1 });
		data.Examples.push({ name: "IMGUI", state: require('./Examples/FreeFlightCamera.js'), available: 1, parent: 1 });
		data.Examples.push({ name: "Spherical Cube", state: require('./Examples/FreeFlightCamera.js'), available: 1, parent: 1 });

		data.Examples.push({ name: "Physics", state: require('./Examples/FreeFlightCamera.js'), available: OP.defined.OPIFEX_OPTION_PHYSICS != undefined, parent: 2 });
		data.Examples.push({ name: "Skinning", state: require('./Examples/FreeFlightCamera.js'), available: 1, parent: 2 });
		data.Examples.push({ name: "Shadows", state: require('./Examples/FreeFlightCamera.js'), available: 1, parent: 2 });
		data.Examples.push({ name: "Deferred", state: require('./Examples/FreeFlightCamera.js'), available: 0, parent: 2 });
		data.Examples.push({ name: "Scripting", state: require('./Examples/FreeFlightCamera.js'), available: OP.defined.OPIFEX_OPTION_V8 != undefined, parent: 2 });
		data.Examples.push({ name: "Oculus", state: require('./Examples/FreeFlightCamera.js'), available: OP.defined.OPIFEX_OPTION_OCULUS != undefined, parent: 2 });

		for(var i = 0; i < data.Examples.length; i++) data.HierarchyDepth.push(0);
	}

	data.FontManager = OP.font.Manager.Setup();

	return 1;
}

function Update(elapsed) {
	if(OP.keyboard.WasPressed(OP.KEY.ESCAPE)) {
		return 1;
	}

    var currentCategoryCount = 0;
    for (var i = 0; i < data.Examples.length; i++) {
        if (data.Examples[i].parent != data.CurrentHierarchy) continue;
        currentCategoryCount++;
    }

	if (OP.keyboard.WasPressed(OP.KEY.W) || OP.keyboard.WasPressed(OP.KEY.UP)) {
		data.Selected--;
	}
	if (OP.keyboard.WasPressed(OP.KEY.S) || OP.keyboard.WasPressed(OP.KEY.DOWN)) {
		data.Selected++;
	}
    if (data.Selected < 0) data.Selected = currentCategoryCount - 1;
    if (data.Selected >= currentCategoryCount) data.Selected = 0;





	// Determine the actual selection within our entire array
	// Skips passed the category entries
	var actualSelected = 0;
	for (var i = 0, j = 0; i < data.Examples.length && j <= data.Selected; i++) {
	    if (data.Examples[i].parent != data.CurrentHierarchy) continue;
	    actualSelected = i;
	    j++;
	}

	// When an example is selected:
	if (data.Examples[actualSelected].available && (OP.keyboard.WasPressed(OP.KEY.SPACE) || OP.keyboard.WasPressed(OP.KEY.E)|| OP.keyboard.WasPressed(OP.KEY.D) || OP.keyboard.WasPressed(OP.KEY.ENTER))) {

	    // Hard coded to category [3] which is Exit
	    if(actualSelected == 3) {
	        return 1;
	    }

	    // If the state is NULL then it's a category so we need to
	    // delve further into the hierarchy
	    if(data.Examples[actualSelected].state == null) {
	        data.HierarchyDepth[data.CurrentHierarchy + 1] = actualSelected;
	        data.CurrentHierarchy = actualSelected;
	        data.CurrentDepth++;
	        data.Selected = data.HierarchyDepth[data.CurrentHierarchy + 1];
	    } else {
	        // Otherwise it's an example, load it up.
	        OP.gameState.Change(data.Examples[actualSelected].state);
	        return 0;
	    }
	}

	// Jump backwards in the hierarchy
	if ((OP.keyboard.WasPressed(OP.KEY.BACKSPACE) || OP.keyboard.WasPressed(OP.KEY.A))) {
	    data.HierarchyDepth[data.CurrentHierarchy + 1] = data.Selected;
	    data.CurrentHierarchy = -1;
	    data.CurrentDepth--;
	    data.Selected = data.HierarchyDepth[data.CurrentHierarchy + 1];
	}





	OP.render.Clear(0.0, 0.0, 0.0);

	//OP.texture2D.Render(data.Background);

	OP.font.Render.Begin(data.FontManager);
	OP.font.Render.Color(1,1,1);

	var start = -(data.Selected) * 40 + data.Size.ScaledHeight / 2;

	data.FontManager.scale(0.75);
	OP.font.Render.Text('OPengine v0.4.6', 50, start - 60);
	data.FontManager.scale(0.5);

	var r, g, b;
	r = g = b = 1;


    var notTheCurrentlySelectedMenuItem = 0, isActiveCategory = 0;
	var r, g, b;
    var pos = 0;
	for (var i = 0; i < data.Examples.length; i++) {

        isActiveCategory = i == data.CurrentHierarchy;
        if(data.Examples[i].parent != data.CurrentHierarchy && !isActiveCategory) continue;

		notTheCurrentlySelectedMenuItem = data.Selected != pos;
        // Set Selected Color (bright yellow-ish gold)
		r = 0.95, g = 0.84; b = 0;

		if(notTheCurrentlySelectedMenuItem) {
			r = g = b = 1.0;
		}

		if (!data.Examples[i].available) {
            // Menu item is not available so make it really dark
			r = g = b = 0.3;
            // Menu item is not available but it's the currently selected
            // item, so we'll brighten it just a bit so that we know what
            // is selected.
            if(!notTheCurrentlySelectedMenuItem) {
                r  = g = b = 0.5;
            }
		}

        // If this is a category, then it becomes light blue
        if(isActiveCategory || data.Examples[i].state == null) {
            r = g = 0.7; b = 1.0;
            if(notTheCurrentlySelectedMenuItem && data.CurrentDepth == 0) {
                r = g = 0.4; b = 0.7;
            }
        }


		OP.font.Render.Color(r,g,b,1);

        // If it's a category it doesn't get pushed to the right
        if(isActiveCategory) {
            OP.font.Render.Text(data.Examples[i].name,
                         75, start + 40 * pos);
        } else {
            // If it's the root menu we don't offset to the right
            // If it isn't the root menu, then we push it to right
            // to help indicate that it's a sub-menu
            var isNotRootMenu = (data.CurrentHierarchy != -1);
            OP.font.Render.Text(data.Examples[i].name,
                         75 + 40 * isNotRootMenu,
                                      start + 40 * (pos + isNotRootMenu));
            pos++;
        }
	}

	OP.font.Render.End();

	OP.render.Present();

	return 0;
}

function Exit() {
	return 1;
}

module.exports = OP.gameState.Create(Initialize, Update, Exit);