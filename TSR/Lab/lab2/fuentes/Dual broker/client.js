const zmq = require('zeromq');
let req = zmq.socket('req');

req.identity = 'Client' + process.pid;
req.connect('tcp://localhost:8000');

req.on('message', (msg) => {
    console.log('\nRespuesta: ' + msg + '\n');
    process.exit(0);
})

req.send('Hola');
