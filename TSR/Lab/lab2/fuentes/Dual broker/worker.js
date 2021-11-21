const zmq = require('zeromq');
let req = zmq.socket('req');

req.identity = 'Worker' + process.pid;
req.connect('tcp://localhost:8002');

req.on('message', (c, sep, msg) => {
    setTimeout(() => {
        req.send([c, '', 'resp']);
    }, 1000);
});

req.send(['', '', '']);
