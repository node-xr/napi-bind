const expect = require("chai").expect;
const bind = require("./build/Release/napi_bind_test.node");

describe("containers", () => {
  const input = ["a", "b", "c"];

  it("list", () => {
    const output = bind.containers.identity_list(input);
    expect(output).not.to.eq(input);
    expect(output).to.eql(input);
  });

  it("optional", () => {
    expect(bind.containers.identity_optional("testing")).to.eq("testing");
    expect(bind.containers.identity_optional(null)).to.eq(undefined);
    expect(bind.containers.identity_optional(undefined)).to.eq(undefined);
  });

  it("vector", () => {
    const output = bind.containers.identity_vector(input);
    expect(output).not.to.eq(input);
    expect(output).to.eql(input);
  });
});
