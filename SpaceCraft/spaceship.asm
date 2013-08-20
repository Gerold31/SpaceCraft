SET PC, main

;-------------------------------------------

fetch_hardware:
    HWN I
    fetch_hardware_loop:
        IFE I, 0
            SET PC, fetch_hardware_end

        SUB I, 1

        HWQ I

        ;keyboard:
        SET Y, num_keyboard
        SET Z, dev_keyboard
        IFE A, [Z + 8]
            IFE B, [Z + 9]
                SET PC, load_dev

        ;monitor:
        SET Y, num_monitor
        SET Z, dev_monitor
        IFE A, [Z + 8]
            IFE B, [Z + 9]
                SET PC, load_dev

        ;doorcontrol:
        SET Y, num_doorcontrol
        SET Z, dev_doorcontrol
        IFE A, [Z + 8]
            IFE B, [Z + 9]
                SET PC, load_dev

        ;lightcontrol:
        SET Y, num_lightcontrol
        SET Z, dev_lightcontrol
        IFE A, [Z + 8]
            IFE B, [Z + 9]
                SET PC, load_dev

        ;lifedetection:
        SET Y, num_lifedetection
        SET Z, dev_lifedetection
        IFE A, [Z + 8]
            IFE B, [Z + 9]
                SET PC, load_dev

        fetch_hardware_continue:

    SET PC, fetch_hardware_loop

    load_dev:
        ADD Z, [Y]
        SET [Z], I
        ADD [Y], 1
    SET PC, fetch_hardware_continue

fetch_hardware_end:
SET PC, POP

;-------------------------------------------

init_all_monitor:
    SET X, [num_monitor]

    init_all_monitor_loop:
        IFE X, 0
            SET PC, init_all_monitor_end
        SUB X, 1
        SET A, X

        JSR init_monitor

    SET PC, init_all_monitor_loop
    init_all_monitor_end:
SET PC, POP

init_monitor:
    SET B, A
    SET C, A
    ADD C, dev_monitor
    MUL B, 384;32*12
    ADD B, image
    ADD A, image0
    SET [A], B

    ;set image
    SET A, 0
    HWI [C]

    SET A, B

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
    SET C, isr_ptr_keyboard0
        SET I, [num_keyboard]
	init_keyboard_loop:
		IFE I, 0
			SET PC, init_keyboard_loop_end
		SUB I, 1
                SET J, I
                SET B, [C]
                ADD C, 1
		ADD J, dev_keyboard
		HWI [J]
	SET PC, init_keyboard_loop

    init_keyboard_loop_end:
SET PC, POP

;--------------------------------------------

init_image:

    SET J, 0
    init_image_monitor_loop:
        IFE J, 4
            SET PC, init_image_monitor_loop_end

        SET I, 0
        init_image_loop:
            IFE I, 12
                SET PC, init_image_loop_end

            SET B, I
            SET C, J
            ; A = 32*I + 12*32*J = 32*(12*J + I)
            SET A, J
            MUL A, 12
            ADD A, I
            SHL A, 5 ;MUL A, 32
            ADD A, map_line_0_0

            JSR write_line

            ADD I, 1
        SET PC, init_image_loop
        init_image_loop_end:

        ADD J, 1
    SET PC, init_image_monitor_loop
    init_image_monitor_loop_end:

SET PC, POP

;--------------------------------------------

write_line: ; A: text, B: lineno C: monID

    SET PUSH, I
    SET PUSH, C
    SET PUSH, B
    SET PUSH, A

    ADD C, image0

    MUL B, 32
    ADD B, [C]

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

    SET A, POP
    SET B, POP
    SET C, POP
    SET I, POP

write_line_end:
SET PC, POP

;----------------------------------------------

set_char_font_color: ; A: row, B: colum, C: color (0x0-0xF), Z: monID

    SET PUSH, C

    SHL C, 8
    SET X, A
    MUL X, 32
    ADD X, B

    ADD Z, image0
    ADD X, [Z]

    AND [X], 0xF0FF
    ADD [X], C

    SET C, POP

