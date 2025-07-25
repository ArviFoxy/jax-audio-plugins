#ifndef JXAP_MLIR_MLIR_PIPELINE_H
#define JXAP_MLIR_MLIR_PIPELINE_H

#include <map>
#include <string>
#include <vector>

#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "jxap/mlir/passes.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/OwningOpRef.h"

namespace jxap {

/*
 * Formats a string encoding an MLIR tensor type.
 *
 * @param shape Dimensions of the tensor.
 * @param dtype Data type, such as "f32" or "i32".
 */
std::string MlirTensorType(const std::vector<int64_t>& shape, absl::string_view dtype);

// Replaces an argument with a constant.
struct ReplaceWithConstant {
  float value;

  ReplaceWithConstant(float value) : value(value) {}
};

// Refines an arguments type.
struct RefineType {
  std::string type;

  RefineType(const std::string& type) : type(type) {}
};

using ArgumentTransform = std::variant<ReplaceWithConstant, RefineType>;

/**
 * Applies passes to an MLIR function to:
 *   - Refines input types to static shapes.
 *   - Inlines constant arguments.
 *   - Replaces `jax.global_constant` arguments with constants.
 *   - Removes dynamicism from the MLIR whenever possible.
 *   - Remove shape assertions.
 *   - Optimize the output.
 */
absl::StatusOr<std::string> MlirPipeline(
    absl::string_view mlir_code, const std::vector<ArgumentTransform>& transforms,
    const std::map<std::string, ScalarValue>& global_constants);

}  // namespace jxap

#endif  // JXAP_MLIR_MLIR_PIPELINE_H
