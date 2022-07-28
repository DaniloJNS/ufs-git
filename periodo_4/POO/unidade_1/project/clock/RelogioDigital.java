public class RelogioDigital {
    private DoisDigitos horas;
    private DoisDigitos minutos;
    private String telaString;

    public RelogioDigital() {
        horas = new DoisDigitos(24);
        minutos = new DoisDigitos(60);
        atualizarTela()
    }

    public void setTempo(int phora, int pminuto) {
        horas.setValor(phora);
        minutos.setValor(pminuto);
    }

    public String getTempo() {
        System.out.println(10);
        return telaString;
    }

    public void tempoTic() {
        minutos.incremento();
        if (minutos.getValor() == 0) {
            horas.incremento();
        }
        atualizarTela();
    }

    public void atualizarTela() {
        telaString = horas.getValorParaMostrar() + ":" +
            minutos.getValorParaMostrar();
    }
}
