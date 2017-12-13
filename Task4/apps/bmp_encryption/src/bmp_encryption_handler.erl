-module(bmp_encryption_handler).

-compile(export_all).

init( Req, State) ->
	Method = cowboy_req:method(Req),
	router(Method, Req, State).


router1(Req, State) -> 
  case cowboy_req:method(Req) of
    <<"POST">> -> 
        {<<"{\"status\": \"POST\"}">>, Req, State};
    <<"GET">> -> 
      {<<"{\"status\": \"GET\"}">>, Req, State};
    <<"PUT">> -> 
      {ok, Data, Req1} = cowboy_req:read_body(Req, #{length => 100000000, period => 500000}),
      io:format("Received as follow:~n~p~n~n",
        [ Req1]),
      Req2 = cowboy_req:reply(201, #{
    <<"content-type">> => <<"text/plain">>
}, "Hello world!", Req),
      {{true, "/foo/bar"}, Req2, State};
    <<"DELETE">> -> 
      {<<"{\"status\": \"DELETE\"}">>, Req, State}
  end.


router(<<"GET">>, Req, State) ->

	 #{file := Filename, text := Text} = cowboy_req:match_qs([file, text], Req),
	 {ok, Image} = bmp_image:load(bmp, Filename),
	 io:format("Received file ~p ~n",
		[Text]),
     NewImage = bmp_enctyption:modify_image(Image, Text),
	io:format("Received file ~p ~n",
		[NewImage]),

	Req1 = cowboy_req:set_resp_header(<<"content-disposition">>, "attachment; filename=result.bmp", Req),
			Req2 = cowboy_req:set_resp_body(NewImage, Req1),
			Req3 = cowboy_req:reply(200, #{
				<<"content-type">> => <<"application/octet-stream">>
			}, Req2),
			{ok, Req3, State};

router(<<"PUT">>, Req, State) ->
	{ok, Headers, Req2} = cowboy_req:read_part(Req),
	{ok, Data, Req3} = cowboy_req:read_part_body(Req2),
	{file, <<"inputfile">>, Filename, ContentType}
		= cow_multipart:form_data(Headers),
	io:format("Received file ~p of content-type ~p as follow:~n~p~n~n",
		[Filename, ContentType, Data]),
	file:write_file(Filename, Data),
	{ok, Req3, State};

%application:ensure_all_started(bmp_encryption).
router(<<"DELETE">>, Req, State) ->
	#{file := Filename} = cowboy_req:match_qs([{file, [], undefined}], Req),
	%{Filename, Req2} = cowboy_req:qs_val(<<"file">>, Req),
	io:format("Delete file ~p~n",
		[Filename]),
	file:delete(Filename),
	{ok, Req, State}.

echo(<<"GET">>, undefined, Req) ->
	cowboy_req:reply(400, [], <<"Missing echo parameter.">>, Req);
echo(<<"GET">>, Echo, Req) ->
	cowboy_req:reply(200, [
		{<<"content-type">>, <<"text/plain; charset=utf-8">>}
	], Echo, Req);
echo(_, _, Req) ->
	%% Method not allowed.
	cowboy_req:reply(405, Req).

terminate(_Reason, _Req, _State) ->
	ok.