"use strict"

module.exports = () => {
    let cfgfile = Context.GetDir('Game')+'/ThirdParty/darknet/cfg/yolo-tiny.cfg'
    let weightfile = Context.GetDir('GameContent')+'/Weights/yolo-tiny.weights'
    let net = DarknetNetwork.parse_network_cfg(cfgfile)
    //net.load_weights(weightfile)
    net.set_batch_network(1)
    $memaccess(net,'self',net_raw => {
        $memaccess(net,`layer_${net_raw.n-1}`,l => {
            let classes = l.classes
            let square = l.square
            let side = l.side
        })
    })
    console.log(net)
    return () => {}
}