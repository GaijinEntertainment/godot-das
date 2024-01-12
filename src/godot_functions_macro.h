#ifndef GODOT_FUNCTIONS_MACRO_H
#define GODOT_FUNCTIONS_MACRO_H

#define CTX_AT das::Context *ctx, das::LineInfoArg *at
#define CHECK_IF_NULL_MSG(PTR, MSG) if (PTR == nullptr) { ctx->throw_error_at(at, MSG); return {}; }
#define CHECK_IF_NULL(PTR) CHECK_IF_NULL_MSG(PTR, "dereferencing null pointer")
#define CHECK_IF_NULL_VOID(PTR) if (PTR == nullptr) { ctx->throw_error_at(at, "dereferencing null pointer"); return; }

#endif // GODOT_FUNCTIONS_MACRO_H
