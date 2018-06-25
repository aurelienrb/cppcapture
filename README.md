# cppcapture
[![pipeline status](https://gitlab.com/aurelienrb/cppcapture/badges/master/pipeline.svg)](https://gitlab.com/aurelienrb/cppcapture/commits/master)

C++11 library to capture and send events to various monitoring platforms.

Supported platforms:
- https://sentry.io

## Example

```cpp
ConfigureSentry(DefaultContext(), sentryID, sentryToken);

try {
	CaptureWarning("simple warning message")
		.WithTag("tag1", "value1");
	CaptureError("simple error message")
		.WithExtra({ "extra1", "value" });

	throw std::logic_error{ "exception message" };
} catch (const std::exception & e) {
	CaptureException(e);
}
```

## Characteristics

- No third party dependencies
- Support for Linux (g++ / clang) and Windows (VC++)
- Compatible with legacy systems:
  - GCC >= 4.8
  - Ubuntu >= 14.04
  - CMake >= 2.8

## Limitation

- No HTTPS support

## Alternatives

- https://github.com/truszkowski/raven-cpp
- https://github.com/shikharkhattar/sentry-cpp

## License

MIT
