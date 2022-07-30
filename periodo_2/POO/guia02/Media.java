import java.util.Scanner;

public class Media {
    public static void main(String[] args) {
        String nome = "";
        double nota1 = 0.0;
        double nota2 = 0.0;
        double nota3 = 0.0;
        double media = 0.0;


        Scanner entrada = new Scanner(System.in);
        
        System.out.print("Informe seu nome: ");
        nome = entrada.nextLine();
        System.out.print("Informe o valor da nota 1: ");
        nota1 = entrada.nextInt();
        System.out.print("Informe o valor da nota 2: ");
        nota2 = entrada.nextInt();
        System.out.print("Informe o valor da nota 3: ");
        nota3 = entrada.nextInt();

        media = (nota1 + nota2 + nota3)/3;

        System.out.print("Olá "+nome+" a sua média é: "+media);
    }
}
