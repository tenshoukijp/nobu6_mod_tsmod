/*
　┌この段階で、スタックにたまっている相手と自分の武将番号を両方とも自分の番号にする
00432A4C  /$ 55             PUSH    EBP
00432A4D  |. 8BEC           MOV     EBP, ESP
00432A4F  |. 83EC 5C        SUB     ESP, 5C



00432CB6  |. 2BD2           SUB     EDX, EDX
　┌ここでゼロ除算エラーがおきるので、なにもしないようにする。
00432CB8  |. F7F1           DIV     ECX


以上で、大砲を自分に打つことが可能。
*/
