xcopy /s /y /exclude:exclude_list.txt ".\Project\Engine\*.h" ".\External\Include\Engine\"
xcopy /s /y /exclude:exclude_list.txt ".\Project\Content\*.h" ".\External\Include\Content\"
xcopy /s /y /exclude:exclude_list.txt ".\Project\Engine\*.hlsl" ".\OutPutFile\content\shader\"
xcopy /s /y /exclude:exclude_list.txt ".\Project\Engine\*.hlsli" ".\OutPutFile\content\shader\"