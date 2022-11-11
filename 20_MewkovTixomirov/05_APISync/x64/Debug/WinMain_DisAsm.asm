;WinAPI

mov     dword ptr ss:[rsp+20],r9d
mov     qword ptr ss:[rsp+18],r8
mov     qword ptr ss:[rsp+10],rdx
mov     qword ptr ss:[rsp+8],rcx
push    rbp
push    rdi
sub     rsp,258
lea     rbp,qword ptr ss:[rsp+50]
lea     rdi,qword ptr ss:[rsp+50]
mov     ecx,52
mov     eax,CCCCCCCC
rep stosd dword ptr ds:[rdi],eax
mov     rcx,qword ptr ss:[rsp+278]
mov     rax,qword ptr ds:[<__security_cookie>]
xor     rax,rbp
mov     qword ptr ss:[rbp+1F8],rax
lea     rcx,qword ptr ds:[13F38509F]
call    <apisync.@ILT+930(__CheckForDebuggerJustMyCode)>
lea     r9,qword ptr ds:[13F37AD98]
xor     r8d,r8d
mov     edx,1
xor     ecx,ecx
call    qword ptr ds:[<&CreateEventW>]
mov     qword ptr ds:[<hQuit>],rax
call    qword ptr ds:[<&GetLastError>]
cmp     eax,B7
jne     apisync.13F371AAD
mov     eax,1
jmp     apisync.13F371E18
lea     rcx,qword ptr ss:[rbp+C0]
call    qword ptr ds:[<&GetStartupInfoW>]
mov     r8d,104
lea     rdx,qword ptr ds:[<szModuleName>]
xor     ecx,ecx
call    qword ptr ds:[<&GetModuleFileNameW>]
lea     rax,qword ptr ss:[rbp+88]
mov     qword ptr ss:[rsp+48],rax
lea     rax,qword ptr ss:[rbp+C0]
mov     qword ptr ss:[rsp+40],rax
mov     qword ptr ss:[rsp+38],0
mov     qword ptr ss:[rsp+30],0
mov     dword ptr ss:[rsp+28],4000000
mov     dword ptr ss:[rsp+20],0
xor     r9d,r9d
xor     r8d,r8d
lea     rdx,qword ptr ds:[<szModuleName>]
xor     ecx,ecx
call    qword ptr ds:[<&CreateProcessW>]
mov     edx,1388
mov     rcx,qword ptr ss:[rbp+88]
call    qword ptr ds:[<&WaitForSingleObject>]
mov     dword ptr ss:[rbp+24],eax
cmp     dword ptr ss:[rbp+24],0
jne     apisync.13F371B7D
lea     rdx,qword ptr ss:[rbp+44]
mov     rcx,qword ptr ss:[rbp+88]
call    qword ptr ds:[<&GetExitCodeProcess>]
mov     r8d,dword ptr ss:[rbp+44]
lea     rdx,qword ptr ds:[13F37ADC0]
lea     rcx,qword ptr ds:[<szMessage>]
call    qword ptr ds:[<&wsprintfW>]
xor     r9d,r9d
lea     r8,qword ptr ds:[13F37AE30]
lea     rdx,qword ptr ds:[<szMessage>]
xor     ecx,ecx
call    qword ptr ds:[<&MessageBoxW>]
jmp     apisync.13F371BB8
cmp     dword ptr ss:[rbp+24],102
jne     apisync.13F371BA1
xor     r9d,r9d
lea     r8,qword ptr ds:[13F37AE30]
lea     rdx,qword ptr ds:[13F37AE50]
xor     ecx,ecx
call    qword ptr ds:[<&MessageBoxW>]
jmp     apisync.13F371BB8
xor     r9d,r9d
xor     r8d,r8d
xor     edx,edx
xor     ecx,ecx
call    qword ptr ds:[<&CreateEventW>]
mov     qword ptr ds:[<hEmpty>],rax

xor     r9d,r9d
mov     r8d,A
mov     edx,A
xor     ecx,ecx
call    qword ptr ds:[<&CreateSemaphoreW>]
mov     qword ptr ds:[<hResource>],rax

lea     rcx,qword ptr ds:[<section>]
call    qword ptr ds:[<&RtlInitializeCriticalSection>]

;i = 0
mov     dword ptr ss:[rbp+4],0
jmp     apisync.13F371BF3
;i++
mov     eax,dword ptr ss:[rbp+4]
inc     eax
mov     dword ptr ss:[rbp+4],eax
;i < NUMBER_OF_THREADS
cmp     dword ptr ss:[rbp+4],A
jge     apisync.13F371C67

;i/3
mov     eax,dword ptr ss:[rbp+4]
cdq
mov     ecx,3
idiv    ecx
;check if remainder = 0
mov     eax,edx
test    eax,eax
je      apisync.13F371C16

mov     dword ptr ss:[rbp+1F4],0 ;???
jmp     apisync.13F371C20


mov     dword ptr ss:[rbp+1F4],4 ;???

movsxd  rax,dword ptr ss:[rbp+4] ;i
lea     rcx,qword ptr ds:[<dwCount>]
lea     rax,qword ptr ds:[rcx+rax*4]


