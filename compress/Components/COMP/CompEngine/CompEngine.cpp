// ======================================================================
// \title  CompEngine.cpp
// \brief  Passive component for compressing files in the filesystem through F' placeholder. Use fprime-util impl
// ======================================================================
#include "COMP/CompEngine/CompEngine.hpp"
#include "CompressionLib/CompressionAlgo.hpp"

namespace COMP {

    void CompEngine :: COMPRESS_FILE_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        const Fw::CmdStringArg& path,
        const CompAlgo& algo,
        const U32 arg
    ) {
        // Implementation here
    }

    void CompEngine :: COMPRESS_DIR_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        const Fw::CmdStringArg& dir,
        const CompAlgo& algo,
        const U32 arg,
        const Fw::CmdStringArg& pattern
    ) {
        // Implementation here
    }

}
