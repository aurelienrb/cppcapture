#include <cppcapture/configure.h>
#include <cppcapture/default.h>

struct Error : public std::logic_error {
    Error() : std::logic_error{ "Error message" } {
        // TODO: try to dinstinguish between the message we pass in the macro and an additional WithMessage we can
        // enqueue options:
        // - use different json tag (sentry only)
        // - use a vector of messages (repeat the field)
        // - append to the existing one
        // - print a warning when overwriting
        CaptureWarning("throwing a new Error").WithMessage(what());
    }
};

static void doSomething() {
    CaptureWarning("warning message from doSomething").WithTag({ "tag1", "value1" }).WithExtra({ "extra1", "value" });
    CaptureError("error message from doSomething").WithTag("tag1", "value1").WithExtra("extra1", "value");
    throw Error{};
}

int main() {
    cppcapture::ConfigureSentry(cppcapture::DefaultContext(), 1227606, "2e776f4b5f4349dfaa74146c71036871");

    try {
        doSomething();
    } catch (const std::exception & e) {
        CaptureException(e).WithMessage("Extra exception message");
    }
    // TODO: CaptureCrash()
}
