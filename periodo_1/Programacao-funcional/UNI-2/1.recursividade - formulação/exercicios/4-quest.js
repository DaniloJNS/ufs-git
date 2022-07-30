// 4 - Soma dos n primeiros numeros naturais.

function somaNtermosNaturais(n) {
    if(n==0){
        return 0
    }
    return n + somaNtermosNaturais(n-1)
}

console.log(somaNtermosNaturais(5))

// {0,1,2,3,4,5,6...}

//f(n) = n + f(n)

//n = 5 => f(5) = 5 + f(4) = 5 + 4 + f(3) ...