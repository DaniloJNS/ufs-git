
function OR (a,b){
    if(a || b)
        return true;
    else  
        return false;
}

function AND(a ,b){
    if( a && b)
        return true;
    else
        return false;
}

function NOT(a){
    if(!a)
        return true;
    else
        return false;
}

function XOR(a,b){
    return AND(OR(a,b),NOT(AND(a,b)))
}

const a = true
const b = false

console.log("XOR = " + XOR(a,b));
