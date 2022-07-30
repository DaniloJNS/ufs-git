//Juros Simples

const ValorFuturo = (valorPresente) => (Juros) =>(T) => valorPresente*(1+Juros/100*T)

const ValorPresente = (valorFuturo) => (Juros) => (T) => valorFuturo/(1 +Juros/100*T)

const Juros = (valorPresente) => (valorFuturo) => (T) => (valorPresente-valorFuturo)/(valorPresente*T)

const Periodo = (valorPresente) => (valorFuturo) => (juros) => (valorPresente - valorFuturo)/(valorFuturo * juros / 100)

// Juros Composto

const valorFuturoComposto = (valorPresente) => (juros) =>(T) => valorPresente * (1+juros/100)**T

const ValorPresenteComposto = (valorFuturo) => (Juros) => (T) => valorFuturo / (1 + juros/100) ** T

const JurosComposto = (valorPresente) => (valorFuturo) => (T) => ((valorFuturo)/(valorPresente))**(1/T)

const PeriodoComposto = (valorPresente) => (valorFuturo) => (juros) =>  Math.log(valorFuturo/valorPresente)/Math.log(1+juros/100)


// Conversão de Taxa de juros

// in comming


//Dinheiro no tempo

