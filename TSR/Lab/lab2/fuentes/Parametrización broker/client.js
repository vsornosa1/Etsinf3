const zmq = require('zeromq');
let req = zmq.socket('req');

const frontendURL = process.argv[2];
const idCliente = process.argv[3];
const txtPeticion = process.argv[4];

req.connect(frontendURL);
req.identity = idCliente;


req.on('message', (msg) => {
    console.log('resp: ' + msg);
});

req.send(txtPeticion);
console.log(idCliente + ' ' + txtPeticion);