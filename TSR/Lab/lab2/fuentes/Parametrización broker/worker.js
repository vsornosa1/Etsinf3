const zmq = require('zeromq');
let req = zmq.socket('req');

const backendURL = process.argv[2];
const idWorker = process.argv[3];
const txtRespuesta = process.argv[4];

req.identity = idWorker;
req.connect(backendURL);

req.on('message', (c, sep, msg) => {
    setTimeout(() => {
        req.send([c, '', txtRespuesta]);
    }, 1000);
});

req.send(['', '', '']);
