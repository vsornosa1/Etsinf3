// net.Socket -> Socket TCP 2

const net = require('net');
let cont = 0;

// Client es un socket
let client = net.connect({ port: 9000 },
    function () {
        console.log('Cliente conectado');
        client.write(cont + ' mundo!');
    }
);

client.on('data',
    function (data) {
        console.log(data.toString());
        cont > 1000 ?
            client.end()
            : client.write((++cont) + ' mundo!');
    }
);

client.on('end', () => console.log('Servidor desconectado'));
client.on('error', () => console.log('Error de conexión'));
