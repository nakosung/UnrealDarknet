/// <reference path="typings/ue.d.ts">/>

"use strict"

let _ = require('lodash')

const voc_names = ["aeroplane", "bicycle", "bird", "boat", "bottle", "bus", "car", "cat", "chair", "cow", "diningtable", "dog", "horse", "motorbike", "person", "pottedplant", "sheep", "sofa", "train", "tvmonitor"]

function convert_yolo_detections(predictions, classes, num, square, side, w, h, thresh, probs, boxes, only_objectness) {
    let i,j,n;
    //int per_cell = 5*num+classes;
    for (i = 0; i < side*side; ++i){
        let row = i / side;
        let col = i % side;
        for (n = 0; n < num; ++n){
            let index = i*num + n;
            let p_index = side*side*classes + i*num + n;
            let scale = predictions[p_index];
            let box_index = side*side*(classes + num) + (i*num + n)*4;
            boxes[index].x = (predictions[box_index + 0] + col) / side * w;
            boxes[index].y = (predictions[box_index + 1] + row) / side * h;
            boxes[index].w = Math.pow(predictions[box_index + 2], (square?2:1)) * w;
            boxes[index].h = Math.pow(predictions[box_index + 3], (square?2:1)) * h;
            for(j = 0; j < classes; ++j){
                let class_index = i*classes;
                let prob = scale*predictions[class_index+j];
                probs[index][j] = (prob > thresh) ? prob : 0;
            }
            if(only_objectness){
                probs[index][0] = scale;
            }
        }
    }
}

function overlap(x1, w1, x2, w2) {
    var l1 = x1 - w1/2;
    var l2 = x2 - w2/2;
    var left = l1 > l2 ? l1 : l2;
    var r1 = x1 + w1/2;
    var r2 = x2 + w2/2;
    var right = r1 < r2 ? r1 : r2;
    return right - left;
}

function box_intersection(a, b) {
    var w = overlap(a.x, a.w, b.x, b.w);
    var h = overlap(a.y, a.h, b.y, b.h);
    if(w < 0 || h < 0) return 0;
    var area = w*h;
    return area;
}

function box_union(a, b) {
    var i = box_intersection(a, b);
    var u = a.w*a.h + b.w*b.h - i;
    return u;
}

function box_iou(a, b) {
    return box_intersection(a, b)/box_union(a, b);
}


function do_nms_sort(boxes, probs, total, classes, thresh) {
    let i, j, k;
    let s = _.range(total).map(i => ({index:i,class:0,probs:probs}))

    for(k = 0; k < classes; ++k){
        for(i = 0; i < total; ++i){
            s[i].class = k;
        }
        s.sort((a,b) => {
            let diff = a.probs[a.index][b.class] - b.probs[b.index][b.class]
            if (diff < 0) return 1
            else if (diff > 0) return -1
            return 0
        })        
        for(i = 0; i < total; ++i){
            if(probs[s[i].index][k] == 0) continue;
            let a = boxes[s[i].index];
            for(j = i+1; j < total; ++j){
                let b = boxes[s[j].index];
                if (box_iou(a, b) > thresh){
                    probs[s[j].index][k] = 0;
                }
            }
        }
    }
}

function print_yolo_detections(out, boxes, probs, total, classes, w, h) {
    var i, j;
    for(i = 0; i < total; ++i){
        var xmin = boxes[i].x - boxes[i].w/2.;
        var xmax = boxes[i].x + boxes[i].w/2.;
        var ymin = boxes[i].y - boxes[i].h/2.;
        var ymax = boxes[i].y + boxes[i].h/2.;

        if (xmin < 0) xmin = 0;
        if (ymin < 0) ymin = 0;
        if (xmax > w) xmax = w;
        if (ymax > h) ymax = h;

        for(j = 0; j < classes; ++j){
            if (probs[i][j]) out(voc_names[j], probs[i][j], xmin, ymin, xmax, ymax);
        }
    }
}


