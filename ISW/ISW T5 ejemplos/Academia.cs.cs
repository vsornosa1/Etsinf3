public abstract class Persona
{
    public Persona(string dni)
    {
        this.dni = dni;
    }

    private string dni;
}


// ----------------------------------------------------------------


public class Administrador : Persona
{
    public Administrador()
    {

    }
    private ICollection<Curso> Oferta;

    public void oferta() { }
    public void borra() { }
}


// ----------------------------------------------------------------


public class Profesor : Persona
{
    public Profesor()
    {

    }

    private ICollection<Curso> Imparte;

    public void imparte() { }
}


// ----------------------------------------------------------------


public class Alumno : Persona
{

    public Alumno(string tipo_descuento)
    {
        this.tipo_descuento = tipo_descuento;
        reservas = new ICollection<Reserva>();
    }
    private string tipo_descuento;
    private ICollection<Reserva> reservas;
}


// ----------------------------------------------------------------


public class Curso
{
    private string nombre;
    private int precio;
    private bool cancelado();
    private DateTime fecha_insercion;

    private Administrador Oferta;
    private Profesor Imparte;
    private ICollection<Reserva> reserva;
}


// ----------------------------------------------------------------


public class Reserva
{
    private DateTime fecha;
    private Alumno reservaAlumno;
    private Curso reservaCurso;
    private Matricula Tiene;
}


// ----------------------------------------------------------------


public class Matricula
{
    private Reserva Tiene;

    private int importe();
}

// ----------------------------------------------------------------
