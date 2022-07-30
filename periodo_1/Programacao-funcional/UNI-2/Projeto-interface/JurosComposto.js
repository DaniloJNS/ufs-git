const percentual = (valorBruto) => valorBruto / 100;

const juros = (TaxaJuros) => (Periodo) =>
  Math.pow(1 + percentual(TaxaJuros) / Periodo);

const JCValor_Futuro = (entrada) => {
  const [valorPresente, taxaJuros, Periodo] = entrada;
  return valorPresente * juros(taxaJuros)(Periodo);
};
const JCValor_Presente = (entrada) => {
  const [valorFuturo, taxaJuros, Periodo] = entrada;
  return valorFuturo / juros(taxaJuros)(Periodo);
};
const JCTaxa_de_Juros = (entrada) => {
  const [valorFuturo, valorPresente, Periodo] = entrada;
  return (valorFuturo / valorPresente) ** (1 / Periodo);
};
const JCNúmero_de_Períodos = (entrada) => {
  const [valorFuturo, valorPresente, taxaJuros] = entrada;
  return (
    Math.log10(valorFuturo / valorPresente) /
    Math.log10(1 + percentual(taxaJuros))
  );
};
// interface esta com erros
const JCConversões = (entrada) => {
  return entrada[0] + " " + entrada[1] + " " + entrada[2];
};
