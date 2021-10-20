// net.Socket -> Socket TCP 1

const net = require('net');

if (process.argv.length != 4) console.error('4 argumentos son necesarios!')
let factOFibo = process.argv[2];
let n = Math.abs(parseInt(process.argv[3]));

let client = net.connect({ port: 9000 },
    function () {
        console.log('Cliente conectado');
        let request = { "fun": factOFibo, "num": n };
        client.write(JSON.stringify(request));
    }
);

client.on('data',
    function (data) {
        console.log(data.toString());
        client.end();
    }
);

client.on('end', () => console.log('Servidor desconectado'));
client.on('error', () => console.log('Error de conexión'));