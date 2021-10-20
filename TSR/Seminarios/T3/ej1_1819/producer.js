// producer.js
const zmq = require('zeromq');
let pusher = zmq.socket('push');
pusher.bind('tcp://*:8001');
let i = 0;
let handler =
    setInterval(function () {
        pusher.send('' + ++i);
        if (i == 150) {
            clearInterval(handler);
            pusher.close();
            process.exit();
        }
    }, 100);
