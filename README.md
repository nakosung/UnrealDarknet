# UnrealDarknet

- [Darknet](http://pjreddie.com/darknet/) on UnrealEngine.

```js
let net = DarknetNetwork.parse_network_cfg(cfgfile)
net.load_weights(weightfile)
const input_size = net.get_network_input_size()
let input = DarknetMatrix.make(1,input_size)
...
```
