public class Main {
    public static void main(String[] args) {
        RelogioDigital relogio1 = new RelogioDigital();
        relogio1.tempoTic();
        String telaString = relogio1.getTempo();


        System.out.println(telaString);
    }
}
