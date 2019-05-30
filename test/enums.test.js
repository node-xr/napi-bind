const expect = require("chai").expect;
const bind = require("./build/Release/napi_bind_test.node");

describe("enums", () => {
  it("unscoped", () => {
    expect(bind.enums.identity_unscoped(1)).to.eq(1);
    expect(bind.enums.is_B(1)).to.eq(true);
    expect(bind.enums.is_B(2)).to.eq(false);
  });

  it("scoped", () => {
    expect(bind.enums.identity_scoped(20)).to.eq(20);
    expect(bind.enums.is_red(20)).to.eq(true);
    expect(bind.enums.is_red(19)).to.eq(false);
  });
});
