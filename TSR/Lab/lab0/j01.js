
//Decalaración de variables. Uso de funciones y clausuras.

//Variables

var x;
var w, ww;
var inf = Infinity;

//Funciones

function f(y,z){
	x = function(){ console.log("función x:      ",y,"     ",z,"    ",inf);};
	w = function(inf){ 
		console.log("función w:      ",y,"     ",z,"   ",inf);
		ww = function(){ 
			console.log("función ww:     ",y,"     ",z,"   ",inf); 
			return y + z + inf;
		};
	};
	console.log(`argumentos de f: ${y}  ---  ${z}`);
}

//Ejecución del código

f(0,100);
x();
w(-1000);

console.log("ww(): " + ww());

//¿Cuál es el resultado de la ejecución de la sentencia
//console.log("u= ",u);
//?
