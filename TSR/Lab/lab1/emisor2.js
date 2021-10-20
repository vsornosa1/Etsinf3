const ev = require('events')

const emitter = new ev.EventEmitter()
const e1='e1', e2='e2'

function handler (event,n) {
    return (incr)=>{ // listener with param
        n+=incr
        console.log(event + ':' + n)
    }
}

emitter.on(e1, handler(e1,0))
emitter.on(e2, handler(e2,'')) // implicit type casting

console.log('\n\n--------------------------- init\n\n')
for (let i=1; i<4; i++) emitter.emit(e1,i) // sequence, iteration, generation with param
console.log('\n\n--------------------------- intermediate\n\n')
for (let i=1; i<4; i++) emitter.emit(e2,i) // sequence, iteration, generation with param
console.log('\n\n--------------------------- end')
