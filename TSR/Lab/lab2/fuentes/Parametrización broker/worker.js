const zmq = require('zeromq');
let req = zmq.socket('req');

req.identity = 'Worker1' + process.pid;

const backendURL = process.argv[2];
const idWorker = process.argv[3];
const txtRespuesta = process.argv[4];

req.connect(backendURL);

req.on('message', (c, sep, msg) => {
    setTimeout(() => {
        req.send([c, '', txtRespuesta]);
    }, 1000);
});

req.send(['', '', '']);
