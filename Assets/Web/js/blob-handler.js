function BlobBuilder(key) {
	this.data = [];
	this.str(key);
}

BlobBuilder.prototype = {
	data: null,
	
	Blob: function() { return new Blob(this.data); },
	Send: function(w) { w.send(this.Blob()); },
	ui32: function(d) {
		var a = new Uint32Array(1);
		a[0] = d;
		this.data.push(a);
	},
	i32: function(d) {
		var a = new Int32Array(1);
		a[0] = d;
		this.data.push(a);
	},
	f32: function(d) {
		var a = new Float32Array(1);
		a[0] = d;
		this.data.push(a);
	},
	vec2: function(a, b) {
		this.f32(a);
		this.f32(b);
	},
	vec3: function(a, b, c) {
		this.f32(a);
		this.f32(b);
		this.f32(c);
	},
	str: function(d) {
		this.ui32(d.length);
		var str = this.utf8AbFromStr(d);	
		this.data.push(str);
	},
	
	utf8AbFromStr: function(str) {
		var strUtf8 = unescape(encodeURIComponent(str));
		var ab = new Uint8Array(strUtf8.length);
		for (var i = 0; i < strUtf8.length; i++) {
			ab[i] = strUtf8.charCodeAt(i);
		}
		return ab;
	}
};

function BlobReader(blob, ready) {
	this.blob = blob;
	this.position = 0;
	this.reader = new FileReader();
	var self = this;
	this.reader.onload = function(progress) {
		self.buffer = this.result;
		ready && ready();
	};
	this.reader.readAsArrayBuffer(this.blob);
}

BlobReader.prototype = {
	blob: null,
	position: null,
	buffer: null,
	
	ui8: function() {
		var arr = new Uint8Array(this.buffer.slice(this.position, this.position + 1));
		this.position += 1;
		return arr[0];
	},
	i8: function() {
		var arr = new Int8Array(this.buffer.slice(this.position, this.position + 1));
		this.position += 1;
		return arr[0];
	},
	ui16: function() {
		var arr = new Uint16Array(this.buffer.slice(this.position, this.position + 2));
		this.position += 2;
		return arr[0];
	},
	i16: function() {
		var arr = new Int16Array(this.buffer.slice(this.position, this.position + 2));
		this.position += 2;
		return arr[0];
	},
	ui32: function() {
		var arr = new Uint32Array(this.buffer.slice(this.position, this.position + 4));
		this.position += 4;
		return arr[0];
	},
	i32: function() {
		var arr = new Int32Array(this.buffer.slice(this.position, this.position + 4));
		this.position += 4;
		return arr[0];
	},
	f32: function() {
		var arr = new Float32Array(this.buffer.slice(this.position, this.position + 4));
		this.position += 4;
		return arr[0];
	},
	vec2: function() {
		return [this.f32(), this.f32()];
	},
	vec3: function() {
		return [this.f32(), this.f32(), this.f32()];
	},
	
	str: function() {
		var len = this.ui32();
		var res = this.uintToString(new Uint8Array(this.buffer.slice(this.position, this.position + len)));
		this.position += len;
		return res;
	},
	uintToString: function(uintArray) {
		var encodedString = String.fromCharCode.apply(null, uintArray),
			decodedString = decodeURIComponent(escape(encodedString));
		return decodedString;
	},
	
	ab2str: function(buf) {
		return String.fromCharCode.apply(null,buf);
	}
};