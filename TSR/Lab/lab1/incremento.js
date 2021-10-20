const ev = require('events');
const emitter = new ev.EventEmitter();

var e1 = 'e1'; var e1Value = 0;
var e2 = 'e2';
var e3 = 'e3';

function listener1(e1Value) {
    for (let inc = 0; inc < e1Value + 1; inc++) {
        e1Value++;
        e2 += e1Value.toString();
    }
    console.log('e1 -> ', e1Value);
}

function listener2(e2) {
    console.log('e2 -> ', e2);
    randomInterval = getRandomInt(2000, 5000);
}

function listener3() {
    console.log('e3 -> ', e3);
}

emitter.on(e1, listener1(e1Value));
emitter.on(e2, listener2(e2));
emitter.on(e3, listener3());

randomInterval = getRandomInt(2000, 5000);

setInterval(() => { emitter.emit(e1) }, randomInterval);
setInterval(() => { emitter.emit(e2) }, randomInterval);
setInterval(() => { emitter.emit(e3) }, randomInterval);


function getRandomInt(min, max) {
    return Math.floor(Math.random() * (max - min)) + min;
}
