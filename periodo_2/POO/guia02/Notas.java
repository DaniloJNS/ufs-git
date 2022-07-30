public class Notas {

	private String nome;
    private float[] notasU1;
    private float[] notasU2;
    private float[] notasP;
    private float notaF;

	public Notas(String nome, float[] notasU1, float[] notasU2, float[] notasP) {
	    this.nome = nome;    
        this.notasU1 = notasU1;
        this.notasU2 = notasU2;
        this.notasP = notasP;
	}
	public String getNome() {
		return nome;
	}
    private float notaUnidade(int unidade) {
        return (this.notasU1[unidade] + this.notasU2[unidade]) * 0.2f +
            this.notasP[unidade] * 0.6f;
    }
    public float notaFinal() {
        if (notaF == 0) {
          for (int i = 0; i < 3; ++i)
            notaF += notaUnidade(i);
          notaF /= 3;
        }
        return notaF;
    }
	public static void main(String[] args) {
        float[] notasU1 = {1,2,3},
                notasU2 = {1,2,3},
                notasP  = {1,2,3};
		Notas umALuno = new Notas("danilo", notasU1, notasU2, notasP);
		System.out.print("nome: "+ umALuno.getNome()+ ", notaF: " + umALuno.notaFinal());
	}
};
