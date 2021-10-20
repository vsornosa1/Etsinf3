// net.Server -> Servidor TCP 2

const net = require('net');

let server = net.createServer(
    // c = Socket que representa la conexión con el cliente
    function (c) { // Connection listener 
        console.log('Server connected');
        c.on('end', () => console.log('Cliente desconectado'));
        c.on('error', () => console.log('Error de conexión'));
        c.on('data',
            function (data) {
                console.log('Datos del cliente: ' + data.toString());
                c.write(data);
            }
        );
    }
);

// Establece el puerto de escucha
server.listen(9000, () => console.log('Servidor a la escucha en el puerto 9000'));

// Lo más importante de las funciones como 2º argumento es 
// poder establecer en el mismo momento de ejecución una serie de
// funcionalidades que confirman que el hecho ha sucedido. (aka MANEJADOR)