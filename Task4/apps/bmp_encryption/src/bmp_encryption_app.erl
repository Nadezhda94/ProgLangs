-module(bmp_encryption_app).
-behaviour(application).

-export([start/2]).
-export([stop/1]).

start(_Type, _Args) ->
    Dispatch = cowboy_router:compile([
        {'_', [
                {"/", rest_bmp_encryption, []}
                ]
        }
    ]),
    {ok, _} = cowboy:start_clear(my_http_listener,
        [{port, 8080}],
        #{env => #{dispatch => Dispatch}}
    ),
    bmp_encryption_sup:start_link().
    

stop(_State) ->
    ok.
