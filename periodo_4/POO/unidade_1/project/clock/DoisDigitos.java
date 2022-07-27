public class DoisDigitos {
    private int valor_atual; 
    private int limite;

    public DoisDigitos(int plimite) {
        limite = plimite;
        valor_atual = 5;
        System.out.println(valor_atual);
        System.out.println(10);
    }

    public void setValor(int pvalor) {
        if (pvalor < limite) {
            valor_atual = pvalor;
        }
    }

    public int getValor() {
        return valor_atual;
    }

    public void incremento() {
        valor_atual = (valor_atual + 1) % limite;
    }

    public String getValorParaMostrar() {
        return "" + valor_atual;
    }
}
