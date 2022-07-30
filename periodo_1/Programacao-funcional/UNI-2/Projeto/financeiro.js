// Percentuais



const Percentual = (porcento) => (Valor) => Valor * (porcento/100)

const AumentoPorcentual = (porcento) => (Valor) => Valor * (porcento/100+1)

const DescontoPercentual = (porcento) => (Valor) => Valor * (porcento/100-1)

const VariacaoPercentual = (valorAntes) => (valorDepois) => valorAntes/valorDepois - 1

const RelacaoPorcentual = (valor) => (ValorReferencia) => (valor)/(ValorReferencia)





