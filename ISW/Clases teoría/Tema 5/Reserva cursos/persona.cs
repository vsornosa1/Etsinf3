public abstract class Persona
{
    private string dni;

    public Persona(string dni)
    {
        this.dni = dni;
    }

    public string getDni()
    {
        return this.dni;
    }
}