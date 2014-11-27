var util = require('util');
var Transform = require('stream').Transform;

util.inherits(SimpleProtocol, Transform);

function SimpleProtocol(options) {
  if (!(this instanceof SimpleProtocol))
    return new SimpleProtocol(options);

  Transform.call(this, options);
}


SimpleProtocol.prototype._transform = function(chunk, encoding, done) {
	this.emit('header', 'Test');
	done();
};

var parser = new SimpleProtocol();
