public Grupo(string nombre, string acronimo, DateTime fechaAlta, Profesor profesorado)
{

}


public Miembro(string nombre, string email)
{

}


public Alumno(int numExpediente, Grupo alumnado) : base(nombre, email)
{

}

//  RELAJAR: Conversacion creador
public Profesor() : base(nombre, email)
{

}

//  RELAJAR: Conversacion inicial
public Conversacion(string texto, bool modificiado, Profesor creador)
{

}
