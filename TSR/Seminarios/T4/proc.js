const zmq = require('zeromq');

var push = zmq.socket('push');
var sub = zmq.socket('sub');

if (process.argv.length!=4) {
	console.error('Usage: node proc seqIP procID');
	process.exit(1);
} 
var local = { x:0, y:0, z:0 }
var port = {x:9997, y:9998, z:9999}

push.connect('tcp://'+process.argv[2]+':8888')
sub.subscribe("");

for (var i in port) 
	sub.connect('tcp://'+process.argv[2]+':'+ port[i])
var id = process.argv[3];

function W(name, value) {
	console.log("W" + id + "(" + name + ")" + value)
}
function R(name) {
	console.log("R" + id + "(" + name + ")" + local[name])
}

var n = 0, names = ["x","y","z"];

function writeValue() {
	n ++; push.send ([names[n%names.length], (10*id)+n, id])
}
sub.on('message', function(name, value, writer) {
	local[name] = value;
	if (writer == id) W(name, value); 
	else R(name);
})

function work() { setInterval(writeValue, 10) }

setTimeout(work, 2000); 
setTimeout(process.exit, 2500);
