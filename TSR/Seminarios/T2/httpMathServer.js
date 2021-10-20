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
var data_listener = function (data, res) {
    return function () {
        var formRespuesta = form + "<br/>The response of your last request is:<br/>"
            + data
            + "<br/>To make another request, use the previous form again.<hr/>";
        res.writeHead(200, { 'Content-Type': 'text/html' });
        res.end(formRespuesta);
    }
};


var server = http.createServer(function (request, response) {
    if (request.method === "POST") {
        var dataPost = '';
        request.on('data', function (data) {
            // Construir res http
            dataPost += data;
        });
        request.on('end', function () {
            var parsedData = querystring.parse(dataPost);
            var num = Math.abs(parseInt(parsedData.num));
            var fun = parsedData.fun;

            let client = net.createConnection({ port: netPort }, function () {
                console.log('Conexión creada');
                client.write(json.stringify(
                    {
                        "num": num,
                        "fun": fun
                    }
                ));
            });
            client.on('end', end_listener);
            client.on('error', error_listener);
            client.on('data', data_listener(data, response)); // Clausura
        });

    }

    if (request.method === "GET") {
        response.writeHead(200, { 'Content-Type': 'text/html' });
        response.end(form);
    };
});


server.listen(httpPort, host, function () {
    console.log('HTTP server running at http://' + host + ':' + httpPort + '/');
});
