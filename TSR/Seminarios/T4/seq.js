const zmq = require('zeromq');
var pull = zmq.socket ("pull");

var port = { x:9997, y:9998, z:9999 }
var s = {}

pull.bindSync('tcp://*:8888');

for (var i in port ) {
	s[i]=zmq.socket('pub')
	s[i].bindSync('tcp://*:'+ port[i])
}
pull.on('message',
	function(name, value, writer) {
	  s[name].send([name, value, writer]);
	}
);