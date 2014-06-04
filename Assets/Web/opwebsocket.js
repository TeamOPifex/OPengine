function OPWebSocket(url, reconnect) {
	this.init(url, reconnect);
}

OPWebSocket.prototype = {
	websocket: null,
	url: null,
	connected: false,
	reconnect: true,
	timer: null,
	
	OnMessage: null,
	OnOpen: null,
	OnClose: null,
	OnError: null,
	
	Send: function( blobBuilder ) {
		this.connected && blobBuilder.Send( this.websocket );
	},
	
	init: function( url, reconnect ) {
		this.url = url || location.host;
		this.reconnect = reconnect || true;
		
		this.websocket = new WebSocket('ws://' + this.url + '/ws');
		var self = this;
		this.websocket.onopen = function(e) { self.open(e); };
		this.websocket.onclose = function(e) { self.close(e); };
		this.websocket.onmessage = function(e) { self.message(e) };
		this.websocket.onerror = function(e) { self.error(e) };
	},
	
	open: function( e ) {
		console.log( 'OPWebSocket Opened' );
		this.connected = true;
		clearTimeout(this.timer);
		this.OnOpen && this.OnOpen( e );
	},
	
	close: function( e ) {
		if ( this.connected ) {
			console.log( 'OPWebSocket Closed' );
			this.connected = false;
			if ( this.reconnect ) {
				this.reconnectWebSocket( );
			}
			this.OnClose && this.OnClose( e );
		}
	},
	
	message: function( e ) {
		var self = this;
		var reader = new BlobReader( e.data , function() {
			self.OnMessage && self.OnMessage( reader );
		});
	},
	
	error: function( e ) {
		if ( !this.connected ) { 
			this.reconnectWebSocket( ); 
		} else {
			console.log( 'OPWebSocket Error :', e );
			this.OnError && this.OnError( e );
		}
	},
	
	reconnectWebSocket: function() {
		var self = this;
		setTimeout(function() {
			console.log('OPWebSocket Reconnecting...');
			self.init( self.url, self.reconnect );
		}, 500);
	}
};