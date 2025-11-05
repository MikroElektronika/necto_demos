#############################################################################
## Function to get the board uid and MCU vendor used in the example
#############################################################################
function(define_vendor_and_board vendor board)
    if (${_MSDK_BOARD_NAME_} STREQUAL "BOARD_EK_RA6M5")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra6m5" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_EK_RA4M3")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra4m3" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_FPB_RA2E3")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "fpb_ra2e3" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "RA4M1_CLICKER")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ra4m1_clicker" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_EK_RA4M1")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra4m1" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_EK_RA6M3")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra6m3" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_EK_RA4L1")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra4l1" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_FPB_RA0E2")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "fpb_ra0e2" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_EK_RA2L2")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra2l2" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_FPB_RA2T1")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "fpb_ra2t1" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_EK_RA4C1")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra4c1" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_FPB_RA0L1")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "fpb_ra0l1" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_EK_RA8D1")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra8d1" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_FPB_RA8E1")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "fpb_ra8e1" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_EK_RA8E2")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra8e2" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_EK_RA8M1")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra8m1" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_MCB_RA8T1")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "mcb_ra8t1" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_FPB_RA0E1")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "fpb_ra0e1" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_EK_RA2A1")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra2a1" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_EK_RA2A2")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra2a2" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_FPB_RA2E1")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "fpb_ra2e1" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_FPB_RA2E2")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "fpb_ra2e2" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_EK_RA2E1")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra2e1" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_EK_RA2E2")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra2e2" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_EK_RA2L1")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra2l1" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_FPB_RA4E1")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "fpb_ra4e1" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_EK_RA4E2")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra4e2" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_EK_RA4M2")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra4m2" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_FPB_RA4T1")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "fpb_ra4t1" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_FPB_RA6E1")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "fpb_ra6e1" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_EK_RA6E2")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra6e2" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_EK_RA6M4")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra6m4" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_EK_RA6M1")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra6m1" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_EK_RA6M2")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra6m2" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_EK_RA4W1")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra4w1" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_MOTOR_RSSK_RA6T1")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "rssk_ra6t1" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_MCB_RA6T2")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "mcb_ra6t2" PARENT_SCOPE)
    elseif(${_MSDK_BOARD_NAME_} STREQUAL "BOARD_FPB_RA6T3")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "fpb_ra6t3" PARENT_SCOPE)
    else()
        set(${vendor} "unimplemented" PARENT_SCOPE)
        set(${board} "unimplemented" PARENT_SCOPE)
        message(INFO
        "
        This Demo for now only supports Bare-Metal setups with the following boards:
        - RA4M1 Clicker
        - Evaluation Kit for RA2A1 MCU Group
        - Evaluation Kit for RA2L2 MCU Group
        - Evaluation Kit for RA2A2 MCU Group
        - Evaluation Kit for RA2E1 MCU Group
        - Evaluation Kit for RA2E2 MCU Group
        - Evaluation Kit for RA2L1 MCU Group
        - Evaluation Kit for RA4C1 MCU Group
        - Evaluation Kit for RA4E2 MCU Group
        - Evaluation Kit for RA4L1 MCU Group
        - Evaluation Kit for RA4M1 MCU Group
        - Evaluation Kit for RA4M2 MCU Group
        - Evaluation Kit for RA4M3 MCU Group
        - Evaluation Kit for RA6E2 MCU Group
        - Evaluation Kit for RA6M1 MCU Group
        - Evaluation Kit for RA6M2 MCU Group
        - Evaluation Kit for RA6M3 MCU Group
        - Evaluation Kit for RA6M4 MCU Group
        - Evaluation Kit for RA6M5 MCU Group
        - Evaluation Kit for RA8D1 MCU Group
        - Evaluation Kit for RA8E2 MCU Group
        - Evaluation Kit for RA8M1 MCU Group
        - RA0E1 Fast Prototyping Board
        - Fast Prototyping Board for RA0L1
        - Fast Prototyping Board for RA0E2 MCU Group
        - Fast Prototyping Board for RA2E1 MCU Group
        - Fast Prototyping Board for RA2E2 MCU Group
        - Fast Prototyping Board for RA2E3 MCU Group
        - Fast Prototyping Board for RA2T1 MCU Group
        - Fast Prototyping Board for RA4E1 MCU Group
        - Fast Prototyping Board for RA4T1 MCU Group
        - Fast Prototyping Board for RA6E1 MCU Group
        - Fast Prototyping Board for RA6T3 MCU Group
        - Fast Prototyping Board for RA8E1 MCU Group
        - Motor Control Evaluation System for RA Family - RA6T1 Group
        - Renesas Flexible Motor Control CPU Board for RA6T2 MCU Group
        - Renesas Flexible Motor Control CPU Board for RA8T1 MCU Group

        Please select a setup for one of these boards.
        "
        )
    endif()
endfunction()
