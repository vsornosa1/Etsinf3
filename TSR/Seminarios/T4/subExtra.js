const zmq = require('zeromq');
var sub = zmq.socket('sub');

var url = process.argv[2]
var topic = process.argv[3]

// Check how many arguments have been received.
if (process.argv.length != 4) {
  console.error("Format is 'node subextra URL topics+'");
  console.error("Example: 'node subextra localhost:9999 deporte'");
  process.exit(1);
}
sub.on('message', function(data) {
  console.log('Received ' + data);
});

sub.connect(url);
sub.subscribe(topic);
