//Emisor de eventos (emisor3.js) -------------------------------------------
const ev = require('events');
const emitter = new ev.EventEmitter();

const e1 = 'e1 ->', e2 = 'e2 ->';
let inc = 0, t = 1000;

function rand(max, min) { // Devuelve valores en rango [2000,5000) (ms)
    // Math.floor(x) devuelve la parte entera del valor x
    // Math.random() devuelve un valor en el rango [0,1)
    let t = Math.floor(Math.random() * (max - min)) + min;
    return t;
}

function handler(e, valor) { // e es el evento, n el valor asociado
    return (inc) => {
        if (typeof valor == 'number') {
            console.log(e, inc++); // Caso e1
        }
        else {
            //valor += inc.toString();
            console.log(e, inc + 2); // Caso e2
        }
    }

} // El oyente recibe un valor (inc)

emitter.on(e1, handler(e1, 0));
emitter.on(e2, handler(e2, ''));

function etapa(t) {
    emitter.emit(e1);
    emitter.emit(e2);
    console.log('Duración: ', t);
}

setInterval(() => etapa(t), rand(5000, 2000));