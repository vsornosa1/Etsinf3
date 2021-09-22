// Imports
const ev = require('events');
const emitter = new ev.EventEmitter;

// Declaración de constantes y variables
const e1 = "Listener 1 activo";
const e2 = "Evento dos";
const e3 = "Evento tres";
let n1 = 0, n2 = 0;
let interval2 = 2000;
let emitter2;


// Funciones listener
function listener1() {
    console.log(e1);
    n1++;
}
function listener2() {
    n2++;
    n2 > n1 ? console.log(e2) : console.log("Hay más eventos de tipo 1");
}
function listener3() {
    console.log(e3);
    clearInterval(emitter2);
    interval2 < 18000 ?
        setInterval(function () { emitter.emit(e2); }, 3 * interval2)
        : setInterval(function () { emitter.emit(e2); }, 18000);
}


// Eventos a la escucha
emitter.on(e1, listener1);
emitter.on(e2, listener2);
emitter.on(e3, listener3);


// Emisión de eventos con intervalos
setInterval(function () { emitter.emit(e1); }, 3000);
emitter2 = setInterval(function () { emitter.emit(e2); }, interval2);
setInterval(function () { emitter.emit(e3) }, 10000);

