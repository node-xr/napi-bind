const expect = require('chai').expect;
const bind = require('./build/Release/napi_bind_test.node')

describe("strings", () => {
  it("std::string", () => {
    expect(bind.strings.identity_str("foo")).to.eq("foo");
  });

  it("std::string&", () => {
    expect(bind.strings.identity_str_ref("bar")).to.eq("bar");
  });

  it("char *", () => {
    expect(bind.strings.identity_char_ptr("baz")).to.eq("baz");
  });
});
