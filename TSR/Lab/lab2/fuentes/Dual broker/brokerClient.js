const zmq = require('zeromq');
let sc = zmq.socket('router'); // Frontend
let sbc = zmq.socket('dealer'); // Broker client 

sc.bind('tcp://*:9998');
sbc.bind('tcp://*:9001');

let clients = [], msgQueue = [];

sc.on('message', (c, sep, m) => {
    if (workers.length == 0) {
        clients.push(c);
        msgQueue.push(m);
    } else {
        // 1a movida: Ya no hay workers en brokerClient
        sbc.send([workers.shift(), '', c, '', m]); // sbc -> sbw
    }
});

sbc.on('message', (w, sep, c, sep2, r, workerCount) => {
    if (c == '') {
        workers.push(w);
        return
    }
    clients.length > 0 ?
        sw.send([w, '', clients.shift(), '', msgQueue.shift()])
        : workers.push(w);

    sc.send([c, '', r]);
});
