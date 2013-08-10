SET PC, main

;-------------------------------------------

fetch_hardware:
    HWN I
    fetch_hardware_loop:
        SUB I, 1
        HWQ I

        ;keyboard:
        SET Z, dev_keyboard
        IFE A, [Z + 1]
            IFE B, [Z + 2]
                SET PC, load_dev

        ;monitor:
        SET Z, dev_monitor
        IFE A, [Z + 1]
            IFE B, [Z + 2]
                SET PC, load_dev

        ;lightcontrol
        SET Z, dev_lightcontrol
        IFE A, [Z + 1]
            IFE B, [Z + 2]
                SET PC, load_dev

        ;lifedetection
        SET Z, dev_lifedetection
        IFE A, [Z + 1]
            IFE B, [Z + 2]
                SET PC, load_dev

        fetch_hardware_continue:
        IFE I, 0
            SET PC, fetch_hardware_end

    SET PC, fetch_hardware_loop

    load_dev:
        SET [Z], I
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

init_keyboard:

    SET A, 3
    SET B, 1
    HWI [dev_keyboard]

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

isr:

    SET A, 1
    HWI [dev_lifedetection]

    ;lifedist is now in B

    SET A, 2
    HWI [dev_lightcontrol] ;set brightness of all lights to B

RFI 0

;----------------------------------------------

main:
    JSR fetch_hardware

    IAS isr
    ;IAQ 1

    SET A, 0
    SET B, 1
    HWI [dev_lifedetection]


    main_loop:

        WFI 0
 
    SET PC, main_loop
    main_loop_end:


main_end:

end: SET PC, end

;data:

dev_monitor: DAT 0, 0xF615, 0x7349
dev_keyboard: DAT 0, 0x7406, 0x30CF
dev_lightcontrol: DAT 0, 0x7C71, 0x1164
dev_lifedetection: DAT 0, 0xDE7C, 0x11F3


;         DAT "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
headline: DAT "  Keyboard test, please type!   "
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
