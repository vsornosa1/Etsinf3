const zmq = require('zeromq');
let sc = zmq.socket('router'); // Frontend
let sbc = zmq.socket('dealer'); // Broker client 

sc.bind('tcp://*:8000');

sbc.identity = 'Broker client' + process.pid;
sbc.bind('tcp://*:8001');

let clients = [], msgQueue = [];
let nWorkers = 0;

// FROM cliente
sc.on('message', (c, sep, m) => {
    if (nWorkers == 0) {
        clients.push(c);
        msgQueue.push(m);
    } else {
        sbc.send([c, '', m]); // sbc -> sbw
        nWorkers--;
        console.log('[*] Enviado mensaje a Broker worker');
    }
});

// FROM broker worker
sbc.on('message', (c, sep2, r) => {
    console.log(' -> Nuevo mensaje del broker worker');
    if (c == '') {
        nWorkers++;
        console.log('[*] Nuevo worker notificado');
        return
    }
    clients.length > 0 ?
        sbc.send([clients.shift(), '', msgQueue.shift()])
        : nWorkers++;

    sc.send([c, '', r]);
});
