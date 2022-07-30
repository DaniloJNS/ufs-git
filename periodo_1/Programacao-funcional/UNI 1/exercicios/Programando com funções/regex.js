const texto = `
Lista de jogos:
- Red dead redeption 2: R$ 180,00;
- The last of us 2: R$ 199,95;
- Resident Evil 2 remake: R$ 140,50;
`;
const regex = /(R\$)\s(\d*,\d*)/gmi;
const funcao = (match, p1, p2) => {
    const real = parseFloat(p2.replace(/,/g,'.'));
    const cotacao = 4.21;
    const dolar = (real / cotacao).toFixed(2)
    const result = dolar.toString().replace(/\./g,',');
    return `US$ ${result}` 
}
console.log(texto.replace(regex, funcao))

const contagem = (lista) => lista.map((texto) => texto.length)

const triplo = (lista) => lista.map((x) => 3*x)

const ordena = (lista) => lista.sort((a,b) => a-b)

const composicao = (...fns) => (lista) => fns.reduce((acc,fn) => fn(acc), lista)

const geraResultado = composicao(
    contagem,
    triplo,
    ordena
)

const nomes = ['Ana Beatriz', 'Bia', 'Guilherme', 'JoÃ£o', 'Rafael']

console.log(geraResultado(nomes))