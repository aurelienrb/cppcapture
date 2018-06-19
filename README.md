# ravencpp

Unofficial C++11 client library for [Sentry](https://sentry.io).

## Example

```cpp
try {
	RavenCaptureWarning("simple warning message")
		.WithTag("tag1", "value1");
	RavenCaptureError("simple error message")
		.WithExtra({ "extra1", "value" });

	throw std::logic_error{ "exception message" };
} catch (const std::exception & e) {
	RavenCaptureException(e).WithMessage("Exception was caught");
}
```

## Characteristics

- No third party dependencies
- Support for Linux (g++ / clang) and Windows (VC++)
- Compatible with legacy systems:
  - GCC >= 4.8
  - Ubuntu >= 14.04
  - CMake >= 2.8

## Limitations

- No HTTPS support

## Alternatives

https://github.com/truszkowski/raven-cpp

## License

MIT
