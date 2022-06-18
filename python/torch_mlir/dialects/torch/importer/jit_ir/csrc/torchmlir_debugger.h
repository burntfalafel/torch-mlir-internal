#include "module_builder.h"

#include "function_importer.h"
#include "ivalue_importer.h"
#include "mlir_utils.h"

#include "mlir-c/Bindings/Python/Interop.h"
#include "mlir-c/BuiltinAttributes.h"
#include "mlir-c/BuiltinTypes.h"
#include "mlir-c/Diagnostics.h"
#include "mlir-c/Registration.h"
#include "torch-mlir-c/Registration.h"

#include <llvm/Support/raw_ostream.h>

template <typename printType>
void printLLVMError(void (*func)(printType, MlirStringCallback, void *),
                    printType obj, std::string message) {
  std::stringstream ss;
  ss << message;
  auto stringCallback = [](MlirStringRef s, void *stringCallbackUserData) {
    auto *ssp = static_cast<std::stringstream *>(stringCallbackUserData);
    ssp->write(s.data, s.length);
  };
  func(obj, stringCallback, static_cast<void *>(&ss));
  llvm::errs() << ss.str() << "\n";
}
