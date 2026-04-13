const assert = require('assert');

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

let i = 0
count = 12
finish = 0
console.log(`Количество тестов ${count}`)

i++
try{
    const iter = main('aaaaa', 'aaaaa')
    const result = 0
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

i++
try{
    const iter = main('bcdefa', 'abcdef')
    const result = 5
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

i++
try{
    const iter = main('fabcde', 'abcdef')
    const result = 1
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

i++
try{
    const iter = main("def".repeat(333) + "abc", "abc".repeat(334))
    const result = -1
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

i++
try{
    const iter = main('abcba', 'abcba')
    const result = 0
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

i++
try{
    const iter = main("a".repeat(2_500_000) + "b".repeat(2_500_000), "b".repeat(2_500_000) + "a".repeat(2_500_000))
    const result = 2_500_000
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}


i++
try{
    const iter = main("abcdefghijklmnopqrstuvwxyz".repeat(100), "zabcdefghijklmnopqrstuvwxy".repeat(100))
    const result = 25
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}



i++
try{
    const iter = main("a".repeat(5_000_000), "a".repeat(5_000_000))
    const result = 0
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

i++
try{
    const iter = main("a".repeat(5_000_000), "b".repeat(5_000_000))
    const result = -1
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

i++
try{
    const iter = main("a".repeat(4_999_999) + "b", "b" + "a".repeat(4_999_999))
    const result = 4_999_999
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

i++
try{
    const iter = main("b" + "a".repeat(4_999_999), "a".repeat(5_000_000))
    const result = -1
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

i++
try{
    const iter = main("cdeab".repeat(200000), "abcde".repeat(200000))
    const result = 3
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

console.log(`Количество успешно пройденных тестов ${finish}`)