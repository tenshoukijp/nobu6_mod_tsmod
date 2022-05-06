/*
顔画像のBMPへの差し替え系の判断系その①									
									
00502497   33C0             XOR EAX,EAX									
00502499   C3               RETN									
0050249A   8B0485 6839B100  MOV EAX,DWORD PTR DS:[EAX*4+B13968]									
005024A1   C3               RETN									
005024A2   05 D1070000      ADD EAX,7D1							←2001という数字を足している。		
005024A7   C3               RETN									
005024A8   05 B90B0000      ADD EAX,0BB9							←3001という数字を足している。		
005024AD   C3               RETN									
005024AE   CC               INT3									
005024AF   CC               INT3									
005024B0   8B4424 04        MOV EAX,DWORD PTR SS:[ESP+4]									
005024B4   83EC 08          SUB ESP,8									
005024B7   53               PUSH EBX									
005024B8   C70485 083BB000 >MOV DWORD PTR DS:[EAX*4+B03B08],0									
005024C3   C1E0 06          SHL EAX,6									
005024C6   8BD8             MOV EBX,EAX									
005024C8   69DB 003C0000    IMUL EBX,EBX,3C00									
005024CE   56               PUSH ESI									
005024CF   68 1CF35000      PUSH DDRAW.0050F31C                      ; ASCII "kao_24bit\000"									
									
									
									
顔画像のBMPへの差し替え系の判断系その②									
									
00502481  ^7C F5            JL SHORT DDRAW.00502478									
00502483   33C0             XOR EAX,EAX									
00502485   83E9 80          SUB ECX,-80									
00502488   3B31             CMP ESI,DWORD PTR DS:[ECX]									
0050248A   74 1C            JE SHORT DDRAW.005024A8									
0050248C   40               INC EAX									
0050248D   83C1 04          ADD ECX,4									
00502490   3D 00030000      CMP EAX,300							←768という姫の人数分と一致している。300に一致する箇所は沢山あるので、定数定義されていると考えられる。		
00502495  ^7C F1            JL SHORT DDRAW.00502488									
00502497   33C0             XOR EAX,EAX									
00502499   C3               RETN									
0050249A   8B0485 6839B100  MOV EAX,DWORD PTR DS:[EAX*4+B13968]									
									
									
									
姫768変更箇所、その①									
									
00502488   3B31             CMP ESI,DWORD PTR DS:[ECX]									
0050248A   74 1C            JE SHORT DDRAW.005024A8									
0050248C   40               INC EAX									
0050248D   83C1 04          ADD ECX,4									
00502490   3D 00030000      CMP EAX,300									
00502495  ^7C F1            JL SHORT DDRAW.00502488									
00502497   33C0             XOR EAX,EAX									
00502499   C3               RETN									
0050249A   8B0485 6839B100  MOV EAX,DWORD PTR DS:[EAX*4+B13968]									
005024A1   C3               RETN									
									
									
DDRAW.DLLにおいて									
1332番以降の顔番号をSDATA.exeで指定されている場合に対応する。									
									
00502200   8B4C24 04        MOV ECX,DWORD PTR SS:[ESP+4]									
00502204   8B4424 08        MOV EAX,DWORD PTR SS:[ESP+8]									
00502208   3B0D 3C3BB000    CMP ECX,DWORD PTR DS:[B03B3C]									
0050220E   75 10            JNZ SHORT DDRAW.00502220									
00502210   3D 34050000      CMP EAX,534								;534=1332 なので、この数字を1700などに書き換えると、顔番号1699まで使用可能となる。	
00502215   7D 09            JGE SHORT DDRAW.00502220									
00502217   8D50 01          LEA EDX,DWORD PTR DS:[EAX+1]									
0050221A   8915 8439B100    MOV DWORD PTR DS:[B13984],EDX									
00502220   894424 08        MOV DWORD PTR SS:[ESP+8],EAX									
00502224   894C24 04        MOV DWORD PTR SS:[ESP+4],ECX									
00502228  -FF25 7CE05000    JMP DWORD PTR DS:[<&KERNEL32.SetFilePoin>; kernel32.SetFilePointer									
									
									
									
									
									
									
このESIがどうやら顔番号だ。									
5BEC2519   81FE E8030000    CMP     ESI, 3E8									
5BEC251F   73 09            JNB     SHORT TSMod.5BEC252A									
5BEC2521   C605 6B424C5C 00 MOV     BYTE PTR DS:[5C4C426B], 0									
5BEC2528   EB 07            JMP     SHORT TSMod.5BEC2531									
5BEC252A   C605 6A424C5C 00 MOV     BYTE PTR DS:[5C4C426A], 0									
5BEC2531   6A 0A            PUSH    0A									
5BEC2533   6A 40            PUSH    40									
5BEC2535   68 E029F25B      PUSH    TSMod.5BF229E0                   ; ASCII "3001"									
5BEC253A   56               PUSH    ESI									
5BEC253B   E8 301F0000      CALL    TSMod.5BEC4470									
5BEC2540   68 E029F25B      PUSH    TSMod.5BF229E0                   ; ASCII "3001"									
5BEC2545   68 00100000      PUSH    1000									
5BEC254A   68 60424C5C      PUSH    TSMod.5C4C4260                   ; ASCII "BM8<"									
5BEC254F   E8 AE1F0000      CALL    TSMod.5BEC4502									
5BEC2554   68 2CF3EC5B      PUSH    TSMod.5BECF32C                   ; ASCII ".bmp"									
5BEC2559   68 00100000      PUSH    1000									
5BEC255E   68 60424C5C      PUSH    TSMod.5C4C4260                   ; ASCII "BM8<"									
5BEC2563   E8 9A1F0000      CALL    TSMod.5BEC4502									
									
									
5BEC281E   8B4C24 1C        MOV     ECX, DWORD PTR SS:[ESP+1C]									
5BEC2822   57               PUSH    EDI									
5BEC2823   E8 88FCFFFF      CALL    TSMod.5BEC24B0									
5BEC2828   83C4 04          ADD     ESP, 4									
5BEC282B   5F               POP     EDI									
5BEC282C   5E               POP     ESI									
									
									
									
									
									
									
									
これで使用する顔範囲のフィルターがかかっているか？									
5BEC2420   33C0             XOR     EAX, EAX									
5BEC2422   3B3485 A0374D5C  CMP     ESI, DWORD PTR DS:[EAX*4+5C4D37A>									
5BEC2429   75 12            JNZ     SHORT TSMod.5BEC243D									
5BEC242B   3B1485 48354C5C  CMP     EDX, DWORD PTR DS:[EAX*4+5C4C354>									
5BEC2432   75 09            JNZ     SHORT TSMod.5BEC243D									
5BEC2434   3B0C85 202DF25B  CMP     ECX, DWORD PTR DS:[EAX*4+5BF22D2>									
5BEC243B   74 14            JE      SHORT TSMod.5BEC2451									
5BEC243D   40               INC     EAX									
5BEC243E   83F8 05          CMP     EAX, 5									
5BEC2441  ^7C DF            JL      SHORT TSMod.5BEC2422									
5BEC2443   3B35 BC374D5C    CMP     ESI, DWORD PTR DS:[5C4D37BC]     ; n6pwav1.03070305									
5BEC2449   75 14            JNZ     SHORT TSMod.5BEC245F									
5BEC244B   A1 84394D5C      MOV     EAX, DWORD PTR DS:[5C4D3984]									
5BEC2450   C3               RETN									
5BEC2451   C70485 70374D5C >MOV     DWORD PTR DS:[EAX*4+5C4D3770], 0>									
5BEC245C   33C0             XOR     EAX, EAX									
5BEC245E   C3               RETN									
5BEC245F   33C0             XOR     EAX, EAX									
5BEC2461   3B3485 A0374D5C  CMP     ESI, DWORD PTR DS:[EAX*4+5C4D37A>									
5BEC2468   74 30            JE      SHORT TSMod.5BEC249A									
5BEC246A   40               INC     EAX									
5BEC246B   83F8 05          CMP     EAX, 5									
5BEC246E  ^7C F1            JL      SHORT TSMod.5BEC2461									
5BEC2470   8B0D 94394C5C    MOV     ECX, DWORD PTR DS:[5C4C3994]     ; TSMod.5C4D40F0									
5BEC2476   33C0             XOR     EAX, EAX									
5BEC2478   3B3481           CMP     ESI, DWORD PTR DS:[ECX+EAX*4]									
5BEC247B   74 25            JE      SHORT TSMod.5BEC24A2									
5BEC247D   40               INC     EAX									
5BEC247E   83F8 1F          CMP     EAX, 1F									
5BEC2481  ^7C F5            JL      SHORT TSMod.5BEC2478									
5BEC2483   33C0             XOR     EAX, EAX									
5BEC2485   83E9 80          SUB     ECX, -80									
5BEC2488   3B31             CMP     ESI, DWORD PTR DS:[ECX]									
5BEC248A   74 1C            JE      SHORT TSMod.5BEC24A8									
5BEC248C   40               INC     EAX									
5BEC248D   83C1 04          ADD     ECX, 4									
5BEC2490   3D 00030000      CMP     EAX, 300									
5BEC2495  ^7C F1            JL      SHORT TSMod.5BEC2488									
5BEC2497   33C0             XOR     EAX, EAX									
5BEC2499   C3               RETN									
5BEC249A   8B0485 68394D5C  MOV     EAX, DWORD PTR DS:[EAX*4+5C4D396>									
5BEC24A1   C3               RETN									
5BEC24A2   05 D1070000      ADD     EAX, 7D1									
5BEC24A7   C3               RETN									
5BEC24A8   05 B90B0000      ADD     EAX, 0BB9									
5BEC24AD   C3               RETN									
*/