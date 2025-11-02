#include "compress/Components/CompEngine/CompEngine.hpp"

// for Fw::COMMAND_OK, Fw::COMMAND_VALIDATION_ERROR, ...
#include "Fw/FPrimeBasicTypes.hpp"

namespace COMP {

  // ------------------------------------------------------------------
  // Construction / init
  // ------------------------------------------------------------------

  CompEngine::CompEngine(const char* compName)
  : CompEngineComponentBase(compName)
  , m_runtimeDefaultAlgo(COMP::Algo::HUFFMAN)   // sensible default
  {
  }

  void CompEngine::init(FwIndexType queueDepth, FwIndexType msgSize) {
    // active base expects both
    CompEngineComponentBase::init(queueDepth, msgSize);

    // if you want to sync to the *parameter* value at startup, you can do:
    // Fw::ParamValid valid;
    // COMP::Algo prmVal = this->paramGet_DefaultAlgo(valid);
    // if (valid == Fw::ParamValid::VALID) {
    //   this->m_runtimeDefaultAlgo = prmVal;
    // }
  }

  // ------------------------------------------------------------------
  // Helpers
  // ------------------------------------------------------------------

  bool CompEngine::algoIsValid(COMP::Algo algo) const {
    switch (algo) {
      case COMP::Algo::HUFFMAN:
      case COMP::Algo::LZSS:
      case COMP::Algo::DCT:
        return true;
      default:
        return false;
    }
  }

  U32 CompEngine::doFileCompression(
      COMP::Algo algo,
      const Fw::StringBase& path,
      U32& bytesIn,
      U32& bytesOut
  ) {
    (void)algo;
    (void)path;
    // TODO: real Huffman/LZSS/DCT here
    bytesIn  = 1024U;
    bytesOut = 512U;
    return 0U;
  }

  U32 CompEngine::doFolderCompression(
      COMP::Algo algo,
      const Fw::StringBase& folder,
      U32& bytesIn,
      U32& bytesOut
  ) {
    (void)algo;
    (void)folder;
    // TODO: real folder strategy
    bytesIn  = 4096U;
    bytesOut = 1024U;
    return 0U;
  }

  // ------------------------------------------------------------------
  // Command handlers
  // ------------------------------------------------------------------

  void CompEngine::COMPRESS_FILE_cmdHandler(
      FwOpcodeType opCode,
      U32 cmdSeq,
      COMP::Algo algo,
      const Fw::StringBase& path
  ) {
    // if caller passed a garbage enum, reject
    if (!this->algoIsValid(algo)) {
      this->log_WARNING_LO_InvalidAlgorithm(static_cast<U8>(algo));
      this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::FORMAT_ERROR);
      return;
    }

    // log start
    this->log_ACTIVITY_HI_CompressionRequested(algo, path);

    U32 bytesIn  = 0U;
    U32 bytesOut = 0U;
    const U32 result = this->doFileCompression(algo, path, bytesIn, bytesOut);

    if (result == 0U) {
      this->log_ACTIVITY_LO_CompressionSucceeded(bytesIn, bytesOut);

      this->tlmWrite_LastAlgo(algo);
      const F32 ratio =
          (bytesIn > 0U) ? static_cast<F32>(bytesOut) / static_cast<F32>(bytesIn) : 0.0F;
      this->tlmWrite_LastRatio(ratio);
      this->tlmWrite_LastResultCode(0U);

      this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    } else {
      this->log_WARNING_HI_CompressionFailed(result);

      this->tlmWrite_LastAlgo(algo);
      this->tlmWrite_LastRatio(0.0F);
      this->tlmWrite_LastResultCode(result);

      this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::EXECUTION_ERROR);
    }
  }

  void CompEngine::COMPRESS_FOLDER_cmdHandler(
      FwOpcodeType opCode,
      U32 cmdSeq,
      COMP::Algo algo,
      const Fw::StringBase& folder
  ) {
    if (!this->algoIsValid(algo)) {
      this->log_WARNING_LO_InvalidAlgorithm(static_cast<U8>(algo));
      this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::INVALID_OPCODE);
      return;
    }

    this->log_ACTIVITY_HI_CompressionRequested(algo, folder);

    U32 bytesIn  = 0U;
    U32 bytesOut = 0U;
    const U32 result = this->doFolderCompression(algo, folder, bytesIn, bytesOut);

    if (result == 0U) {
      this->log_ACTIVITY_LO_CompressionSucceeded(bytesIn, bytesOut);

      this->tlmWrite_LastAlgo(algo);
      const F32 ratio =
          (bytesIn > 0U) ? static_cast<F32>(bytesOut) / static_cast<F32>(bytesIn) : 0.0F;
      this->tlmWrite_LastRatio(ratio);
      this->tlmWrite_LastResultCode(0U);

      this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    } else {
      this->log_WARNING_HI_CompressionFailed(result);

      this->tlmWrite_LastAlgo(algo);
      this->tlmWrite_LastRatio(0.0F);
      this->tlmWrite_LastResultCode(result);

      this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::EXECUTION_ERROR);
    }
  }

  void CompEngine::SET_DEFAULT_ALGO_cmdHandler(
      FwOpcodeType opCode,
      U32 cmdSeq,
      COMP::Algo algo
  ) {
    if (!this->algoIsValid(algo)) {
      this->log_WARNING_LO_InvalidAlgorithm(static_cast<U8>(algo));
      this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::INVALID_OPCODE);
      return;
    }

    // keep a runtime copy; the autocoded param copy is private to the base
    this->m_runtimeDefaultAlgo = algo;

    // reflect in tlm immediately
    this->tlmWrite_LastAlgo(algo);

    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void CompEngine::PING_cmdHandler(
      FwOpcodeType opCode,
      U32 cmdSeq,
      U32 key
  ) {
    (void) key;
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

} // namespace COMP
