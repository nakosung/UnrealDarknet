/// <reference path="typings/ue.d.ts">/>

"use strict"

if (!global.$boot) {
    global.$boot = true
    Context.RunFile('aliases.js')
    Context.RunFile('polyfill/unrealengine.js')
    Context.RunFile('polyfill/timers.js')
    return require('devrequire')('darknet')
}

module.exports = function() {    
    require('./rnn')    
    return _ => {}    
}

