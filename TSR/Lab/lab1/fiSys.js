//Módulo fiSys (fiSys.js) ------------------------------------------------------------
//Ejemplo de módulo de funciones adaptadas para el uso de ficheros.
//(Podrían haberse definido más funciones.)

const fs = require("fs");

function readFile(fichero, callbackError, callbackLectura) {
    fs.readFile(fichero, "utf8", function (error, datos) {
        if (error) callbackError(fichero);
        else callbackLectura(datos);

    });
}


function readFileSync(fichero) {
    var resultado; //retornará undefined si ocurre algún error en la lectura
    try {
        resultado = fs.readFileSync(fichero, "utf8");
    } catch (e) { };
    return resultado;
}


function writeFile(fichero, datos, callbackError, callbackEscritura) {
    fs.writeFile(fichero, datos, function (error) {
        if (error) callbackError(fichero);
        else callbackEscritura(fichero);
    });
}

exports.readFile = readFile;
exports.readFileSync = readFileSync;
exports.writeFile = writeFile;