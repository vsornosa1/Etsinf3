const zmq = require('zeromq');
let pub = zmq.socket('pub');

let port = process.argv[2] | 8005;
let nMensajes = process.argv[3];
let temas = process.argv.slice(4);

let cont = 0;
pub.bind('tcp://*:' + port);

function emite() {
    if (cont >= nMensajes) process.exit(0);
    let tema = temas[0];
    let vecesTema = Math.floor((cont/temas.length) + 1);
    let aEnviar = (cont + 1) + ': ' + tema + ' ' + vecesTema;
    pub.send(aEnviar);
    cont++;
    temas.shift(); temas.push(tema); //Rotatorio
}

setInterval(emite,1000);
