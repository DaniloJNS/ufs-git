public class Principal {
    public static void main(String[] args) throws InterruptedException {
        RelogioDigital relogio1 = new RelogioDigital();

        while (true) {
            long time = 1000;

            Thread.sleep(time);

            String telaString = relogio1.getTempo();
            
            System.out.println( telaString );

            relogio1.tempoTic();
        }
    }
}
