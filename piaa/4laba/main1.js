function prefic(S){
    let m = S.length
    let result =new Array(m).fill(0)
    for(let i = 1; i < m; i++){
        let k = result[i-1]

        while(k > 0 && S[i] != S[k]){
            k = result[k-1]
        }
        if( S[i] == S[k]){
            k++
        }
        result[i] = k
    }
    return result
}

function func(S, T){
    if (!S || !T) {
        return [] 
    }

    let k = 0
    let pi = prefic(S)
    let A = []
    for(let i = 0; i <  T.length; i++){
        while (k > 0 && T[i] != S[k]){
            k = pi[k-1]
        }
        if ( T[i] == S[k]){
            k++
        }
        if (k == S.length){
            A.push(i-S.length + 1)
            k = pi[k-1]
        }
    }

    return A

}

const readline = require('readline')

const rl = readline.createInterface(
    {
        input: process.stdin,
        output: process.stdout
    }
)
let lines = []

rl.on('line', line => {
    lines.push(line.trim())
})

rl.on('close', () => {
    const A = lines[0]
    const B = lines[1]

    const res = func(A, B)
    if(res.length > 0) {
        console.log(res.join(","))
    } else {
        console.log(-1)
    }
})