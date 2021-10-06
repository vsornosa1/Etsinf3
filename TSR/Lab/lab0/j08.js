// Uso de operaciones asíncronas, aquí modeladas con la función setTimeout.
// Note el valor de i asociado a las ejecuciones de las temporizaciones.
// Uso de la sentencia let.

// Nota -> x toma el valor de (i+2) 
for(let i = 0; i < 10; i++){ 
  setTimeout(
    function(x) { 
      return function() { 
        console.log(x);
      } 
    }(i+2), i*1000);
}


// ¿Cuál es el resultado de la ejecución de la sentencia
// console.log("i= ",i);
// ?
// RESPUESTA: Error - i es una variable local no definida fuera del bucle for

// Esta sentencia se ejecuta primero porque console.log(x) tiene un timeout asociado
console.log("Terminado codigo script");


