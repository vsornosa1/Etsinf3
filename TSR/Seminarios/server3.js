// net.Server -> Servidor TCP 3

const net = require('net');

let server = net.createServer(
    function (c) {
        c.on('end', () => console.log('Cliente desconectado'));
        c.on('error', () => console.log('Error de conexión'));
        c.on('data',
            function (data) {
                let parsedData = JSON.parse(data);
                let res;
                //if (typeof (parsedData.num) != 'number') res = NaN;
                //else {
                switch (parsedData.fun) {
                    case 'fibo':
                        res = fibo(parsedData.num);
                        break;
                    case 'fact':
                        res = fact(parsedData.num);
                        break;
                    default:
                        res = NaN;
                }
                //}
                c.write(parsedData.fun + '(' + parsedData.num + ') = ' + res);
            }
        );
    }
);

server.listen(9000, () => console.log('Servidor a la escucha en el puerto 9000'));

function fibo(n) {
    return (n < 2) ? 1 : fibo(n - 2) + fibo(n - 1)
}

function fact(n) {
    var rval = 1;
    for (var i = 2; i <= n; i++)
        rval = rval * i;
    return rval;
}
