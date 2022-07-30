const calcularResultado = (input) => {
  return geraResultado(formataEntrada(input)(conveterEmNumero)(juntaNumeros));
};

const geraResultado = (lista) => {
  if (lista.includes("(") && lista.includes(")")) {
    return geraResultado(calculadoraParenteses(lista)(encontraParenteses));
  } else return Calculadora(lista);
}; // funcao que processa a entrada verificando se esta tem ou não parentees

// funções para o tratamento da entrada {
const formataEntrada = (entrada) => (fconveter) => (fjunta) => {
  return fjunta(fconveter(entrada.split("")));
}; // função para transforma entrada em uma lista que seja possivel
// realizar os calculos.

const conveterEmNumero = (lista) =>
  lista.reduce((resultado, valorAtual) => {
    if (isNaN(Number(valorAtual))) {
      resultado.push(valorAtual);
    } else {
      resultado.push(Number(valorAtual));
    }
    return resultado;
  }, []); // Essa função transforma todas as posição de uma lista que tem numeros

// em formato de string para seu formato real (number)

const juntaNumeros = (lista) =>
  lista.reduce((resultado, valorAtual) => {
    if (
      typeof valorAtual === "number" &&
      typeof resultado[resultado.length - 1] === "number"
    ) {
      resultado[resultado.length - 1] =
        resultado[resultado.length - 1] * 10 + valorAtual;
    }
    if (
      typeof valorAtual === "number" &&
      resultado[resultado.length - 1] == "."
    ) {
      resultado.pop();
      resultado[resultado.length - 1] += valorAtual / 10;
    } else {
      resultado.push(valorAtual);
    }
    return resultado;
  }, []); // esta Função junta os digitos de um numeros que estão espaços
//  em varias posição, como por exemplo,
// [3, 3, '+', 3, 3 , 3] => [33 , '+', 333  ]

// }

// funções para os calculos envolvendo parentes {

const calculadoraParenteses = (lista) => (func) => {
  const [a, b] = func(lista)(0)(0);
  return geraLista([a, b + 1])(lista)(Calculadora(lista.slice(a + 1, b)))([]);
}; // essa função processa entrada calculando a parte desta que esta entre
// obj.parent('selector expression')

const encontraParenteses = (lista) => (acc) => (indice) => {
  const [x, ...xs] = lista;

  if (x == "(") {
    return encontraParenteses(xs)(acc + 1)(acc);
  } else if (x == ")") {
    return [indice, acc];
  }

  return encontraParenteses(xs)(acc + 1)(indice);
}; // Esta função indica as posições de parenteses na lista da entrada, se
// existe um conjunto de parenteses dentro de outro retorna o mais
// interno, ela retorna um array em que a primeira posição é o indice do
// parenteses de abertura e a segunda é do pararenteses de fechamento. É
// uma função auxilar de calculaParenteses

//};

// funções para realizar os calculos {

const Calculadora = (lista) => {
  const operandos = contadorOperadores(lista);
  if (operandos.raiz.length != 0) {
    return Calculadora(execCalculo(lista)(operandos.raiz[0])(Raiz));
  } else if (operandos.potencia.length != 0) {
    return Calculadora(execCalculo(lista)(operandos.potencia[0])(potencia));
  } else if (operandos.multiplicacao.length != 0) {
    return Calculadora(
      execCalculo(lista)(operandos.multiplicacao[0])(multiplicacao)
    );
  } else if (operandos.divisao.length != 0) {
    return Calculadora(execCalculo(lista)(operandos.divisao[0])(divisao));
  } else if (operandos.subtracao.length != 0) {
    return Calculadora(execCalculo(lista)(operandos.subtracao[0])(subtracao));
  } else if (operandos.soma.length != 0) {
    return Calculadora(execCalculo(lista)(operandos.soma[0])(soma));
  }
  return lista;
}; // essa função gerencia a ordem em que as operações devem
// ser realizadas recursivamente, sendo as raizes e potencias
// primeiro, em seguida, multiplicação e divisão, por fim, soma
// e subtração, respeitando, assim, as regras da aritmetrica.

const contadorOperadores = (lista) =>
  lista.reduce(
    (listaFinal, valorAtual, _indice) => {
      if (valorAtual == "+") {
        listaFinal.soma.push(_indice);
      } else if (valorAtual == "-") {
        listaFinal.subtracao.push(_indice);
      } else if (valorAtual == "*") {
        listaFinal.multiplicacao.push(_indice);
      } else if (valorAtual == "/") {
        listaFinal.divisao.push(_indice);
      } else if (valorAtual == "^") {
        listaFinal.potencia.push(_indice);
      } else if (valorAtual == "√") {
        listaFinal.raiz.push(_indice);
      }

      return listaFinal;
    },
    {
      soma: [],
      subtracao: [],
      multiplicacao: [],
      divisao: [],
      raiz: [],
      potencia: [],
    }
  ); // essa função informa quantas operações estão pendentes,
// ela retorna um objeto com atributos correspondentes a cada
// operação, em que esses atributos têm um array indicando
// em que posição eles estão na lista, é uma função auxiliar
// da funçao Calculadora.

const geraLista = (indice) => (lista) => (resultado) => (novaLista) => {
  return novaLista
    .concat(lista.slice(0, indice[0]))
    .concat(resultado)
    .concat(lista.slice(indice[1]));
}; // a cada operação indicada na função calculadora é gerada
// é um novo array, essa função realiza o papel de gerar
// essa nova lista trocando os itens processados pelo resulado
// obtido por esse processamento, é uma função auxiliar da
// função execCalculo
const execCalculo = (lista) => (indice) => (operação) => {
  return geraLista([indice - 1, indice + 2])(lista)(
    operação(lista[indice - 1])(lista[indice + 1])
  )([]);
}; // essa função que executa os calculos indicados na função Calculadora
//}

// funções para realizar operações aritmetricas{
const Raiz = (expoente) => (base) => {
  return [Math.pow(base, 1 / expoente)];
};

const potencia = (base) => (expoente) => {
  return [Math.pow(base, expoente)];
};

const multiplicacao = (x) => (y) => {
  return [x * y];
};

const divisao = (x) => (y) => {
  return [x / y];
};

const subtracao = (x) => (y) => {
  return [x - y];
};

const soma = (x) => (y) => {
  return [x + y];
};
//}

console.log(calcularResultado("(3+3)*3"));

