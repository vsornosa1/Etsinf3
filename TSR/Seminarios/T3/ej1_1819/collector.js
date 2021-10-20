// collector.js
const zmq = require('zeromq');
let puller = zmq.socket('pull');
puller.bind('tcp://*:8002');
puller.on('message',
    function (w, n, m) {
        console.log('[w' + w + '] fact(' + n + ') = ' + m);
    }
);
