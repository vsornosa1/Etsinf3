
//Uso de funciones, objetos, cláusula this y función bind().

console.log("\n");

function g() { return this.valor; }

var obj01 = { valor:-5*2,
	      fun: function(){
				  var gg = g.bind(this); // this == obj01
				  return gg;
			   }
	    }
	    
console.log("obj01.valor:  ", obj01.valor);
console.log("obj01.fun()():  ", obj01.fun()()); // Invoca a lo que devuelve la variable gg

console.log("__________________________________________________________\n");

fx1 = obj01.fun();
console.log("fx1():  ", fx1());

var obj02 = { valor: "fun no me referencia",
	        fun: fx1,
	        fan: g
	      }
	      
console.log("obj02.valor:  ", obj02.valor);	    
console.log("obj02.fun():  ", obj02.fun()); // Invoca a la variable gg
console.log("obj02.fan():  ", obj02.fan());

console.log("__________________________________________________________\n");

obj01.valor = "Nuevo valor";
console.log("obj01.valor:  ", obj01.valor);
console.log("obj02.fun():  ", obj02.fun()); // obj02.fun() -> fx1 -> obj01.fun() -> obj01.valor
console.log("obj02.fan():  ", obj02.fan()); 
