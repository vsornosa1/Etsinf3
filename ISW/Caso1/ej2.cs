public class Grupo
{
    private string nombre { get; set; }
    private string acronimo { get; set; }
    private DateTime fechaAlta { get; set; }
    private int numDocumentos() { }
    private ICollection<Profesor> profesorado { get; set; }
    private ICollection<Alumno> alumado { get; set; }
}

public abstract class Miembro
{
    private string nombre;
    private string email;
}

public class Alumno : Miembro
{
    private int numExpediente { get; set; }
    private ICollection<Grupo> alumnado { get; set; }
}

public class Profesor : Miembro
{
    private ICollection<Conversacion> creador { get; set; }
}

public class Conversacion
{
    private string texto { get; set; }
    private bool modificado { get; set; }
    private Profesor creador;
    private ICollection<Conversacion> inicial { get; set; }
    private ICollection<Conversacion> subconversaciones { get; set; }
}
