import java.util.Scanner;

public class scan {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        System.out.print("Digite seu nome: ");
        String nome = in.nextLine();
        System.out.print("Digite sua idade: ");
        int idade = in.nextInt();
            System.out.print("Seu nome é " + nome +", sua idade é " + idade + " anos");
    }
    
}
