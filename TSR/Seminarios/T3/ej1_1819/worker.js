// worker.js
const zmq = require('zeromq');
const funciones = require('./myMath');

let producer = zmq.socket('pull'); // Se recibe del producer
let collector = zmq.socket('push');  // Se envía al collector

producer.connect('tcp://127.0.0.1:8001');
collector.connect('tcp://127.0.0.1:8002');

let w = process.argv[2];

producer.on('message',
    function (n) {
        collector.send([w, n, '' + funciones.fact(parseInt(n))]);
    }
);
