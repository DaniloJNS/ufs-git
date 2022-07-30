const Percentual = (valorBruto) => valorBruto / 100;

const Juros = (taxaJuros) => (Periodo) => 1 + taxaJuros * Periodo;

const JSValor_Futuro = (entrada) => {
  const [valorPresente, TaxaJuros, Periodo] = entrada;
  return valorPresente * juros(Percentual(TaxaJuros))(Periodo);
};
const JSValor_Presente = (entrada) => {
  const [valorFuturo, TaxaJuros, Periodo] = entrada;
  return valorFuturo / juros(Percentual(TaxaJuros))(Periodo);
};
const JSTaxa_de_Juros = (entrada) => {
  const [valorPresente, valorFuturo, Periodo] = entrada;
  return (valorFuturo - valorPresente) / (valorPresente * Periodo);
};
const JSNúmero_de_Períodos = (entrada) => {
  const [valorFuturo, valorPresente, taxaJuros] = entrada;
  return (valorFuturo - valorPresente) / (valorPresente * taxaJuros);
};
