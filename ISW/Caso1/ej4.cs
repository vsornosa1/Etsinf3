Profesor creador = new Profesor();
Conversacion c = new Conversacion(texto, modificado, creador);
Conversacion inicial = new Conversacion(texto, modificado, creador);
c.addConversacion(inicial);
creador.addConversacion(c);


