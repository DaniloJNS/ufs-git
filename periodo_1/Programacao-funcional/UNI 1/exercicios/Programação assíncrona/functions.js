let nomes = ['thiago','carlos','pereira','thiago'];

const totais = nomes.reduce((acumulador, elemento) => {
    acumulador[elemento] = (acumulador[elemento] || 0) + 1;
    return acumulador;
}, {})

console.log(totais);

var numeros = [0, 1, 2, 3, 4, 5, 6];
var total = numeros.reduce((acumulador, numero, indice, original) => {
console.info(`${acumulador} total até o momento`);
console.log(`${numero} valor atual`);
console.log(`${indice} indice atual`);
console.log(`${original} array original`);
const qtdPalavra =  original.filter((x) => x== numero).length
acumulador.push({palavra: numero,qtd: qtdPalavra})
return acumulador;
}, [])
console.warn('acaboooou');
console.log(total)

/*const exec = remover(conveter)(transfor).reduce((acumulador, elemento) => {
    acumulador[elemento] = (acumulador[elemento] || 0) + 1;
    return acumulador;
}, {});*/


/*const texto = conveter.replace(/[^a-zA-Z- -]/g, "").split(" ").reduce((acumulador, elemento) => {
    acumulador[elemento] = (acumulador[elemento] || 0) + 1;
    return acumulador;
}, {});*/


/*const remover = (lista)=>(f) => lista.map(f)

const transfor = (x) => "mr."+ x*/



/*const nomes = ['OrganizaÃ§Ã£o', 'Mundial', 'SaÃºde']

const sigla = (acc, x) => acc + x[0]
console.log(nomes.reduce(sigla,''))

const numLetras = (acc, x) => acc + x.length
console.log(nomes.reduce(numLetras,0))*/
/*const str = "abc's test#s";
console.log(texto.replace(/[^a-zA-Z- -]/g, "").split(" "));*/

//console.log(texto.split('$(').join('->').split(')').join('<-'))
/*const regexData = /\d{2}\/\d{1,2}\/\d{4}/g 

const regexDataResumida = /\d{2}\/\d{4}/g
const regexDinheiro = /(R\$)\s(\d*,\d*)/g;

const regexGeral = /[^a-zA-Z- -]/g

const regexLista = [regexData, regexDinheiro, regexGeral]*/