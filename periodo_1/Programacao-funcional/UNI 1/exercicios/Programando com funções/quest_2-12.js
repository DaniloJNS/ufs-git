//

function fdelta(a,b,c){
    return Math.pow(b,2) - 4 * a * c
}

function fraiz1(a, b, c){
    return (-b+fdelta(a,b,c))/(2*a)
}

function fraiz2(a, b, c){
    return (-b-fdelta(a,b,c))/(2*a)
}

const a = 2
const b = 7
const c = 3

console.log(`As raizes da equação com coeficientes a = ${a}, b = ${b}, c= ${c} eh \n x1 = ` +fraiz1(a,b,c) + "\n x2 = " + fraiz2(a,b,c))