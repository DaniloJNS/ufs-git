const fs = require('fs')
const path = require('path')

const caminho1 =
    path.join(__dirname, 'dados1.txt') // puxa os arquivos dos diretorios

const caminho2 =
    path.join(__dirname, 'dados2.txt') // puxa os arquivos dos diretorios

const conteudo1 = fs.readFileSync(caminho1) // ler os os arquivos

const conteudo2 = fs.readFileSync(caminho2) // ler os os arquivos

const conteudo = conteudo1 + conteudo2 // junta os dados dos arquivos

const converteemString = (dados) =>
    dados.toString().toLowerCase() // transforma o conteudo gerado pelos
                                   // arquivos em string com letras minusculas

const transformaemLista = (texto) =>
    texto.split("-") // transforma o texto em lista de palvras

const substituiCaracterEspecial = (texto) =>
    texto.normalize('NFD')
        .replace(/[\u0300-\u036f]/g, '') // Remove acentos
        .replace(/([^\w]+|\s+)/g,
                 '-')        // Substitui espaço e outros caracteres por hífen
        .replace(/\d+/g, '') // Remove numeros
        .replace(/\-\-+/g, '-') // Substitui multiplos hífens por um único hífen
        .replace(/(^-+|-+$)/,
                 ''); // Remove hífens extras do final ou do inicio da string

const listaPalavras = (conteudo) =>
    transformaemLista(substituiCaracterEspecial((converteemString(conteudo))))

const contadorPalavras = (Palavras) => Palavras.reduce(
    (listaFinal, palavra, _indice, lista) => {
      const qtdPalavra = lista.filter((x) => x == palavra)
                             .length // quantifica as palavras exitem no array

      listaFinal.filter((y) => y.palavra == palavra).length == 0
          ? listaFinal.push({
              palavra : palavra,
              qtd : qtdPalavra,
              '%' : ((qtdPalavra / (lista.length)) * 100).toFixed(2)
            })
          : null
      // verifica se determinada palavra ja foi inserida em algum objeto do
      // array caso não exista insere no array junto com a frequencia contada no
      // texto e a porcentagem
      return listaFinal;
    },
    []) // transforma uma lista palavras em um array de objetos com atribuitos

const geraResultado = (dados) =>
    contadorPalavras(listaPalavras(dados)).sort((a, b) => b.qtd - a.qtd)
// processa os arquivos

const resultado = geraResultado(conteudo)

const caminhoResultado =
    path.join(__dirname, 'resultado.txt') // puxa o diretorio do arquivo

fs.writeFile('resultado.txt', 'Hello World!', function(err) {
  if (err)
    return console.log(err);
  console.log('Hello World > helloworld.txt');
}); // escreve o dados processados no arquivo resultado.txt

console.log(resultado)

[1, 2, 3]?.reduce((relationships) =>  (relationship) => { if (relationship.name === relationship_code) relationship.description[0] || "-" }, [])