;(i % 3) ? 0 : CREATE_SUSPENDED
lea     rcx,qword ptr ss:[rbp+64]
mov     qword ptr ss:[rsp+28],rcx
;&dwCount[i]
mov     ecx,dword ptr ss:[rbp+1F4] ;???
mov     dword ptr ss:[rsp+20],ecx
mov     r9,rax
;ThreadFunc
lea     r8,qword ptr ds:[<@ILT+910(?ThreadFunc@@YAKPEAX@Z)>] ;lbl_13F6C1830
xor     edx,edx
xor     ecx,ecx
call    qword ptr ds:[<&CreateThread>]

movsxd  rcx,dword ptr ss:[rbp+4] ;i
lea     rdx,qword ptr ds:[<hThread>]
mov     qword ptr ds:[rdx+rcx*8],rax
jmp     apisync.13F371BEB



mov     edx,1388 ;5000d
mov     rcx,qword ptr ds:[<hEmpty>]
call    qword ptr ds:[<&WaitForSingleObject>]
mov     rcx,qword ptr ds:[<hQuit>]
call    qword ptr ds:[<&PulseEvent>]
mov     r9d,1388 ;5000d
mov     r8d,1
lea     rdx,qword ptr ds:[<hThread>]
mov     ecx,A
call    qword ptr ds:[<&WaitForMultipleObjects>]
lea     rdx,qword ptr ds:[13F37AED0]
lea     rcx,qword ptr ds:[<szMessage>]
call    qword ptr ds:[<&lstrcpyW>]
mov     dword ptr ss:[rbp+4],0
jmp     apisync.13F371CC9

mov     eax,dword ptr ss:[rbp+4]
inc     eax
mov     dword ptr ss:[rbp+4],eax
cmp     dword ptr ss:[rbp+4],A
jge     apisync.13F371DA7

movsxd  rax,dword ptr ss:[rbp+4]
lea     rcx,qword ptr ds:[<hThread>]
lea     rdx,qword ptr ss:[rbp+44]
mov     rcx,qword ptr ds:[rcx+rax*8]
call    qword ptr ds:[<&GetExitCodeThread>]
cmp     dword ptr ss:[rbp+44],103
jne     apisync.13F371D60

movsxd  rax,dword ptr ss:[rbp+4]
lea     rcx,qword ptr ds:[<hThread>]
xor     edx,edx
mov     rcx,qword ptr ds:[rcx+rax*8]
call    qword ptr ds:[<&TerminateThread>]
mov     eax,dword ptr ss:[rbp+4]
cdq     
mov     ecx,3
idiv    ecx
mov     eax,edx
test    eax,eax
jne     apisync.13F371D37

mov     r8d,dword ptr ss:[rbp+4]
lea     rdx,qword ptr ds:[13F37AF20]
lea     rcx,qword ptr ds:[<szMessageCode>]
call    qword ptr ds:[<&wsprintfW>]
jmp     apisync.13F371D5E

movsxd  rax,dword ptr ss:[rbp+4]
lea     rcx,qword ptr ds:[<dwCount>]
mov     r9d,dword ptr ds:[rcx+rax*4]
mov     r8d,dword ptr ss:[rbp+4]
lea     rdx,qword ptr ds:[13F37AF50]
lea     rcx,qword ptr ds:[<szMessageCode>]
call    qword ptr ds:[<&wsprintfW>]
jmp     apisync.13F371D8E

movsxd  rax,dword ptr ss:[rbp+4]
lea     rcx,qword ptr ds:[<dwCount>]
mov     eax,dword ptr ds:[rcx+rax*4]
mov     dword ptr ss:[rsp+20],eax
mov     r9d,dword ptr ss:[rbp+44]
mov     r8d,dword ptr ss:[rbp+4]
lea     rdx,qword ptr ds:[13F37AFA8]
lea     rcx,qword ptr ds:[<szMessageCode>]
call    qword ptr ds:[<&wsprintfW>]
lea     rdx,qword ptr ds:[<szMessageCode>]
lea     rcx,qword ptr ds:[<szMessage>]
call    qword ptr ds:[<&lstrcatW>]
jmp     apisync.13F371CC1

mov     r8d,dword ptr ds:[<dwResources>]
lea     rdx,qword ptr ds:[13F37AFF0]
lea     rcx,qword ptr ds:[<szMessageCode>]
call    qword ptr ds:[<&wsprintfW>]
lea     rdx,qword ptr ds:[<szMessageCode>]
lea     rcx,qword ptr ds:[<szMessage>]
call    qword ptr ds:[<&lstrcatW>]
xor     r9d,r9d
lea     r8,qword ptr ds:[13F37B030]
lea     rdx,qword ptr ds:[<szMessage>]
xor     ecx,ecx
call    qword ptr ds:[<&MessageBoxW>]
lea     rcx,qword ptr ds:[<section>]
call    qword ptr ds:[<&RtlDeleteCriticalSection>]
mov     rcx,qword ptr ds:[<hResource>]
call    qword ptr ds:[<&CloseHandle>]
mov     rcx,qword ptr ds:[<hQuit>]
call    qword ptr ds:[<&CloseHandle>]
xor     eax,eax
mov     rdi,rax
lea     rcx,qword ptr ss:[rbp-50]
lea     rdx,qword ptr ds:[13F37ACD0]
call    <apisync.@ILT+820(_RTC_CheckStackVars)>
mov     rax,rdi
mov     rcx,qword ptr ss:[rbp+1F8]
xor     rcx,rbp
call    <apisync.@ILT+435(__security_check_cookie)>
lea     rsp,qword ptr ss:[rbp+208]

pop     rdi
pop     rbp
ret     


