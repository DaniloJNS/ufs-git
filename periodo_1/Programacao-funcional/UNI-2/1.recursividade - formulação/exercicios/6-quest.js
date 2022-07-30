//  6 - Potência natural do número 2: 2^n. Naturamente, você não deve utilizar o operador de expoente da linguagem

function potenciaBase2(n) {
    if(n==1){
        return 2
    }
    return 2*potenciaBase2(n-1)
}

console.log(potenciaBase2(4))

// f(1) = 2
// f(2) = 2 * 2
// f(3) = 2 * 2 * 2
// f(n) = 2*f(n-1)
