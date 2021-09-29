// Imports
const events = require('events');
const emitter = new events.EventEmitter;

// Declaración de constantes y variables
const e1 = "Listener 1 activo";
const e2 = "Evento dos";
const e3 = "Evento tres";
const e4 = "+1s";
let n1 = 0, n2 = 0;
let interval2 = 2000;
let emitter2;

let s = 1;

// Funciones listener
function listener1() {
    console.log(s, "Evento 1");
    n1++;
}
function listener2() {
    n2++;
    n2 > n1 ? console.log(s, "E2: e1 < e2", n1, n2) : console.log(s, "E2: e1 > e2", n1, n2);
}
function listener3() {
    console.log(s, e3);
    if (interval2 < 18000) {
        console.log(interval2, "< 18s");
        interval2 *= 3;
    }
    else {
        interval2 = 18000;

        console.log(interval2, ">=18s !!")
        clearInterval(emitter2);
        emitter2 = setInterval(() => emitter.emit(e2), 18000);
    }
    // : clearInterval(emitter2); setInterval(() => emitter.emit(e2), 18000);

}
function addSecond() {
    s++;
}

// Eventos a la escucha
emitter.on(e1, listener1);
emitter.on(e2, listener2);
emitter.on(e3, listener3);
emitter.on(e4, addSecond);

// Emisión de eventos con intervalos
setInterval(() => emitter.emit(e1), 3000);
emitter2 = setInterval(() => emitter.emit(e2), interval2);
setInterval(() => emitter.emit(e3), 10000);

setInterval(() => emitter.emit(e4), 1000);