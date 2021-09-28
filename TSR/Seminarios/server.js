// net.Server -> Servidor TCP

const net = require('net');

let server = net.createServer(
    // c = Socket que representa la conexión con el cliente
    function (c) { // Connection listener 
        console.log('Server connected');
        c.on('end', () => console.log('Cliente desconectado'));
        c.write('Hola\r\n'); // Envía un Hola al cliente
        c.pipe(c); // Echo -> Lo que lee del socket lo vuelve a colocar
    }
);

// Establece el puerto de escucha
server.listen(9000, () => console.log('Server listening on 9000'));

// Lo más importante de las funciones como 2º argumento es 
// poder establecer en el mismo momento de ejecución una serie de
// funcionalidades que confirman que el hecho ha sucedido. (aka MANEJADOR)