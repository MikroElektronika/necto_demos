#############################################################################
## Function to get the board uid and MCU vendor used in the example
#############################################################################
function(define_vendor_and_board vendor board)
    if (${_MSDK_BOARD_NAME_} STREQUAL "RA4M1_CLICKER")
        set(${vendor} "renesas" PARENT_SCOPE)
        set(${board} "ra4m1_clicker" PARENT_SCOPE)
    else()
        set(${vendor} "default" PARENT_SCOPE)
        set(${board} "default" PARENT_SCOPE)
        message(INFO
        "
        This Demo only supports setup with the following boards:
        - RA4M1 Clicker

        Please select a setup for one of these boards.
        "
        )
    endif()
endfunction()
