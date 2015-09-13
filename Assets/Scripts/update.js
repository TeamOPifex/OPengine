var OP = require('OPengine');

console.log('Update');

function Update(r) {

    OP.keyboard.Update();

    if (OP.keyboard.WasReleased(OP.KEY.ESCAPE)) return 1;
    if (OP.keyboard.WasReleased(OP.KEY.BACKSPACE) && OP.gameState.Active != exampleSelectorState) {
        console.log('Changed');
        OP.gameState.Change(exampleSelectorState);
    }

    OP.render.Clear(r,0,0);
    OP.render.Present();

    return 0;
}