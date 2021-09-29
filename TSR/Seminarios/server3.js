// net.Server -> Servidor TCP 3

const net = require('net');
let myF = require('./myFunctions');

let server = net.createServer(
    function (c) {
        c.on('end', () => console.log('Cliente desconectado'));
        c.on('error', () => console.log('Error de conexión'));
        c.on('data',
            function (data) {
                let parsedData = JSON.parse(data);
                let res;
                if (typeof (parsedData.fun) != 'number') res = NaN;
                else {
                    switch (parsedData.fun) {
                        case 'fibo':
                            res = myF.fibo(parsedData.num);
                            break;
                        case 'fact':
                            res = myF.fact(parsedData.num);
                            break;
                        default:
                            res = NaN;
                    }
                }
                c.write(parsedData.fun + '(' + parsedData.num + ') = ' + res);
            }
        );
    }
);

server.listen(9000, () => console.log('Servidor a la escucha en el puerto 9000'));