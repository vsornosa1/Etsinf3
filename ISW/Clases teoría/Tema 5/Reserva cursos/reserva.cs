public class Reserva
{
    private DateTime fecha;
    private Curso reservaCurso;
    private Alumno reservaAlumno;
    private Matricula tiene;

    public Reserva(DateTime fecha, Curso reservaCurso, Alumno reservaAlumno)
    {
        this.fecha = fecha;
        this.reservaCurso = reservaCurso;
        this.alumno = reservaAlumno;
    }
}