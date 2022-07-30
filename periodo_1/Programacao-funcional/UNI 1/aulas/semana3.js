const fareaEli = ( x, y, pi = 3.14) => pi*x*y

const x = 2
const y = 5

console.log(` A area da elipse é : ${fareaEli( x , y)}`)

const quadrado = (z) => Math.pow(z,2)

const z = 12

console.log(` o valor de ${z} a quarta potência eh : ${quadrado(quadrado(z))}`)

const nome = "Fulano Santos"

const sobrenome = nome.substring(nome.search(" "),nome.length)

//subtring é um metodo que divide uma string
//seach é um método que procura um char na string e retorna a sua posição na string
//length é o tamanho da string

const resultado = `${sobrenome} , ${nome.substring(0,1)}`

const resultado2 = nome.concat(sobrenome)

console.log(` o result =  ${resultado2}`)
console.log(` o resultado é  ${resultado}`)