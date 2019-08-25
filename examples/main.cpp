#include <cppcapture/configure.h>
#include <cppcapture/default.h>

struct Error : public std::logic_error {
    Error() : std::logic_error{ "Error message" } {
        BreadcrumbsRecordInfo("throwing Error");
        // TODO: try to dinstinguish between the message we pass in the macro and an additional WithMessage we can
        // enqueue options:
        // - use different json tag (sentry only)
        // - use a vector of messages (repeat the field)
        // - append to the existing one
        // - print a warning when overwriting
        CaptureWarning("throwing a new Error").WithExtra("exception", what());
    }
};

static void doSomething() {
    BreadcrumbsRecordWarning("warning message from doSomething").WithExtra("extra", "value");
    CaptureError("error message from doSomething").WithTag("tag", "value").WithExtra("extra", "value");
    throw Error{};
}

int main() {
    cppcapture::ConfigureSentry(cppcapture::Client::Instance(), 1227606, "2e776f4b5f4349dfaa74146c71036871");

    try {
        BreadcrumbsResetHere();
        doSomething();
    } catch (const std::exception & e) {
        BreadcrumbsRecordException(e);
        CaptureException(e).WithMessage("Extra exception message");
    }
    // TODO: CaptureCrash()
}
