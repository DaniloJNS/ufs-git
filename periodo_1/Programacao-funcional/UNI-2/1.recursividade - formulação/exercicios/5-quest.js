// 5 - Fatorial de um número natural qualquer

function Fatorial (n) {
    if(n==0){
        return 1
    }

    return n * Fatorial(n-1)
}
console.log(Fatorial(3))

// f(n) = n * f(n-1)
// n= 5 => f(5) = 5 * f(4) = 5 * 4 * f(3) = 5 * 4 * 3 * f(2) = 5 * 4 * 3 * 2 * 1