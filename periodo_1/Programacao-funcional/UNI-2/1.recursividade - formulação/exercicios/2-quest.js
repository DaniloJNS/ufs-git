// 2 - N-ésimo termo da sequência {0,1,1,2,3,5,8,13,21,34,55}

function fibonacci(n/* indice do termo na sequencia*/) {
    if(n==1) return 0;

    if(n==2 || n==3) return 1;

    return fibonacci(n-1) + fibonacci(n-2)
}

console.log(fibonacci(8))
//f(1) = 0
//f(2) = 1
//f(3) = 1 
//f(n) = f(n-1) + f(n-2)
// n=5 => f(5) = f(4) + f(3) = (f(3) + f(2)) + (f(2) + f(1)) = ((f(2)+f(1)) + 1) + (1 + 1) =  ((1+1)+1) + (1 + 1 ) = 5

