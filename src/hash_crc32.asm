section .text
global AsmGetHashCRC32
AsmGetHashCRC32:
    xor rax, rax
    xor r10, r10
    
    .loop:
        crc32 eax, byte [rdi + r10]     
        inc r10
        cmp byte [rdi + r10], 0x00       
        jne .loop
    ret