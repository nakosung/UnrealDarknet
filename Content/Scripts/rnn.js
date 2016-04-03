"use strict"

let seed_str = "abcd"
let cfgfile = Context.GetDir('Game')+'/ThirdParty/darknet/cfg/rnn.cfg'
let weightfile = Context.GetDir('GameContent')+'/Weights/grrm.weights'
let net = DarknetNetwork.parse_network_cfg(cfgfile)
net.load_weights(weightfile)
const input_size = net.get_network_input_size()
console.log('network input size:',input_size)
let input = DarknetMatrix.make_matrix(1,input_size)

let _ = require('lodash')
const num = 100

$memaccess(net,raw => {
    _.range(raw.n).forEach(layer => {
        $memaccess(net,`layer_${layer}`,raw => {
            raw.temperature = 0.7
        })
    })        
})

let seed = seed_str.split('').map(c => c.charCodeAt(0))
function mark(c,fn) {
    $memaccess(input,(mat) => {
        let fa = new Float32Array(mat[0])
        fa[c] = 1
        fn(input)
        fa[c] = 0
    })
}    
while (seed.length > 1) {
    let c = seed.shift()
    mark(c,_ => net.network_predict(input))
}
let c = seed[0]    
let msg = []
_.range(num).forEach(i => {
    let r = Math.random()
    let sum = 0
    msg.push(c)
    mark(c,input => {
        let out = net.network_predict(input)            
        $memaccess(out,(mat) => {
            let fa2 = new Float32Array(mat[0])
            for (let j=0; j<input_size; ++j) 
            {
                sum += fa2[j]
                if (sum>r) {
                    c = j;
                    break;
                }
            }    
        })
    })        
})

let str = msg.map(x => String.fromCharCode(x)).join('')
console.log('generated:',str)