const zmq = require('zeromq');
let sc = zmq.socket('router'); // Frontend
let sw = zmq.socket('router'); // Backend

const frontendURL = process.argv[2];
const backendURL = process.argv[3];

sc.bind(frontendURL);
sw.bind(backendURL);

let clients = [], msgQueue = [], workers = [];

sc.on('message', (c, sep, m) => {
    if (workers.length == 0) {
        clients.push(c);
        msgQueue.push(m);
    } else {
        sw.send([workers.shift(), '', c, '', m]);
    }
});

sw.on('message', (w, sep, c, sep2, r) => {
    if (c == '') {
        workers.push(w);
        return
    }
    clients.length > 0 ?
        sw.send([w, '', clients.shift(), '', msgQueue.shift()])
        : workers.push(w);

    sc.send([c, '', r]);
});
