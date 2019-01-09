/**
 *  Copyright (c) 2014-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under both the Apache 2.0 license (found in the
 *  LICENSE file in the root directory of this source tree) and the GPLv2 (found
 *  in the COPYING file in the root directory of this source tree).
 *  You may select, at your option, one of the above-listed licenses.
 */

#pragma once

#include "row.h"

#include <sqlite3.h>

namespace osquery {

class TableRow;

using TableRowHolder = std::unique_ptr<TableRow>;

/**
 * Interface for accessing a table row. Implementations may be backed by
 * a map<string, string> or code generated with type-safe fields.
 */
class TableRow {
 public:
  TableRow() = default;
  virtual ~TableRow() {}

  // Non-copyable
  TableRow(const TableRow&) = delete;
  TableRow& operator=(const TableRow&) = delete;

  /**
   * Output the rowid of the current row into pRowid, returning SQLITE_OK if
   * successful or SQLITE_ERROR if not.
   */
  virtual int get_rowid(sqlite_int64 default_value,
                        sqlite_int64* pRowid) const = 0;
  /**
   * Invoke the appropriate sqlite3_result_xxx method for the given column, or
   * null if the value does not fit the column type.
   */
  virtual int get_column(sqlite3_context* ctx,
                         sqlite3_vtab* pVtab,
                         int col) = 0;
  /**
   * Serialize this row as key,value pairs into the given JSON object.
   */
  virtual Status serialize(JSON& doc, rapidjson::Value& obj) const = 0;

  /**
   * Clone this row.
   */
  virtual TableRowHolder clone() const = 0;

  /**
   * Convert this row to a string map.
   */
  virtual operator Row() const = 0;
};

} // namespace osquery
