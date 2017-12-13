-module(rest_bmp_encryption).

-compile(export_all).

init(Req, Opts) ->
    {cowboy_rest, Req, Opts}.

allowed_methods(Req, State) ->
    {[<<"GET">>, <<"PUT">>, <<"DELETE">>], Req, State}.

content_types_provided(Req, State) ->
    {[ {<<"application/octet-stream">>, get_file}], Req, State}.

content_types_accepted(Req, State) ->
    {[{<<"application/octet-stream">>, put_file}], Req, State}.

get_file(Req, State) -> 
    #{file := Filename, text := Text} = cowboy_req:match_qs([file, text], Req),
    {ok, Image} = bmp_image:load(bmp, Filename),
    NewImage = bmp_enctyption:modify_image(Image, Text),
    io:format("Processed file ~p ~n", [Filename]),
    {NewImage, Req, State}.

put_file(Req, State) -> 
    #{fileName := Filename} = cowboy_req:match_qs([fileName], Req),
    {ok, Data, Req1} = cowboy_req:read_body(Req, #{length => 100000000, period => 500000}),
    AllHeaders = cowboy_req:headers(Req),
    file:write_file(Filename, Data),
    io:format("Received file ~p ~n", [Filename]),
    {true, Req, State}.

delete_resource(Req, State) ->
    #{file := Filename} = cowboy_req:match_qs([{file, [], undefined}], Req),
    io:format("Delete file ~p~n", [Filename]),
    file:delete(Filename),
    {true, Req, State}.