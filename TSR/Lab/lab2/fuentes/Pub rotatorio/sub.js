const zmq = require('zeromq');
let sub = zmq.socket('sub');

sub.connect('tcp://127.0.0.1:8005');
sub.subscribe('');

sub.on('message', (msg) => {
    console.log('', msg + '');
})
