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
        return -1
    }
    let k = 0
    let pi = prefic(S)
    for(let i = 0; i <  T.length; i++){
        while (k > 0 && T[i] != S[k]){
            k = pi[k-1]
        }
        if ( T[i] == S[k]){
            k++
        }
        if (k == S.length){
            return i - S.length + 1
        }
    }

    return -1

}

function main(A, B){
    if (A.length != B.length || !A || !B) { return -1}
    let tmp = A + A
    let index = func(B, tmp)

    if (index != -1 && index <  A.length){ return index}
    return -1
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

    const res = main(A, B)
    console.log(res)
})