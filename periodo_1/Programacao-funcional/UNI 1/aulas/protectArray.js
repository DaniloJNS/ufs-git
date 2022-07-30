// Object.freeze para congelar valores de uma lista
const listacongelada = Object.freeze([3, 1, 7, 9, 4, 6])

// Tentativa de ordenar sem uso de cÃ³pia
const listaordenada = listacongelada.sort((a, b) => a - b)
console.log(listacongelada, listaordenada)

// Com uso de cÃ³pia funciona
const listaordenada = [...listacongelada].sort((a, b) => a - b)
console.log(listacongelada, listaordenada)

// Acessar partes da lista congelada funciona normalmente
const pedaco = listacongelada.slice(3)
console.log(pedaco, listacongelada)