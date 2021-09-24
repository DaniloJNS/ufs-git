import java.io.*;


public class buffer {
    public static void main(String[] args) {
        BufferedReader bf = new BufferedReader(new InputStreamReader(System.in));
        try {
            System.out.print("Digite uma frase: ");
            String frase = bf.readLine();
            System.out.print(frase);
        } catch(IOException e){
            System.out.print("Erro");
        }
    }
    
}
