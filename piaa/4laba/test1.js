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

let i = 0
count = 15
finish = 0
console.log(`Количество тестов ${count}`)

i++
try{
    const iter = func('abab', 'dsdaababkk')
    const result = [4]
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

i++
try{
    const iter = func('qqq', 'dsadesfsg')
    const result = []
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

i++
try{
    const iter = func('abc', 'dfddabchjkkabcjkljljjabc')
    const result = [4, 11, 21]
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

i++
try{
    const iter = func('банан', 'я ем вкусный банан')
    const result = [13]
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

i++
try{
    const iter = func('qqq', 'qqqqqqq')
    const result = [0, 1, 2, 3, 4]
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

i++
try{
    const iter = func('fdsfg', 'ds')
    const result = []
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

i++
try{
    const iter = func('!@#', 'dsade!@#sfsg!')
    const result = [5]
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

i++
try{
    const iter = func('и', 'мимимишка')
    const result = [1, 3, 5]
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

i++
try{
    const iter = func('', 'dsad')
    const result = []
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

i++
try{
    const iter = func('', '')
    const result = []
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

i++
try{
    const iter = func('abc', 'abc'.repeat(1000))
    const result = []
    for (let i = 0; i < 3000; i++){
        if ( i%3 === 0) {result.push(i)}
    }
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

i++
try{
    const iter = func('a', 'a'.repeat(1000000))
    const result = []
    for ( let i = 0; i < 1000000; i++){
        result.push(i)
    }
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

i++
try{
    const iter = func('AbC', 'xAbC yAbC zAbC')
    const result = [1, 6, 11]
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

i++
try{
    const iter = func('abcde', 'abxde abxcde abcdd')
    const result = []
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}

i++
try{
    const iter = func('aaaa', 'aaaabaaaa')
    const result = [0, 5]
    assert.deepEqual(iter, result, 'Проблема с функцией')
    finish++ 
} catch {
    console.log(`Ошибка в тесте ${i}`)
}


console.log(`Количество успешно пройденных тестов ${finish}`)