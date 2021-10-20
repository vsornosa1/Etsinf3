const net = require('net');

const client = net.connect(parseInt(process.argv[2]) || 21000,
    function () { // Connection listener
        console.log('Cliente conectado');
        client.write(process.pid + '');
    }
);

client.on('data', function () {
    console.log('aaaaaa');
    //console.log(data.toString()); 
    client.end();
    process.exit(0);
});
client.on('end', () => console.log('Cliente desconectado'));
