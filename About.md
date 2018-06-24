
# Features

- Intercept and send crash reports
- Send log events (errors, warnings, ...) to a third party platform or database (Sentry, Logmatic, Kafka, ...)
- Collect general metrics about your software (memory usage, CPU, ...) + custom ones as well
- Send metrics to a dedicated aggregation platform (Telegraph, Prometheus)
- Send specific events to Slack
- Filter / limit the events that are sent (avoid flooding)
- Configure the filters at runtime via a REST API
- Expose your metrics over HTTP directly from application (with a Web view dashboard!)
- Easy integration with logging libraries
- Easy integration with third party libraries: Boost, Qt, abseil, ..
- Compatible with legacy systems (Ubuntu 14.04, gcc 4.8)
- Compatible with Windows / VC++
- Simple and direct integration into your software (header only, cmake module)

# About

This library is an also an experiment on how to implement a *proxy library*.

The library offers a uniform interface to several backends. To function, it requires dependencies in order to:
- serialize the events to be sent as JSON strings
- send HTTP(S) requests to some servers
- parse URLs in order to configure the HTTP(S) end points to be used

Therefore the library can be used as follows:
- Header only (with manual configuration via some `#define`) or as a CMake library

## No thrid party dependency

The library can use its own home made JSON library (only for encoding requests) and HTTP/Socket library (HTTP only).

This is ideal if you want to give a quick trial to the library, or use it for a small project.

Pros:
  - very little setup: just add the source to your project and adjust your include paths
  - nothing else to install (Boost or any other system package)
  - small code size

Cons:
  - Limited to HTTP (no HTTPS support)
  - May be not as robust / performant as reknown implementations such as Boost sockets/HTTP?

## Use a third party HTTPS/network transport backend

### C++ Rest SDK (Casablanca)

### Boost

You can configure the library to use Boost sockets and even Boost ASIO.

Pros:
  - HTTPS support
  - Full control on how HTTP requests are sent and managed. More specifically, they can be managed by you own Boost ASIO instance rather than an extra internal thread you have no real control on.
  - Boost library usage remains hidden from the normal public API, thus limiting the impact on the clilent code (in terms of build time...)
  - Dependending on ASIO is not a bad thing in the long term as it is in the process of being standardized

Cons:
  - Your program will depend on Boost which is a huge library that is painful to install on Windows

### Bring Your Own Library

The framework is easily extendable and adding support for your own implementation of the transport layer is quite easy.

There is an example that uses the Qt framework for all the Network and JSON parts.

## Event logging service endpoints

This library was primarly designed to support Sentry. But it quickly appeared that with little changes it would integrate well with other end points.

Currently it offers an unified API to send your events to:
- Sentry (http://sentry.io)
- Telegraf (InfluxDB) in order to create Grafana dashboards
- Logmatic

### Flexible routing design

Addind support for a new end point is quite easy. So if you decide to move from one service to another, you won't have to update all your event macros, but just add the support for your new endpoint.


## FAQ

### The metrics should not be collected by a library but a dedicated stack

Yes, it should. But you need to setup that stack. You need a DevOps to help you configure tools such as Telegraf, Grafana, InfluxDB, Prometheus. This tools require time and effort to be properly setup. They also cost money because you have to host them somehwere.

Some teams already have them and that's great. Use them. But many teams don't have the time / the will to use them. It's too much effort to setup for a first evaluation.

This library hels you fill that gap. In less than 10 minutes, you can have a live dashboard about your running instance. And if you just have 2 or 3 servers, that may be sufficient. This is a huge improvement in situations where nothing was available. Think about it: your can quickly update your legacy system to report its crashes, metrics and errors.
