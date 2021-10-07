public class Alumno : Persona
{
    private string tipo_descuento;
    private ICollection<Reserva> refReservaS;

    public Alumno(string tipo_descuento, string dni)
    {
        this.tipo_descuento = tipo_descuento;
    }
}