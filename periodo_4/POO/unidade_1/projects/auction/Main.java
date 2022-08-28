import java.util.Random;

public class Main {
    public static void main(String[] args) {
        Auction auction = new Auction();

        Person person1 = new Person("maria");
        Person person2 = new Person("mauro");
        Person person3 = new Person("renata");
        Person person4 = new Person("carlos");

        auction.enterLot("a good computer");
        auction.enterLot("an beautiful phone");
        auction.enterLot("an fast car");

        auction.makeABid(1, person1, 30000);
        auction.makeABid(1, person2, 30000);
        auction.makeABid(1, person3, 50000);


        auction.showLots();
    }
}