SET PC, POP

;----------------------------------------------

set_char_bg_color: ; A: row, B: colum, C: color (0x0-0xF), Z: monID

    SET PUSH, C

    SHL C, 12
    SET X, A
    MUL X, 32
    ADD X, B

    ADD Z, image0
    ADD X, [Z]

    AND [X], 0x0FFF
    ADD [X], C

    SET C, POP

SET PC, POP
;----------------------------------------------

set_char: ; A: row, B: colum, C: char (0x00-0xFF), Z: monID

    SET PUSH, C

    SET X, A
    MUL X, 32
    ADD X, B

    ADD Z, image0
    ADD X, [Z]

    AND [X], 0xFF00
    ADD [X], C

    SET C, POP

SET PC, POP

;----------------------------------------------

isr:
    JSR A
RFI 0

;----------------------------------------------

isr_keyboard0:
    SET A, 0
    SET PC, isr_keyboard
isr_keyboard1:
    SET A, 1
    SET PC, isr_keyboard
isr_keyboard2:
    SET A, 2
    SET PC, isr_keyboard
isr_keyboard3:
    SET A, 3
    SET PC, isr_keyboard
isr_keyboard4:
    SET A, 4
    SET PC, isr_keyboard
isr_keyboard5:
    SET A, 5
    SET PC, isr_keyboard
isr_keyboard6:
    SET A, 6
    SET PC, isr_keyboard
isr_keyboard7:
    SET A, 7
    SET PC, isr_keyboard
isr_keyboard:

;    SET Z, A
;    SET A, 3
;    SET B, 0
;    SET C, 0x30
;    JSR set_char

SET PC, POP

;----------------------------------------------

main:
    JSR fetch_hardware

    ;init monitor
    JSR init_all_monitor

    JSR init_image

    ;init keyboard
    JSR init_keyboard

    IAS isr
    ;IAQ 1


    main_loop:

        WFI 0
 
    SET PC, main_loop
    main_loop_end:


main_end:

end: SET PC, end

;data:

num_monitor: DAT 0
dev_monitor: DAT 0, 0, 0, 0, 0, 0, 0, 0, 0xF615, 0x7349
num_keyboard: DAT 0
dev_keyboard: DAT 0, 0, 0, 0, 0, 0, 0, 0, 0x7406, 0x30CF
num_doorcontrol: DAT 0
dev_doorcontrol: DAT 0, 0, 0, 0, 0, 0, 0, 0, 0x2C71, 0xD001
num_lightcontrol: DAT 0
dev_lightcontrol: DAT 0, 0, 0, 0, 0, 0, 0, 0, 0x7C71, 0x1164
num_lifedetection: DAT 0
dev_lifedetection: DAT 0, 0, 0, 0, 0, 0, 0, 0, 0xDE7C, 0x11F3

isr_ptr_keyboard0: DAT isr_keyboard0
isr_ptr_keyboard1: DAT isr_keyboard1
isr_ptr_keyboard2: DAT isr_keyboard2
isr_ptr_keyboard3: DAT isr_keyboard3
isr_ptr_keyboard4: DAT isr_keyboard4
isr_ptr_keyboard5: DAT isr_keyboard5
isr_ptr_keyboard6: DAT isr_keyboard6
isr_ptr_keyboard7: DAT isr_keyboard7

max_num_lifedetector: DAT 8

lifedetector_0_x: DAT 0x0A
lifedetector_1_x: DAT 0x01
lifedetector_2_x: DAT 0x01
lifedetector_3_x: DAT 0x0A
lifedetector_4_x: DAT 0x01
lifedetector_5_x: DAT 0x0A
lifedetector_6_x: DAT 0x01
lifedetector_7_x: DAT 0x16

