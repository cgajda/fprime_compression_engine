module COMP {
    @ Passive component for compressing files in the filesystem through F'

      enum CompAlgo : U8 {
    HUFFMAN = 0
    LZSS    = 1
    DCT     = 2
  }

    passive component CompEngine {


        # --------------------------------------------------------------
        # COMMANDS
        # --------------------------------------------------------------

        command COMPRESS_FILE(
        path: string size 128,
        algo: CompAlgo,
        arg: U32
        )

        command COMPRESS_DIR(
        dir: string size 128,
        algo: CompAlgo,
        arg: U32,
        pattern: string size 32
        )

        command SET_OUTPUT_DIR(
        outDir: string size 128
        )

        # --------------------------------------------------------------
        # TELEMETRY
        # --------------------------------------------------------------

        telemetry CE_LAST_IN_BYTES: U32
        telemetry CE_LAST_OUT_BYTES: U32
        telemetry CE_LAST_RATIO_X1K: U32
        telemetry CE_LAST_ALGO: CompAlgo
        telemetry CE_LAST_STATUS: U32

        telemetry CE_DIR_FILES_TOTAL: U32
        telemetry CE_DIR_FILES_OK: U32
        telemetry CE_DIR_IN_BYTES: U32
        telemetry CE_DIR_OUT_BYTES: U32

        # --------------------------------------------------------------
        # EVENTS
        # --------------------------------------------------------------

        event CE_FileDone(
        path: string size 128,
        inBytes: U32,
        outBytes: U32,
        algo: CompAlgo
        ) severity activity low

        event CE_DirDone(
        dir: string size 128,
        filesTotal: U32,
        filesOk: U32
        ) severity activity low

        event CE_Error(
        code: U32,
        info: string size 64
        ) severity warning high

        # --------------------------------------------------------------
        # PARAMETERS
        # --------------------------------------------------------------

        param CE_DATA_ROOT: string size 128 default "/home/cgajda/fprime_projects/compression_engine/data_sets"
        param CE_OUT_ROOT:  string size 128 default "/home/cgajda/fprime_projects/compression_engine/data_sets/out"
        param CE_DEF_BLOCK: U32 default 65536
        param CE_DEF_DCT_Q: U32 default 50

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending command registrations
        command reg port cmdRegOut

        @ Port for receiving commands
        command recv port cmdIn

        @ Port for sending command responses
        command resp port cmdResponseOut

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

    }
}