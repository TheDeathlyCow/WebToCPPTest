# Web to C++ Communication Test in Unreal Engine 4

This project demonstrates communication between a web browser interface and C++ code in Unreal Engine 4.27. It uses an embedded `UWebBrowser` widget to display a web-based UI that lists assets found in the Unreal project's Content Browser.

## Functionality

- The embedded browser displays a list of asset names.
- When an asset name is clicked in the web UI, it sends that information to a C++ class in Unreal.
- The C++ code then logs the data received, and displays the selected item in a header at the top.


## Communication Method Overview

Unreal Engine does not provide a robust built-in API for bidirectional communication between JavaScript and C++. As such, this project uses a workaround:

- **C++ to JS**: This is straightforward and handled using `UWebBrowser::ExecuteJavascript(const FString&)`. The JavaScript string can contain arbitrary data from Unreal, which the WebBrowser will receive.
- **JS to C++**: This is a bit more complex. This communication direction is achieved using the `UWebBrowser::OnUrlChanged` delegate. When JavaScript wants to send data back to Unreal, it updates the URL's query parameters using `window.location.search`. Although the page content stays the same, this triggers the `OnUrlChanged` event in Unreal, allowing data to be parsed from the URL. A drawback of this approach is that it also reloads the page, so it is not 100% seamless from the user perspective. However, given the limitations of the built-in `UWebBrowser` widget, this was the most practical solution without relying on external plugins.

## Primary Logic Source Files 

The main C++ logic is contained in these source files:
- [`UBrowserWidget.h`](./WebToCPPTest/Source/WebToCPPTest/UBrowserWidget.h)
- [`UBrowserWidget.cpp`](./WebToCPPTest/Source/WebToCPPTest/UBrowserWidget.cpp)

The main JS logic is contained in this asset:
- [`index.html`](./WebToCPPTest/Content/Web/index.html)

## An Important Assumption

Modern web browsers restrict direct access to local files for security reasons. As such, I have assumed that the most appropriate way to demonstrate this functionality, without involving a separate server, is to embed the web interface directly within Unreal Engine using the `UWebBrowser` widget. This setup allows both the UI and the C++ logic to coexist in the same runtime environment, enabling communication without breaking browser security policies.