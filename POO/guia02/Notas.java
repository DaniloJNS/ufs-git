public class Notas {

	private String nome;
	private float nota1;

    public Notas(String nome, float nota1) {
		this.nome = nome;
		this.nota1 = nota1;
    }

	public void setNome(String nome) {
    	this.nome = nome;
	}

	public String getNome() {
    	return nome;
	}

	public void setNota1(float nota1) {
    	this.nota1 = nota1;
	}

	public float getNota1() {
    	return nota1;
	}

}
