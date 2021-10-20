const net = require('net');

const server = net.createServer(
    function (c) { // Connection listener
        console.log('Servidor: Cliente conectado');
        c.on('end',
            function () {
                console.log('Servidor: Cliente desconectado');
            });
        c.on('data',
            function (data) {
                c.write(parseInt(data + '') * 3 + '');
            });
    }
);

// Listening listener
server.listen(parseInt(process.argv[2]) || 21001, () => console.log('Server bound'));
