const zmq = require('zeromq');
let sc = zmq.socket('router'); // Frontend
let sw = zmq.socket('router'); // Backend

const frontendPort = process.argv[2];
const backendPort = process.argv[3];

sc.bind('tcp://*:' + frontendPort);
sw.bind('tcp://*:' + backendPort);

let clients = [], msgQueue = [], workers = [];
let nRequests = 0;
let workersCount = {};

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

        if (workersCount[w]) {
            workersCount[w]++
        }
        else {
            workersCount[w] = 1
        }

    ++nRequests;
    let finalMsg = ' ' + nRequests;
    sc.send([c, '', r + finalMsg]);
});

setInterval(() => { 
    console.log('Numero de peticiones totales: ' + nRequests);
    for(const w in workersCount) {
        console.log('  El ' + w + ' ha completado ' + workersCount[w] + ' peticiones.\n');
    }
}, 5000);
