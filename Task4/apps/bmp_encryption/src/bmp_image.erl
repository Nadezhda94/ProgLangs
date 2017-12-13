-module(bmp_image).
-export([load/2,get_size/1,get_pixel/2,get_content/1, get_headers/1]).
-export([main/1]).
-define(BPP, 3).
-include("image_rec.hrl").
-compile(export_all).

% load(FileName) -> Image
% save(FileName, Image)
% get_size(Image) -> {W, H}
% get_pixel(Image, {X, Y}) -> {R, G, B}


% { {width,0}, {height,0}, {contents,<<>>}, {headers, <<>>} }
load(jpeg, _FileName) -> {error, not_implemented};
load(bmp, FileName) ->
    case file:read_file(FileName) of
        {ok, Contents} -> parse_contents(Contents);
        SomethingElse  -> SomethingElse
    end.
    
parse_contents(Contents) ->
    case Contents of
        <<"BM",_:64,Off:32/little,_:32,
            Width:32/signed-little,
            Height:32/signed-little,
            _Rest/binary>>
                        ->
                        Headers = binary_part(Contents,0,Off),
                        PixelDataSize = size(Contents)-Off,
                        %io:fwrite("Head size: ~p, cont.size: ~p~n", [size(Headers), PixelDataSize]),
                        PixelData = binary_part(Contents,Off,PixelDataSize),
                        Image = #image{
                            width = Width,
                            height = Height,
                            headers = Headers,
                            contents = PixelData
                        },
                        {ok, Image}
                        ;
                        
        _               -> {error, wrong_format}
    end.

get_size(Image) ->
    W = Image#image.width,
    H = Image#image.height,
    {W, H}.

get_content(Image) ->
    Image#image.contents.

get_headers(Image) ->
    Image#image.headers.
    
row_size(Image) ->
    {W, _} = get_size(Image),
    MeaningfullBytes = W * ?BPP,
    MeaningfullBytes + (MeaningfullBytes rem 4).

pixel_position(Image, {X,Y}) ->
    RowSize = row_size(Image),
    RowSize * Y + X * ?BPP.

get_pixel(Image, Point) ->
    Address = pixel_position(Image, Point),
    Contents = Image#image.contents,
    PixelData = binary_part(Contents, Address, ?BPP),
    <<B,G,R>> = PixelData,
    {R, G, B}.

main([FileName, X, Y | _ ]) ->    
    {ok, Image} = load(bmp, FileName),
    {W, H} = get_size(Image),
    io:fwrite("Read bitmap of size ~p x ~p~n", [W, H]),
    XValue = list_to_integer(X) - 1,
    YValue = list_to_integer(Y) - 1,
    Color = get_pixel(Image, {XValue,YValue}),
    io:format("Pixel at: ~p,~p: ~p~n", [XValue,YValue,Color]).
    
