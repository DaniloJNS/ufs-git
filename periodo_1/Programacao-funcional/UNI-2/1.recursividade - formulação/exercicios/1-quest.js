// 1 - N-ésimo termo da sequência {3,6,12,24,48,..}

function recusiva (n /* termo da sequência*/) {
    if(n==1){
        return 3
    }
    return 2*recusiva(n-1)
}

console.log(recusiva(4))

// f(1) = 3
//f(n) = 2*f(n)

//n= 4 => f(4)=2*(f(3))=2*(2*(f(2)))=2*(2*(2*(f(1)))) = 2*2*2*3 = 24
