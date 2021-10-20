// Imports
const events = require('events');
const emitter = new events.EventEmitter;

// Declaración de constantes y variables
const viajeMetro = "+1 viaje de metro";
const pantalonSara = "Pantalón de Saraaa!!";
const cadaSegundo = "+1s";

let s = 1;


// Funciones listener
function emitirEventoMetro() {
    console.log(s, 'lereles', viajeMetro);
}
function emitirEventoSara() {
    console.log('');
    console.log(s, pantalonSara);
    console.log('');
}
function addSecond() {
    s++;
}

// Eventos a la escucha
emitter.on(viajeMetro, emitirEventoMetro);
emitter.on(pantalonSara, emitirEventoSara);
emitter.on(cadaSegundo, addSecond);

// Emisión de eventos con intervalos
setInterval(() => emitter.emit(viajeMetro), 1000);
setInterval(() => emitter.emit(pantalonSara), 41000);

setInterval(() => emitter.emit(cadaSegundo), 1000);