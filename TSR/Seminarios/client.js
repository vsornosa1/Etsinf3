// net.Socket -> Socket TCP

const net = require('net');

// Client es un socket
let client = net.connect({ port: 9000 },
    function () {
        console.log('Cliente conectado');
        client.write('mundo!\r\n');
    }
);

client.on('data',
    function (data) {
        console.log(data.toString());
        client.end();
    }
);

client.on('end', () => console.log('Servidor desconectado'));