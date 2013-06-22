SET PC, main

;-------------------------------------------

fetch_hardware:
    HWN I
    fetch_hardware_loop:
        SUB I, 1
        HWQ I

        ;monitor:
        SET Z, dev_monitor
        IFE A, [Z + 1]
            IFE B, [Z + 2]
                SET PC, load_monitor

        fetch_hardware_continue:
        IFE I, 0
            SET PC, fetch_hardware_end

    SET PC, fetch_hardware_loop

    load_monitor:
        SET [dev_monitor], I
    SET PC, fetch_hardware_continue

fetch_hardware_end:
SET PC, POP

;-------------------------------------------

init_monitor:

    SET A, image

    SET I, 0
    init_monitor_outerloop:
        SET J, 0
        init_monitor_innerloop:
            IFE J, 32
                SET PC, init_monitor_innerloop_end
            SET [A], 0xF120
            ADD A, 1
            ADD J, 1
        SET PC, init_monitor_innerloop
        init_monitor_innerloop_end:
        IFE I, 12
            SET PC, init_monitor_outerloop_end
        ADD I, 1
    SET PC, init_monitor_outerloop
    init_monitor_outerloop_end:

init_monitor_end:
SET PC, POP
;--------------------------------------------

write_line:

    SET PUSH, I

    MUL B, 32
    ADD B, image

    SET I, 0
    write_line_loop:
        IFE I, 32
            SET PC, write_line_loop_end

        AND [B], 0xFF00
        ADD [B], [A]

        ADD A, 1
        ADD B, 1
        ADD I, 1

    SET PC, write_line_loop
    write_line_loop_end:

    SET I, POP
write_line_end:
SET PC, POP

;----------------------------------------------

set_pixel_color:

    SET PUSH, C

    SHL C, 8
    SET X, A
    MUL X, 32
    ADD X, B

    AND [image + X], 0xF0FF
    ADD [image + X], C

    SET C, POP

set_pexel_color_end:
SET PC, POP

;----------------------------------------------

main:
    JSR fetch_hardware

    ;set image buffer
    SET A, 0
    SET B, image
    HWI [dev_monitor]

    ;init monitor
    JSR init_monitor

    ;set headline
    SET A, headline
    SET B, 0
    JSR write_line


    SET C, 0

    main_loop:

        SET I, 1
        text_loop:
            IFE I, 12
                SET PC, text_loop_end
            SET X, I
            ADD X, C
            IFG X, 12
                SUB X, 12
            MUL X, 32
            ADD X, line00
            SET A, X
            SET B, I
            JSR write_line

        STI PC, text_loop
        text_loop_end:

        ADD C, 1
        IFE C, 12
            SET C, 0

    main_loop_end:
    SET PC, main_loop

main_end:

end: SET PC, end

;data:

dev_monitor: DAT 0, 0xF615, 0x7349

;         DAT "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
headline: DAT "Displaytest running, please wait"
line00:   DAT "x                       x       "
line01:   DAT " x                     x x      "
line02:   DAT "  x                   x   x     "
line03:   DAT "   x                 x     x    "
line04:   DAT "    x               x       x   "
line05:   DAT "     x             x         x  "
line06:   DAT "      x           x           x "
line07:   DAT "       x         x             x"
line08:   DAT "        x       x               "
line09:   DAT "         x     x                "
line10:   DAT "          x   x                 "
line11:   DAT "           x x                  "
line12:   DAT "            x                   "
line13:   DAT "!ERROR ERROR ERROR ERROR ERROR !"


image:
