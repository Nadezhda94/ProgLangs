-module(bmp_enctyption).
-export([main/1, modify_image/2]).

get_offset_size(Rest, Result) ->
	BitsCount = (byte_size(Rest) - 1) *8,
	<<PreviousPart:BitsCount/bitstring, FirstBit:1/integer, SecondBit:1/integer, _:6/bitstring>> = Rest,
	if 
        FirstBit == 1 andalso SecondBit /= 1 -> 
         	get_offset_size(PreviousPart, Result + 1);
        true ->
         	Result + 1
    end.

calc_size_to_write(CharBytes, {W, H}) ->
	BytesCount = byte_size(CharBytes),
	BytesForUse = (W * H - 4),
	if
        BytesCount > BytesForUse ->
        	BitsForUse = BytesForUse * 8,
            <<UsedTextPart:BitsForUse/bitstring, Rest/bitstring>> = CharBytes,
             <<FirstBit:1/integer, SecondBit:1/integer, _/bitstring>> = Rest,
             if 
             	FirstBit == 1 andalso SecondBit /= 1 -> 
             		BytesForUse - get_offset_size(UsedTextPart, 0);
             	true ->
             		BytesForUse
             end;
        true -> 
            BytesCount
    end.

inner_modify(CharBytes, ReadSize, Contents, BytesCount) ->
	RestSize = byte_size(Contents) - 3,
	%io:fwrite("ReadSize: ~p, BytesCount:~p~n", [ReadSize, BytesCount]),
	TextByte = binary_part(CharBytes, ReadSize, 1),
	<<T1:2/bitstring, T2:3/bitstring, T3/bitstring>> = TextByte,
	PixelData = binary_part(Contents, 0, 3),
	<<B:6/bitstring, B_:2/bitstring, G:5/bitstring, G_:3/bitstring, R:5/bitstring, R_:3/bitstring>> = PixelData,
	NextContent = binary_part(Contents, 3, RestSize),
	if 
		( ReadSize + 1 )< BytesCount ->
			Rest = inner_modify(CharBytes, ReadSize + 1,  NextContent, BytesCount);
		true ->
			Rest = NextContent
	end,
	<<B/bitstring, T1/bitstring, G/bitstring, T2/bitstring, R/bitstring, T3/bitstring, Rest/bitstring>>.


do_modify(CharBytes, BytesCount, Contents)->
	TextSize = <<BytesCount:32>>,
	CharBytesWithSize = <<TextSize/bitstring, CharBytes/bitstring>>,
	%ContentToModify = binary_part(Contents, 0, BytesCount),
	%Rest = binary_part(Contents, 0, BytesCount),
	Modified = inner_modify(CharBytesWithSize, 0, Contents, BytesCount + 4),
	Modified.

modify_image(Image, Text) ->
	CharBytes = unicode:characters_to_binary(Text),
	{W, H} = bmp_image:get_size(Image),
	BytesCount = calc_size_to_write(CharBytes, {W, H}),
	Contents = bmp_image:get_content(Image),
	Result = do_modify(CharBytes, BytesCount, Contents),
	Headers = bmp_image:get_headers(Image),
	NewImage = <<Headers/bitstring, Result/bitstring>>,
	NewImage.

get_value(PixelData) ->
	<<B:6/bitstring, B_:2/bitstring, G:5/bitstring, G_:3/bitstring, R:5/bitstring, R_:3/bitstring>> = PixelData,
	<<B_/bitstring, G_/bitstring, R_/bitstring>>.

inner_get(BytesCount, ReadSize, Contents) ->
	RestSize = byte_size(Contents) - 3,
	PixelData = binary_part(Contents, 0, 3),
	<<B:6/bitstring, B_:2/bitstring, G:5/bitstring, G_:3/bitstring, R:5/bitstring, R_:3/bitstring>> = PixelData,
	NextContent = binary_part(Contents, 3, RestSize),
	if 
		( ReadSize + 1 )< BytesCount ->
			Rest = inner_get(BytesCount, ReadSize + 1,  NextContent);
		true ->
			Rest = <<>>
	end,
	<<B_/bitstring, G_/bitstring, R_/bitstring, Rest/bitstring>>.

get_text_size(Contents) ->
	<<B1:24/bitstring, B2:24/bitstring, B3:24/bitstring, B4:24/bitstring>> = binary_part(Contents, 0, 12),
	<<BytesCount:32/integer>> = <<(get_value(B1))/bitstring, (get_value(B2))/bitstring, (get_value(B3))/bitstring, (get_value(B4))/bitstring>>,
	BytesCount.


decode_image(Image) ->
	Contents = bmp_image:get_content(Image),
	BytesCount = get_text_size(Contents),
	inner_get(BytesCount, 0, binary_part(Contents, 12, (byte_size(Contents) - 12))).

main([FileName, Text|_]) ->
    {ok, Image} = bmp_image:load(bmp, FileName),
    NewImage = modify_image(Image, Text),
    file:write_file("new_image.bmp", NewImage),
    {ok, ModifiedImage} = bmp_image:load(bmp, "new_image.bmp"),
    io:fwrite("~ts~n", [decode_image(ModifiedImage)]).