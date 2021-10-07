public class Curso
{
    private string nombre;
    private int precio;
    private DateTime fecha_insercion;
    public bool cancelado() { }
    private Administrador oferta;
    private Profesor imparte;
    private ICollection<Reserva> reservaS;

    public Curso(string nombre, string precio, DateTime fecha_insercion, Administrador oferta, Profesor imparte)
    {
        this.nombre = nombre;
        this.precio = precio;
        this.fecha_insercion = fecha_insercion;
        this.oferta = oferta;
        this.imparte = imparte;
    }
}