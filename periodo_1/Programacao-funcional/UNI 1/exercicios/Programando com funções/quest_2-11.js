function fmedia( x, y, z) {
    return (x+y+z)/3
}
function fMaiorMedia(x, y, z) {
    if (x > fmedia(x,y,z)) {
        return x
    }
    else if (y> fmedia(x,y,z)){
        return y
    }
    else if (z > fmedia(x,y,z)){
        return z
    }
}

const x = 2
const y = 5
const z = 10

console.log("Maior valor entre ("+x+", "+y+", "+ z+") eh "+ fMaiorMedia(x,y,z))