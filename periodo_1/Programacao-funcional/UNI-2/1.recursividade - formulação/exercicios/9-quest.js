/* 8 - Mínimo Múltiplo Comum (MMC) entre dois inteiros postivos 
       fornecidos , n e m .*/

function MMC(n , m) {
    if(n == m) {
        return n
    }
    else if(n > m) {
        return MMC(n,m)
    }
        return MMC(n,m)
    
}
function ResDiv(n,m) {
    if(n < m)
        return n
    return ResDiv(n-m, m)
}

//f(2,3) = 3

console.log(ResDiv(10,3))