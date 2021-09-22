const ev = require('events');
const emitter = new ev.EventEmitter;
const e1 = "print", e2 = "read";
const books = ["Walk Me Home", "When I Found You", "Jane's Melody", "Pulse"];

let n1 = 0, n2 = 0;

// Listener for event e1.
function listener1() {
    console.log("Event " + e1 + " has happened " + ++n1 + " times.");
}
// Listener for event e2.
function listener2(a) {
    console.log("Event " + e2 + " (with arg: " + a + ") has happened " + ++n2 + " times.");
}

// Listeners are registered in the event emitter.
emitter.on(e1, listener1);
emitter.on(e2, listener2);
// There might be more than one listener for the same event.
emitter.on(e1, function () { console.log("Something has been printed!!"); });

// Auxiliary function for generating e2.
let counter = 0;
function generateEvent2() {
    // This second argument provides the argument for the "e2" listener.
    emitter.emit(e2, books[counter++ % books.length]);
}

// Generate the events periodically...
// First event generated every 2 seconds.
setInterval(function () {
    emitter.emit(e1);
}, 2000);
// Second event generated every 3 seconds.
setInterval(generateEvent2, 3000);