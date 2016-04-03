/// <reference path="typings/ue.d.ts">/>

"use strict"

module.exports = () => {
    let alive = true
    process.nextTick(__ => {
        let cfgfile = Context.GetDir('Game')+'/ThirdParty/darknet/cfg/yolo-tiny.cfg'
        let weightfile = Context.GetDir('GameContent')+'/Weights/yolo-tiny.weights'
        let net = DarknetNetwork.parse_network_cfg(cfgfile)
        net.load_weights(weightfile)
        net.set_batch_network(1)
        $memaccess(net,'self',net_raw => {
            $memaccess(net,`layer_${net_raw.n-1}`,l => {
                let classes = l.classes
                let square = l.square
                let side = l.side
                
                let _ = require('lodash')
                let cam = _.find(GWorld.GetAllActorsOfClass(CameraActor).OutActors,actor => actor.GetDisplayName() == 'CAPTURE')
                let target = cam.SceneCaptureComponent2D.TextureTarget
                
                function tick() {
                    if (!alive) return
                    
                    let image = DarknetImage.load_image(0, target)
                    let sized = image.resize_image(net_raw.w, net_raw.h)
                    let prediction = net.network_predict_image(sized)
                    $memaccess(prediction,ab => {
                        let data = new Float32Array(ab[0])
                        console.log(data,'prediction')
                    })
                    process.nextTick(tick)                    
                }
                tick()
            })
        })   
    })    
    
    return () => {
        alive = false
    }
}