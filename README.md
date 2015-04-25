# StreamIngester
multicast stream recorder written in qt  
it is normally used to capture multicast transport streams (mpeg ts) to files

## building
    qmake .
    make

## usage
    streamingester [rpc_port=1338] [media_dir=/media] [interface_name - omit for any]
    streamingester 1338 /media/recordings eth0
    streamingester 1338 /media/recordings

## json rpc interface

**method** : addChannel  
**params** : [channelUid, streamType [ unused - deduced from packet size], address, port]
