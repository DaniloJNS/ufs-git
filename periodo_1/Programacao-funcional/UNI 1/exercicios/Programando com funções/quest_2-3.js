function ehtriangulo( a, b ,c){
    if(a>c+b)
        return "não eh triangulo"
    else
        return "eh triangulo"
}
function maior( a, b, c){
    if(a>b && a>c)
        return ehtriangulo( a, b, c)
        else if(b>c && b>a)
        return ehtriangulo( b, a, c)
        else
        return ehtriangulo( c, a, b)
}
const a = 10
const b = 3
const c = 4

console.log(maior( a, b ,c))