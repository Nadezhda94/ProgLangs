#### Install ####
```
rebar get-deps
rebar compile
cd rel
rebar generate
```
#### Start server ####
```
sh bmp_encryption/bin/bmp_encryption console
```

#### Query examples ####
```
curl -v -H "Content-Type: application/octet-stream" -T <source_file> localhost:8080?fileName=<file_name>

curl -X GET -v  localhost:8080?file=<file_name>&text=<text> > <file_name>

curl -X DELETE -v localhost:8080?file=<file_name>

```

  