module.exports = () => {
    let alive = true
    
    let brushAsset = new SlateBrushAsset()
    brushAsset.Brush.ResourceObject = TextureRenderTarget2D.Load('/Game/Camera.Camera')
    brushAsset.Brush.ImageSize = {X:512,Y:512}
    
    let whiteBrushAsset = new SlateBrushAsset()
    
    let annotations = []
    // create a widget
    class TestWidget2 extends JavascriptWidget {
        OnPaint(context) {
            PaintContext.C(context).DrawBox({X:0,Y:0},{X:512,Y:512},brushAsset,{R:1,G:1,B:1,A:1})
                         
        }
    }
    
    let TestWidget2_C = require('uclass')()(global,TestWidget2)
    class TestWidget extends JavascriptWidget {
        OnPaint(context) {
            let start = {X:0,Y:0}
            let r = 512
            let t = (new Date() | 0) / 1000
            let end = {X:Math.cos(t)*r,Y:Math.sin(t)*r}
            // PaintContext.C(context).DrawBox({X:0,Y:0},{X:512,Y:512},brushAsset,{R:1,G:1,B:1,A:1})
            PaintContext.C(context).DrawLine(start,end,{R:1,A:1},true)
            
            annotations.forEach(a => {
                PaintContext.C(context).DrawBox(
                    {X:a.xmin * 512,Y:a.ymin * 512},
                    {X:(a.xmax - a.xmin) * 512,Y:(a.ymax - a.ymin) * 512},
                    whiteBrushAsset,
                    {R:1,G:1,B:1,A:a.prob})
                PaintContext.C(context).DrawText(
                    `${a.id} ${a.prob}`,
                    {X:a.xmin * 512,Y:a.ymin * 512},
                    GEngine.TinyFont,
                    16,
                    'Regular',
                    {R:1,G:1,B:1,A:1}
                )
            })           
        }
    }
    
    let TestWidget_C = require('uclass')()(global,TestWidget)
    let instantiator = require('instantiator')
    let UMG = require('UMG')
    
    let design = UMG.div({'slot.size.size-rule':'Fill'},
        UMG.text({},"YOLO / darknet / Unreal.js / UnrealEngine"),
        UMG.span({'slot.size.size-rule':'Fill'},
            UMG(SizeBox,{
                'slot.size.size-rule':'Fill',
                WidthOverride:512,
                HeightOverride:512},
                UMG(TestWidget_C,{})
            ),
            UMG(TestWidget2_C,{'slot.size.size-rule':'Fill'})
        )
    )
    let page = instantiator(design)    
    let widget = GWorld.CreateWidget(JavascriptWidget)
    widget.JavascriptContext = Context
    widget.SetRootWidget(page)
    widget.AddToViewport()
    
    // if (0)
    process.nextTick(__ => {
        let cfgfile = Context.GetDir('Game')+'/ThirdParty/darknet/cfg/yolo-tiny.cfg'
        let weightfile = Context.GetDir('GameContent')+'/Weights/yolo-tiny.weights'
        let net = DarknetNetwork.parse_network_cfg(cfgfile)
        net.load_weights(weightfile)
        net.set_batch_network(1)
        $memaccess(net,'self',net_raw => {
            $memaccess(net,`layer_${net_raw.n-1}`,l => {
                let cam = _.find(GWorld.GetAllActorsOfClass(CameraActor).OutActors,actor => actor.GetDisplayName() == 'CAPTURE')
                let target = cam.SceneCaptureComponent2D.TextureTarget
                
                const thresh = 0.05
                let nms = 0.5
                let iou_thresh = 0.5
                
                function tick() {
                    if (!alive) return
                    
                    let image = DarknetImage.load_image(0, target)
                    let sized = image.resize_image(net_raw.w, net_raw.h)
                    let prediction = net.network_predict_image(sized)
                    let boxes = _.range(l.side * l.side * l.n).map(__ => ({}))
                    let probs = _.range(l.side * l.side * l.n).map(__ => _.range(l.classes).map(_ => 0))
                    $memaccess(prediction,ab => {
                        let data = new Float32Array(ab[0])                        
                        convert_yolo_detections(data, l.classes, l.n, l.square, l.side, 1, 1, thresh, probs, boxes, 0)
                        if (nms) do_nms_sort(boxes, probs, l.side*l.side*l.n, l.classes, nms);
                        annotations.length = 0
                        function dump(id,prob,xmin,ymin,xmax,ymax) {
                            annotations.push({id:id,prob:prob,xmin:xmin,ymin:ymin,xmax:xmax,ymax:ymax})
                        }
                        print_yolo_detections(dump,boxes, probs, l.side*l.side*l.n, l.classes, 1, 1);
                    })
                    process.nextTick(tick)                    
                }
                tick()
            })
        })   
    })    
    
    return () => {
        widget.RemoveFromViewport()
        alive = false
    }
}