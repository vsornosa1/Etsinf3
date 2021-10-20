//Lecturas de ficheros (read2.js) ---------------------------------------
const fiSys = require("./fiSys");

//Para la lectura asíncrona:
console.log("Invocación lectura asíncrona");
fiSys.readFile("/proc/loadavg", cbError, formato);
console.log("Lectura asíncrona invocada\n\n");

//Lectura síncrona
console.log("Invocación lectura síncrona");
const datos = fiSys.readFileSync("/proc/loadavg");
if (datos != undefined) formato(datos);
else console.log(datos);
console.log("Lectura síncrona finalizada\n\n");

//- - - - - - - - - - -
function formato(datos) {
    const separador = " "; //espacio
    const tokens = datos.toString().split(separador);
    const min1 = parseFloat(tokens[0]) + 0.01;
    const min5 = parseFloat(tokens[1]) + 0.01;
    const min15 = parseFloat(tokens[2]) + 0.01;
    const resultado = min1 * 10 + min5 * 2 + min15;
    console.log(resultado);
}

function cbError(fichero) {
    console.log("ERROR DE LECTURA en " + fichero);
}