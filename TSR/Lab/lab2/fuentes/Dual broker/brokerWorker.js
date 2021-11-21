const zmq = require('zeromq')
let sbw = zmq.socket('dealer'); // Broker worker
let sw = zmq.socket('router'); // Backend

sbw.connect('tcp://*:9001');
sw.bind('tcp://*:9999');

let workers = [];
let workerCount = 0;

sw.on('message', (w, sep, c, sep2, r) => {
    if (c == '') {
        workers.push(w);
        // 2a movida: Tiene que enviar la notificacion de que existe un nuevo cliente desde sbw al sbc
        workerCount++;
        sbw.send([w, sep, c, sep2, r, workerCount]);
        return
    }
    clients.length > 0 ?
        sw.send([w, '', clients.shift(), '', msgQueue.shift()])
        : workers.push(w);

    sc.send([c, '', r]);
});

sbw.on('message', (c, sep, m) => {

    // 3a movida: Ya no hay clients ni msgQueue en brokerWorker
    if (workers.length == 0) {
        clients.push(c);
        msgQueue.push(m);
    } else {
        sw.send([workers.shift(), '', c, '', m]);
    }
});