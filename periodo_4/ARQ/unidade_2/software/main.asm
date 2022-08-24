.text
    init:
        bun main
        .align 5

    main:
        int 0
        mov r1, 0
        mov r2, 0
        addi ir, r0, 1
        bun 1
        mov r1, 0
        int 0
