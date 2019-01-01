/*******************************************************************************
 *
 * \file
 * \brief Double free checker
 *
 * Author: Thomas Bailleux
 *
 * Contact: ikos@lists.nasa.gov
 *
 * Notices:
 *
 * Copyright (c) 2018-2019 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Disclaimers:
 *
 * No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF
 * ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS,
 * ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
 * OR FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE
 * ERROR FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO
 * THE SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
 * ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS,
 * RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS
 * RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY
 * DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE,
 * IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
 *
 * Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST
 * THE UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL
 * AS ANY PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS
 * IN ANY LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH
 * USE, INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM,
 * RECIPIENT'S USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD
 * HARMLESS THE UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS,
 * AS WELL AS ANY PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.
 * RECIPIENT'S SOLE REMEDY FOR ANY SUCH MATTER SHALL BE THE IMMEDIATE,
 * UNILATERAL TERMINATION OF THIS AGREEMENT.
 *
 ******************************************************************************/

#pragma once

#include <llvm/ADT/SmallVector.h>

#include <ikos/analyzer/checker/checker.hpp>

namespace ikos {
namespace analyzer {

/// \brief Double free checker
class DoubleFreeChecker final : public Checker {
private:
  using PointsToSet = core::PointsToSet< MemoryLocation* >;

public:
  /// \brief Constructor
  explicit DoubleFreeChecker(Context& ctx);

  /// \brief Get the checker name
  CheckerName name() const override;

  /// \brief Get the checker description
  const char* description() const override;

  /// \brief Check a statement
  void check(ar::Statement* stmt,
             const value::AbstractDomain& inv,
             CallContext* call_context) override;

private:
  /// \brief Check result
  struct CheckResult {
    CheckKind kind;
    Result result;
    llvm::SmallVector< ar::Value*, 2 > operands;
    JsonDict info;
  };

  /// \brief Check for a double free call
  CheckResult check_double_free(ar::IntrinsicCall* stmt,
                                const value::AbstractDomain& inv);

  /// \brief Check for a double free call on a memory location
  Result check_memory_location_free(ar::IntrinsicCall* stmt,
                                    const value::AbstractDomain& inv,
                                    MemoryLocation* addr);

  /// \brief Display the double free check, if requested
  bool display_double_free_check(Result result, ar::IntrinsicCall* stmt) const;

  /// \brief Display the double free check, if requested, with a memory location
  bool display_double_free_check(Result result,
                                 ar::IntrinsicCall* stmt,
                                 MemoryLocation* addr) const;

}; // end class DoubleFreeChecker

} // end namespace analyzer
} // end namespace ikos
