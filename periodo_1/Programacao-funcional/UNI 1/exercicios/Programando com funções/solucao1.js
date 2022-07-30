const fs = require('fs')
const path = require('path')

const caminho = path.join(__dirname, 'dados2.txt')

const A= ['\,','\:', '\.', '\?', '\"', '\_', '\;', '\%', '\"','\!','\@','\#','\$','\¨','\&','*','(',')','-','=','+','{','}','/','°','º','ª',"'",'1','2','3','4','5','6','7','8','9','\n','\r','\t']


const conteudo = fs.readFileSync(caminho)

const remove = (lista) => lista.map((texto) => texto.replace(A," "))

const conteudo1 = conteudo.toString().toLowerCase().split(/\s* \s*/g)

const conteudo6 = remove(conteudo1)

const filter = (x) => 
console.log(conteudo)

