var OP = require('OP').engine;

console.log('Update');

function Update(r) {

    OP.render.Clear(r,0,0);
    OP.render.Present();

    return 0;
}
