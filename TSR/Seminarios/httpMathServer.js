// httpMathServer.js -> Modificación de netClient3.js

const http = require('http');
const net = require('net');
const querystring = require('querystring');
const form = require('fs').readFileSync('formulario.html');

if (process.argv.length != 4) {
    console.log('Two arguments, httpPort and netPort, son necesarios');
    process.exit();
}
var host = '127.0.0.1';
var httpPort = Math.abs(parseInt(process.argv[2]));
var netPort = Math.abs(parseInt(process.argv[3]));

var end_listener = 'Servidor desconectado';
var error_listener = 'Error de conexión';
var data_listener = 'Data';


var server = http.createServer(function (request, response) {
    if (request.method === "POST") {
        var dataPost = '';
        request.on('data', function (data) { console.log(data.toString()); });
        request.on('end', function () { console.log(error_listener); });
        let client = net.createConnection({ port: netPort }, () => console.log('Conexión creada'));
        client.on('end', end_listener)
        client.on('error', error_listener)
        client.on('data', data_listener); // Clausura
    }

    if (request.method === "GET") {
        response.writeHead(200, { 'Content-Type': 'text/html' })
        response.end(form)
    };
});

server.listen(httpPort, hostname, function () {
    console.log('http server running at http://' + host + ':' + httpPort + '/')
});