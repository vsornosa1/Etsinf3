const http = require("http");
const host = '127.0.0.1';
const port = 3000; // Oficialmente el puerto http es el 80 -> 3000 es el de la URL del servidor

const server = http.createServer((req, res) => {
    // res se utiliza como acciones de respuesta del servidor 
    res.statusCode = 200;
    res.setHeader('Content-Type', 'text/plain');
    res.end('Cualuier cosa aquí aparecerá escrita\n');
});

server.listen(port, host,
    () => console.log('Servidor activo en http://' + host + ':' + port + '/')
);
