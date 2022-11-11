;ThreadFunc
lbl_13F6C1830:

mov     qword ptr ss:[rsp+8],rcx
push    rbp
push    rdi
sub     rsp,158

lea     rbp,qword ptr ss:[rsp+20]
lea     rdi,qword ptr ss:[rsp+20]
mov     ecx,1E
mov     eax,CCCCCCCC
rep stosd dword ptr ds:[rdi],eax
mov     rcx,qword ptr ss:[rsp+178]
mov     rax,qword ptr ds:[<__security_cookie>]
xor     rax,rbp
mov     qword ptr ss:[rbp+128],rax
lea     rcx,qword ptr ds:[13FC0509F]
call    <apisync.@ILT+930(__CheckForDebuggerJustMyCode)>


mov     rax,qword ptr ss:[rbp+150]
mov     qword ptr ss:[rbp+28],rax
mov     rax,qword ptr ds:[<hQuit>]
mov     qword ptr ss:[rbp+48],rax
mov     rax,qword ptr ds:[<hResource>]
mov     qword ptr ss:[rbp+50],rax

mov     rax,qword ptr ss:[rbp+28]
mov     dword ptr ds:[rax],0


lbl_13F6C18A4:
mov     r9d,FFFFFFFF ;INFINITE
xor     r8d,r8d
lea     rdx,qword ptr ss:[rbp+48]
mov     ecx,2
call    qword ptr ds:[<&WaitForMultipleObjects>]
mov     dword ptr ss:[rbp+4],eax


;if (dwWait == WAIT_OBJECT_0)
cmp     dword ptr ss:[rbp+4],0
jne     apisync.13FBF18CF
jmp     apisync.13FBF1983 ;break


lbl_13F6C18CA:
jmp     apisync.13FBF197E ;= apisync.13FBF18A4


lbl_13F6C18CF:
mov     rax,qword ptr ss:[rbp+28]
mov     eax,dword ptr ds:[rax]
inc     eax
cmp     eax,5
jb      apisync.13FBF1932

lea     rcx,qword ptr ds:[<section>]
call    qword ptr ds:[<&RtlEnterCriticalSection>]
mov     rax,qword ptr ss:[rbp+28]
mov     eax,dword ptr ds:[rax]
inc     eax
xor     r8d,r8d
mov     edx,eax
mov     rcx,qword ptr ds:[<hResource>]
call    qword ptr ds:[<&ReleaseSemaphore>]

mov     rax,qword ptr ss:[rbp+28]
mov     eax,dword ptr ds:[rax]
mov     ecx,dword ptr ds:[<dwResources>]
add     ecx,eax
mov     eax,ecx
mov     dword ptr ds:[<dwResources>],eax
lea     rcx,qword ptr ds:[<section>]
call    qword ptr ds:[<&RtlLeaveCriticalSection>]
mov     rax,qword ptr ss:[rbp+28]
mov     dword ptr ds:[rax],0
jmp     apisync.13FBF197E ;= apisync.13FBF18A4

lbl_13F6C1932:
mov     rax,qword ptr ss:[rbp+28]
mov     eax,dword ptr ds:[rax]
inc     eax
mov     rcx,qword ptr ss:[rbp+28]
mov     dword ptr ds:[rcx],eax
lea     rcx,qword ptr ds:[<section>]
call    qword ptr ds:[<&RtlEnterCriticalSection>]
mov     eax,dword ptr ds:[<dwResources>]
dec     eax
mov     dword ptr ds:[<dwResources>],eax
cmp     dword ptr ds:[<dwResources>],0
jne     apisync.13FBF1971

mov     rcx,qword ptr ds:[<hEmpty>]
call    qword ptr ds:[<&SetEvent>]

lbl_13F6C1971:
lea     rcx,qword ptr ds:[<section>]
call    qword ptr ds:[<&RtlLeaveCriticalSection>]

lbl_13F6C197E:
jmp     apisync.13FBF18A4


lbl_13F6C1983:
mov     rax,qword ptr ss:[rbp+28]
mov     eax,dword ptr ds:[rax]
mov     edi,eax
lea     rcx,qword ptr ss:[rbp-20]
lea     rdx,qword ptr ds:[13FBFAD70]


call    <apisync.@ILT+820(_RTC_CheckStackVars)>
mov     eax,edi
mov     rcx,qword ptr ss:[rbp+128]
xor     rcx,rbp
call    <apisync.@ILT+435(__security_check_cookie)>
lea     rsp,qword ptr ss:[rbp+138]

pop     rdi
pop     rbp
ret     





