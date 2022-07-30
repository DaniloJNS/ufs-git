const conveterPorcentagemEmDecimal = (valorBruto) => valorBruto / 100;

const Percentual_de_um_valor = (entrada) => {
  const [valor, porcentagem] = entrada;
  return valor[0] * conveterPorcentagemEmDecimal(valor[1]);
};

const Aumento_Percentual = (entrada) => {
  const [valor, porcentagem] = entrada;
  return valor * (conveterPorcentagemEmDecimal(porcentagem) + 1);
};

const Desconto_Percentual = (entrada) => {
  const [valor, porcentagem] = entrada;
  return valor[0] * (1 - conveterPorcentagemEmDecimal(porcentagem));
};

const Margem_de_Venda = (entrada) => {
  const [valor, porcentagem] = entrada;
  return valor[0] / (1 - conveterPorcentagemEmDecimal(porcentagem));
};

const Variação_Percentual = (entrada) => {
  const [valor, porcentagem] = entrada;
  return valor / porcentagem - 1;
};

const Relação_Percentual = (entrada) => {
  const [valor, porcentagem] = entrada;
  return valor / porcentagem;
};
