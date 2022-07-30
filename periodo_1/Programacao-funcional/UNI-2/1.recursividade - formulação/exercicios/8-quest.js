/* Quociente da divisão entre dois números interios
   positivos fornecidos, n e m */

function Quociente(n,m) {
    if(n<m){
        return 0;
    }
    return 1 + Quociente(n-m,m)
}

// f(9,3) = 1 + f(6,3) = 1 + 1 + f(3,3) = 1 + 1 +1 +f(0,3) = 1 + 1 + 1 + 0 = 3
// f(n, m) = 1 + f(n-m,n)
console.log(Quociente(10,3))
