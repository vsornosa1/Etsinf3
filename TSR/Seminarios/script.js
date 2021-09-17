console.log('Hola mundo');

var contadorTSR = 0;
function sumar() {
    contadorTSR++;
    document.getElementById('output').innerHTML = contadorTSR;
}

function restar() {
    contadorTSR--;
    document.getElementById('output').innerHTML = contadorTSR;
}

function sumar5() {
    contadorTSR += 5;
    document.getElementById('output').innerHTML = contadorTSR;
}

function restar5() {
    contadorTSR -= 5;
    document.getElementById('output').innerHTML = contadorTSR;
}

function sumar10() {
    contadorTSR += 10;
    document.getElementById('output').innerHTML = contadorTSR;
}

function restar10() {
    contadorTSR -= 10;
    document.getElementById('output').innerHTML = contadorTSR;
}

function doblar() {
    contadorTSR *= 2;
    document.getElementById('output').innerHTML = contadorTSR;
}

function partir() {
    contadorTSR /= 2;
    document.getElementById('output').innerHTML = contadorTSR;
}

function dividirPI() {
    contadorTSR /= Math.PI;
    document.getElementById('output').innerHTML = contadorTSR;
}

function reset() {
    contadorTSR = 0;
    document.getElementById('output').innerHTML = 0;
}

