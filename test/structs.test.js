const expect = require("chai").expect;
const bind = require("./build/Release/napi_bind_test.node");

describe("structs", () => {
  const test_input = {
    field_embedded: {
      field_nested: 1234
    },
    field: 4321,
    // field_optional_absent: undefined
    field_optional_present: 2032
  };
  const test_output = bind.structs.identity_struct(test_input);

  it("field", () => {
    expect(test_output.field).to.eq(4321);
  });

  it("field default", () => {
    expect(test_output.field_default).to.eq(5555);
  });

  it("field nested", () => {
    expect(test_output.field_embedded.field_nested).to.eq(1234);
  });

  it("field optional", () => {
    expect(test_output.field_optional_absent).to.be.undefined;
    expect(test_output.field_optional_present).to.eq(2032);
  });
});