lifedetector_0_y: DAT 0x04
lifedetector_1_y: DAT 0x1C
lifedetector_2_y: DAT 0x16
lifedetector_3_y: DAT 0x0B
lifedetector_4_y: DAT 0x24
lifedetector_5_y: DAT 0x13
lifedetector_6_y: DAT 0x2B
lifedetector_7_y: DAT 0x2B

door_0_x: DAT 0x19
door_0_y: DAT 0x04
door_1_x: DAT 0x06
door_1_y: DAT 0x1C
door_2_x: DAT 0x10
door_2_y: DAT 0x05
door_3_x: DAT 0x10
door_3_y: DAT 0x1D
door_4_x: DAT 0x1E
door_4_y: DAT 0x06
door_5_x: DAT 0x0A
door_5_y: DAT 0x08
door_6_x: DAT 0x1E
door_6_y: DAT 0x09
door_7_x: DAT 0x1E
door_7_y: DAT 0x0F
door_8_x: DAT 0x0A
door_8_y: DAT 0x10
door_9_x: DAT 0x1E
door_9_y: DAT 0x12
door_A_x: DAT 0x10
door_A_y: DAT 0x13
door_B_x: DAT 0x10
door_B_y: DAT 0x2B
door_C_x: DAT 0x19
door_C_y: DAT 0x14
door_D_x: DAT 0x06
door_D_y: DAT 0x2C
door_E_x: DAT 0x08
door_E_y: DAT 0x0C


;             DAT "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
map_line_0_0:
;   "0123456789ABCDEF0123456789ABCDEF"
DAT "                        _A_     " ;00
DAT "     ___________       | X |    " ;01
DAT "    |           |      |   |    " ;02
DAT "    |    L0     |______| 0 |____" ;03
DAT "    |     /     2   L1   X      " ;04
DAT "    |           X___________  4 " ;05
DAT "    |           |           | X " ;06
DAT "    |___  5  ___|           |   " ;07
DAT "        | X |           ____| 6 " ;08
DAT "        |   |          |      X " ;09
DAT "    ____| L4|          |        " ;0A
DAT "   E   14  /|          |   L5   " ;0B
DAT "   X____X   |          |        " ;0C
DAT "        |   |          |        " ;0D
DAT "        |   |          |____  7 " ;0E
DAT "     ___| 8 |___            | X " ;0F
DAT "    |     X     |           |   " ;10
DAT "    |           |___________| 9 " ;11
DAT "    |     L7   10   L8        X " ;12
DAT "    |      /    X______ 12  ____" ;13
DAT "    |           |      | X |    " ;14
DAT "    |___________|      |   |    " ;15
DAT "                       |_C_|    " ;16
DAT "                         X      " ;17
DAT "     _B_                        " ;18
DAT "    | X |       ___________     " ;19
DAT "    |   |      |           |    " ;1A
DAT "____| 1 |______|     L3    |    " ;1B
DAT " /    X  L2    3      /    |    " ;1C
DAT " ______________X           |    " ;1D
DAT "|              |           |    " ;1E
DAT "|              |___________|    " ;1F
DAT "|_________                      " ;20
DAT "          |                     " ;21
DAT "          |                     " ;22
DAT "    L6    |                     " ;23
DAT " /        |                     " ;24
DAT "          |                     " ;25
DAT " _________|                     " ;26
DAT "|               ____________    " ;27
DAT "|              |            |   " ;28
DAT "|______________|            |   " ;29
DAT "         L9   11     L10    |   " ;2A
DAT "_/__ 13  ______X      /     |   " ;2B
DAT "    | X |      |            |   " ;2C
DAT "    |   |      |____________|   " ;2D
DAT "    |_D_|                       " ;2E
DAT "      X                         " ;2F

image0: DAT 0
image1: DAT 0
image2: DAT 0
image3: DAT 0
image4: DAT 0
image5: DAT 0
image6: DAT 0
image7: DAT 0

image:

