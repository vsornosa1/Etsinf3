const zmq = require('zeromq')
let sbw = zmq.socket('dealer'); // Broker worker
let sw = zmq.socket('router'); // Backend

sbw.identity = 'Broker worker' + process.pid;
sbw.connect('tcp://localhost:8001');
sw.bind('tcp://*:8002');

let workers = [];

// FROM worker
sw.on('message', (w, sep, c, sep2, r) => {
    workers.push(w);
    sbw.send([c, '', r]);
    console.log('[*] Enviado mensaje a Broker cliente');
});

// FROM broker cliente
sbw.on('message', (c, sep, m) => {
    sw.send([workers.shift(), '', c, '', m]);
    console.log(' -> Nuevo mensaje del Broker cliente');
});
