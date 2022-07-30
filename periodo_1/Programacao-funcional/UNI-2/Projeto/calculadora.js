function replace(text) {
  text = ("" + text).toLowerCase().trim();
  if (text.length == 0) {
    return "";
  }
  text = text.replace(/\./g, "").replace(/,/g, ".").replace(/;/g, ","); // 2.000,10 -> 2000.10
  text = text.replace(/(ˆ|\^)/g, "**"); // ^ -> **
  text = text.replace(/(:|÷|\/)/g, "/"); // : -> /
  text = text.replace(/×/g, "*"); // × -> *
  text = text.replace(/([ \d)])x([ \d(])/g, "$1*$2"); // x -> * lefting x inside words
  text = text
    .replace(/(\(.+\))%/g, "($1/100)")
    .replace(/(\d*\.?\d+)%/g, "($1/100)"); // % -> /100
  text = text.replace(/([ \d\)])mod([ \d\()])/g, "$1%$2"); // mod -> %
  text = text.replace(/(\d)(\()/g, "$1*$2").replace(/(\))(\d)/g, "$1*$2"); // 2( -> 2*(
  text = text
    .replace(/(pi)([\(\d])/g, "$1*$2")
    .replace(/([\)\d])(pi)/g, "$1*$2"); // PI( -> PI*(

  if (isNaN(text)) {
    text = "Incompleto";
    return text;
  } else {
    return parseFloat(text);
  }
}

console.log(replace("3+3/6"));

const input = "3+3+3+3";

const regexOperadores = /(:|÷|\/|\+|-|\*)/g;

const regexNumero = /\d+/g;

const regexSub = /\@\@+/g;
const substituiTraço = "@";
const substituiPipe = "";

const regex = (texto) => (regex) => (sub) => texto.replace(regex, sub);

const preListNumber = regex(input)(regexOperadores)(substituiTraço);

const preListOperandos = regex(input)(regexNumero)(substituiTraço);

console.log(preListNumber);
console.log(preListOperandos);

const Transf = (texto) => texto.split("@");

console.log(Transf(preListNumber));

console.log(Transf(preListOperandos.replace(/(^@+|@+$)/, "")));

function calcList(lista, operadores) {
  if (operadores != null) {
    const [op, ...ops] = operadores;
    const [x, ...xs] = lista;
    if(op == "+") {
      return Number(x) + calcList(xs, ops) ;
    }
  }
  return 0;
  }
  console.log(calcList(Transf(preListNumber),Transf(preListOperandos.replace(/(^@+|@+$)/, ""))))
