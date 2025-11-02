#ifndef COMP_COMPENGINE_HPP
#define COMP_COMPENGINE_HPP

#include "Fw/FPrimeBasicTypes.hpp"
#include "compress/Components/CompEngine/CompEngineComponentAc.hpp"

namespace COMP {

  /**
   * Active compression engine component implementation.
   *
   * FPP defines:
   *  - async COMPRESS_FILE(...)
   *  - async COMPRESS_FOLDER(...)
   *  - async SET_DEFAULT_ALGO(...)
   *  - sync  PING(...)
   * plus tlm/events/param.
   */
  class CompEngine final : public CompEngineComponentBase {
  public:
    //! Construct with name
    explicit CompEngine(const char* compName);

    //! Init active base
    void init(FwIndexType queueDepth, FwIndexType msgSize);

    //! Dtor
    ~CompEngine() override = default;

  private:
    // ------------------------------------------------------------------
    // Command handlers (override autocoded ones)
    // ------------------------------------------------------------------
    void COMPRESS_FILE_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq,
        COMP::Algo algo,
        const Fw::StringBase& path
    );

    void COMPRESS_FOLDER_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq,
        COMP::Algo algo,
        const Fw::StringBase& folder
    );

    void SET_DEFAULT_ALGO_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq,
        COMP::Algo algo
    ) override;

    void PING_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq,
        U32 key
    ) override;

    // ------------------------------------------------------------------
    // Helpers
    // ------------------------------------------------------------------
    bool algoIsValid(COMP::Algo algo) const;

    // return 0 on success, nonzero on error
    U32 doFileCompression(
        COMP::Algo algo,
        const Fw::StringBase& path,
        U32& bytesIn,
        U32& bytesOut
    );

    U32 doFolderCompression(
        COMP::Algo algo,
        const Fw::StringBase& folder,
        U32& bytesIn,
        U32& bytesOut
    );

  private:
    // runtime copy of “default algo” (separate from the autocoded private param)
    COMP::Algo m_runtimeDefaultAlgo;
  };

} // namespace COMP

#endif // COMP_COMPENGINE_HPP
