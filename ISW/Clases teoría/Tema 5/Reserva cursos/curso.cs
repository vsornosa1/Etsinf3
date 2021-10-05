public class Curso
{
    private string nombre;
    private int precio;
    private DateTime fecha_insercion;
    public bool cancelado() { }
    private Administrador oferta;
    private Profesor imparte;
    private ICollection<Reserva> reservaS;
}