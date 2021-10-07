public class Profesor : Persona
{
    private ICollection<Curso> imparte;
    public void imparte() { }

    public Profesor(string dni) : base(dni)
    {

    }

}