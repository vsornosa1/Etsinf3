public class ApHORA
{
    private ICollection<Asignatura> asignaturas;
    private ICollection<Profesor> profesores;

    public ApHORA(Asignatura asignatura, Profesor profesor)
    {
        this.asiganturas = new List<Asignatura>();
        this.asignaturas.Add(asignatura);
        this.profesores = new List<Profesor>();
        this.profesores.Add(profesor);
    }
}


// ----------------------------------------------------------------


public class Asignatura
{
    private string nombre;
    private int codigo;
    private string curso;
    private ApHORA apHORA;
    private ICollection<Profesor> profesores;
    private ICollection<Teoria> teorias;
    private ICollection<Practica> practicas;

    // Se relaja ApHora
    public Asignatura(string nombre, int codigo, string curso, Profesor profesor, Teoria teoria, Practica practica)
    {
        this.nombre = nombre;
        this.codigo = codigo;
        this.curso = curso;
        this.profesores = new List<Profesor>();
        this.profesores.Add(profesor);
        this.teorias = new List<Teoria>();
        this.teorias.Add(teoria);
        this.practicas = new List<Practica>();
        this.practicas.Add(practica);
    }

}


// ----------------------------------------------------------------


public class Teoria : Grupo
{
    private Asignatura asignatura;

    // Se relaja Asignatura
    public Teoria(int cod_g, int tamaño, DateTime franja_hor, Profesor profesor, Aula aula)
    : base(cod_g, tamaño, franja_horaria, profesor, aula)
    {

    }
}


// ----------------------------------------------------------------


public class Practica : Grupo
{
    private Asignatura asignatura;

    public Practica(Asignatura asignatura, int cod_g, int tamaño, DateTime franja_horaria, Profesor profesor, Aula aula)
    : base(cod_g, tamaño, franja_horaria, profesor, aula)
    {
        this.asignatura = asignatura;
    }
}


// ----------------------------------------------------------------


public class Grupo
{
    private int cod_g;
    private int tamaño;
    private DateTime franja_horaria;
    private Profesor asignado;
    private Aula asigna;

    public Grupo(int cod_g, int tamaño, DateTime franja_hor, Profesor profesor, Aula aula)
    {
        this.cod_g = cod_g;
        this.tamaño = tamaño;
        this.franja_hor = franja_hor;
        this.asignado = profesor;
        this.asigna = aula;
    }
}


// ----------------------------------------------------------------


public class Profesor
{
    private string nombre;
    private int codigo;
    private ApHORA aphora;
    private ICollection<Asignatura> asignaturas;
    private ICollection<Grupo> grupos;

    // Se relaja ApHora, Asignatura, Grupo
    public Profesor(string nombre, int codigo)
    {
        this.nombre = nombre;
        this.codigo = codigo;
        this.asignaturas = new List<Asignatura>();
        this.grupos = new List<Grupo>();
    }

}


// ----------------------------------------------------------------


public class Aula
{
    private int cod_a;
    private int capacidad;
    private ICollection<Grupo> grupos;

    // Se relaja Grupo
    public Aula(int cod_a, int capacidad)
    {
        this.cod_a = cod_a;
        this.capacidad = capacidad;
        this.grupos = new List<Grupo>();
    }
}

// ----------------------------------------------------------------
