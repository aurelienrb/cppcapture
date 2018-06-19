#include <raven/raven.h>

struct Error : public std::logic_error {
    Error() : std::logic_error{ "Error message" } {}
};

int main() {
    raven::Configure().setProjectID(1227606).setToken("2e776f4b5f4349dfaa74146c71036871");

    try {
        RavenCaptureWarning("simple warning message").WithTag("tag1", "value1").WithExtra("extra1", "value");
        RavenCaptureError("simple error message").WithTag({ "tag1", "value1" }).WithExtra({ "extra1", "value" });

        throw Error{};
    } catch (const std::exception & e) {
        RavenCaptureException(e).WithMessage("Extra message");
    }
    // TODO: RavenCaptureCrash()
}
