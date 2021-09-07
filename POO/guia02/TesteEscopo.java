public class TesteEscopo {
    String nome = "Danilo Jose";
    int idade = 18;

    public void imprimeDados() {
        double altura =1.70;

        for (int i = 0; i < 3; i++) {
            System.out.print(i);
        }
        System.out.print("nome:"+nome+"\nIdade"+idade);
        System.out.print("Altura:"+altura);
    }

}
