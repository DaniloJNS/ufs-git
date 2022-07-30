function seila (v_txt){
    v_txt = (""+v_txt).toLowerCase().trim();
    if (v_txt.length == 0) {
        return "";
    }
    console.log(v_txt)
    v_txt = v_txt.replace(/\./g, '').replace(/,/g, '.').replace(/;/g, ','); // 2.000,10 -> 2000.10
    v_txt = v_txt.replace(/(ˆ|\^)/g, '**'); // ^ -> **
    v_txt = v_txt.replace(/(:|÷)/g, '/'); // : -> /
    v_txt = v_txt.replace(/×/g, '*'); // × -> *
    v_txt = v_txt.replace(/([ \d)])x([ \d(])/g, '$1*$2'); // x -> * lefting x inside words
    v_txt = v_txt.replace(/(\(.+\))%/g, '($1/100)').replace(/(\d*\.?\d+)%/g, '($1/100)');  // % -> /100
    v_txt = v_txt.replace(/([ \d\)])mod([ \d\()])/g, '$1%$2'); // mod -> %
    v_txt = v_txt.replace(/(\d)(\()/g, '$1*$2').replace(/(\))(\d)/g, '$1*$2'); // 2( -> 2*(
    v_txt = v_txt.replace(/(pi)([\(\d])/g, '$1*$2').replace(/([\)\d])(pi)/g, '$1*$2'); // PI( -> PI*(
    console.log(v_txt)
    
    try {
        v_txt = eval(v_txt);
        // console.log(v_txt+"")
        if ((v_txt + "").indexOf("function ") != -1) {
            v_txt = "INCOMPLETO";
        }
    } catch (e) {
        v_txt = "INCOMPLETO";
    }
    
    if (isNaN(v_txt)) {
        v_txt = "Incompleto"
        return v_txt;
    } else {
        console.log(v_txt)
        return v_txt;
        return parseFloat(v_txt);
    }
}




const $dados = document.querySelector("#num1")

var form = document.getElementById('formulario');
var campo = document.getElementById('campo');
var p = document.getElementById('resultado')

form.addEventListener('submit', function(e) {
    // alerta o valor do campo
    render();
    render2();
    // impede o envio do form
    e.preventDefault();
});

campo.addEventListener("change", function(e) {
    // alerta o valor do campo
    render();
    render2();
    // impede o envio do form
    e.preventDefault();
} )


const mostrar = () => {
    return `<p>é isso ${campo.value}</p>`
}

const render = () => {
    const $local = document.getElementById("formulario")
    $local.insertAdjacentHTML("afterend", mostrar())
}


var form2 = document.getElementById('formulario2');
var campo2 = document.getElementById('campo2');

form2.addEventListener('submit', function(e) {
    // alerta o valor do campo
    render2();
    // impede o envio do form
    e.preventDefault();
});

campo2.addEventListener("change", function(e) {
    // alerta o valor do campo
    render2();
    // impede o envio do form
    e.preventDefault();
} )

const render2 = () => {
    p.textContent = `o valor é: `+seila(campo2.value)
}