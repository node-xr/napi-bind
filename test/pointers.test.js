const expect = require("chai").expect;
const bind = require("./build/Release/napi_bind_test.node");

/**
 *   set_function(env, value, "create_struct_ptr", create_struct_ptr);
  set_function(env, value, "create_void_ptr", create_void_ptr);
  set_function(env, value, "create_null_ptr", create_null_ptr);
  set_function(env, value, "delete_struct_ptr", delete_struct_ptr);
  set_function(env, value, "identity_void_ptr", identity<void *>);
  set_function(env, value, "identity_test_ptr", identity<test_struct *>);
 */

describe("pointers", () => {
  const addr = bind.pointers.get_ptr_address;

  it("nullptr", () => {
    const nullptr = bind.pointers.create_null_ptr();
    expect(nullptr).to.be.null;
    expect(addr(nullptr)).to.eq(0n);
  });

  it("void *", () => {
    const ptr = bind.pointers.create_void_ptr();
    expect(addr(bind.pointers.identity_void_ptr(ptr))).to.eq(addr(ptr));
    bind.pointers.delete_void_ptr(ptr);
  });

  it("struct *", () => {
    const ptr = bind.pointers.create_struct_ptr();
    expect(addr(bind.pointers.identity_struct_ptr(ptr))).to.eq(addr(ptr));
    bind.pointers.delete_struct_ptr(ptr);
  });
});
