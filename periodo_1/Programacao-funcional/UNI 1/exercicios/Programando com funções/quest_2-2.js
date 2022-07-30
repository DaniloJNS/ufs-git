//2.2 Programa para calcular a área de uma circunferência dado o valor do raio.

function AreaCirculo(x)
{
    return pi*Math.pow(x,2)// A = pi*r^2
}

const pi = 3.14
const r=2

console.log(`Area do circulo de raio ${r} é: ` + AreaCirculo(r))