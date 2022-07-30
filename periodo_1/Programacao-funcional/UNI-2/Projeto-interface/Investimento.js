const Decimal = (porcentual) => porcentual / 100;

const fjuros = (taxaJuros) => (Periodo) =>
    Math.pow(1 + Decimal(taxaJuros), Periodo);

const calcValorFuturo = (valorPresente) => (juros) => valorPresente * juros;

const calcValorPeriodicoFuturo = (ValorPeriodico) => (juros) => (taxaJuros) =>
    (ValorPeriodico * (juros - 1)) / Decimal(taxaJuros);

const calcValorPresente = (valorFuturo) => (juros) => valorFuturo / juros

const calcValorPeriodicoPresente = (valorPeriodico) => (juros) =>
    (jurosIncrementado) =>
        valorPeriodico * (1 - juros) / (juros - jurosIncrementado)

const InvestimentoValor_Futuro = (entradas) => {
  const [taxaJuros, Periodo, ValorPeriodico, valorPresente] = entradas;
  return (calcValorFuturo(valorPresente)(fjuros(taxaJuros)(Periodo)) +
          calcValorPeriodico(ValorPeriodico)(fjuros(taxaJuros)(Periodo))(
              taxaJuros))
      .toFixed(2);
};

const InvestimentoValor_Presente = (entradas) => {
  const [taxaJuros, Periodo, valorPeriodico, valorFuturo] return calcValorPresente(
      valorPresente)(juros(Decimal(taxaJuros))(Periodo)) +
      calcValorPeriodicoPresente(valorPeriodico)(juros(Decimal(taxaJuros))(
          Periodo))(juros(Decimal(taxaJuros))(Periodo + 1));
};

const InvestimentoValor_Periodico = (entradas) => { return entradas; };

const InvestimentoPeríodos = (entradas) => { return entradas; };

const InvestimentoTaxa_de_Juros = (entradas) => { return entradas; };
