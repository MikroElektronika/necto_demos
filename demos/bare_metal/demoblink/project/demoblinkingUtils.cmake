#############################################################################
## Function to get the board uid and MCU vendor used in the example
#############################################################################
function(define_vendor_and_board vendor board)
    if (${MCU_NAME} MATCHES "^R7FA6M5.+")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra6m5" PARENT_SCOPE)
    elseif(${MCU_NAME} MATCHES "^R7FA4M3.+")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ek_ra4m3" PARENT_SCOPE)
    elseif(${MCU_NAME} MATCHES "^R7FA2E3.+")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "fpb_ra2e3" PARENT_SCOPE)
    elseif(${MCU_NAME} MATCHES "^R7FA4M1.+")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ra4m1_clicker" PARENT_SCOPE)
    else()
        set(${vendor} "unimplemented" PARENT_SCOPE)
        set(${board} "unimplemented" PARENT_SCOPE)
        message(INFO
        "
        This Demo for now only supports setup with the following boards:
        - RA4M1 Clicker
        - RA2E3 Fast Prototyping Board
        - RA4M3 Evaluation Kit
        - RA6M5 Evaluation Kit

        Please select a setup for one of these boards.
        "
        )
    endif()
endfunction()
